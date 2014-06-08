#include "notifier.h"
#include "string_buffer.h"

#include <curl/curl.h>

#include <stdlib.h>
#include <string.h>

typedef struct Notifier {
	char* url;
	CURL* curl;
	int notif_sent;
	int notif_successful;
	NotifierResult last_result;
	StringBuffer last_error;
} Notifier;

Notifier* notifier_create(const char* url)
{
	if (url == NULL) {
		return NULL;
	}

	Notifier* n = malloc(sizeof(*n));

	if (n == NULL) {
		return NULL;
	}

	n->notif_sent = 0;
	n->notif_successful = 0;

	n->url = strdup(url);

	curl_global_init(CURL_GLOBAL_ALL);
	n->curl = curl_easy_init();

	if (n->url == NULL && n->curl) {
		curl_easy_cleanup(n->curl);
	}

	if (n->curl == NULL) {
		free(n->url);
	}

	if (n->url == NULL || n->curl == NULL) {
		free(n);
		curl_global_cleanup();
		return NULL;
	}

	string_buffer_init(&n->last_error);

	return n;
}

void notifier_destroy(Notifier* n)
{
	if (n == NULL) { return; }

	free(n->url);
	n->url = NULL;

	curl_easy_cleanup(n->curl);
	n->curl = NULL;

	string_buffer_cleanup(&n->last_error);

	free(n);

	curl_global_cleanup();
}

static size_t write_func(void* contents, size_t size, size_t nmemb, void* userp) {
	const size_t sz = size * nmemb;
	if (sz == 0) { return sz; }

	StringBuffer* buf = userp;

	return string_buffer_append_bytes(buf, contents, sz) ? sz : 0;
}

static void quick_formadds(struct curl_httppost** frm, struct curl_httppost** lp, const char* key, const char* val) {
	curl_formadd(frm, lp, CURLFORM_COPYNAME, key, CURLFORM_COPYCONTENTS, val, CURLFORM_END);
}

static void quick_formaddi(struct curl_httppost** frm, struct curl_httppost** lp, const char* key, long long val) {
	char buf[24];
	sprintf(buf, "%lld", val);
	quick_formadds(frm, lp, key, buf);
}

static void quick_formaddf(struct curl_httppost** frm, struct curl_httppost** lp, const char* key, double val) {
	char buf[64];
	sprintf(buf, "%f", val);
	quick_formadds(frm, lp, key, buf);
}

static struct curl_httppost* build_form(const char* host, const struct ProcParseUptime* up,
		const struct ProcParseMemInfo* mem, const struct ProcParseLoadAvg* avg) {
	struct curl_httppost* formpost = NULL;
	struct curl_httppost* lastptr = NULL;
	quick_formadds(&formpost, &lastptr, "hostname", host);
	quick_formaddf(&formpost, &lastptr, "uptime[total]", up->total);
	quick_formaddf(&formpost, &lastptr, "uptime[idle]", up->idle);
	quick_formaddi(&formpost, &lastptr, "mem[free]", mem->free);
	quick_formaddi(&formpost, &lastptr, "mem[total]", mem->total);
	quick_formaddf(&formpost, &lastptr, "load[l1]", avg->load1);
	quick_formaddf(&formpost, &lastptr, "load[l5]", avg->load5);
	quick_formaddf(&formpost, &lastptr, "load[l15]", avg->load15);
	quick_formaddi(&formpost, &lastptr, "load[runningProcs]", avg->runningProcs);
	quick_formaddi(&formpost, &lastptr, "load[totalProcs]", avg->totalProcs);
	return formpost;
}

enum NotifierResult notifier_send(Notifier* n, const char* host, struct ProcParseLoadAvg* avg,
					struct ProcParseMemInfo* mem, struct ProcParseUptime* up)
{
	struct curl_httppost* formpost = build_form(host, up, mem, avg);

	curl_easy_setopt(n->curl, CURLOPT_URL, n->url);
	curl_easy_setopt(n->curl, CURLOPT_HTTPPOST, formpost);
	curl_easy_setopt(n->curl, CURLOPT_USERAGENT, "uptimed http://github.com/nibroc/UptimeMonitor/");

	StringBuffer rs;
	string_buffer_init(&rs);

	curl_easy_setopt(n->curl, CURLOPT_WRITEFUNCTION, &write_func);
	curl_easy_setopt(n->curl, CURLOPT_WRITEDATA, &rs);

	CURLcode res = curl_easy_perform(n->curl);

	curl_formfree(formpost);

	enum NotifierResult result;

	if (res == CURLE_OK) {
		if (strcmp(string_buffer_cstr(&rs), "ok") == 0) {
			result = NOTIFIER_SUCCESS;
			string_buffer_clear(&n->last_error);
		} else {
			result = NOTIFIER_ERR_RESPONSE;
			string_buffer_set_cstr(&n->last_error, "unexpected response from server: ");
			string_buffer_append_string_buffer(&n->last_error, &rs);
		}
	} else {
		result = NOTIFIER_ERR_CONNECTION;
		string_buffer_set_cstr(&n->last_error, "error connecting to host");
	}

	string_buffer_cleanup(&rs);

	return result;
}

const char* notifier_error(const Notifier* n) {
	return string_buffer_cstr(&n->last_error);
}
