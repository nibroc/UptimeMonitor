#include "notifier.h"

#include <curl/curl.h>

#include <stdlib.h>
#include <string.h>

typedef struct notifier {
	char* url;
	CURL* curl;
	int notif_sent;
	int notif_successful;
} notifier;

notifier* notifier_create(const char* url)
{
	if (url == NULL) {
		return NULL;
	}

	notifier* n = malloc(sizeof(*n));

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

	return n;
}

void notifier_destroy(notifier* n)
{
	if (n == NULL) { return; }

	free(n->url);
	n->url = NULL;

	curl_easy_cleanup(n->curl);
	n->curl = NULL;

	free(n);

	curl_global_cleanup();
}

typedef struct string_buffer {
	char str[256];
	size_t len;
} string_buffer;

static size_t write_func(void* contents, size_t size, size_t nmemb, void* userp) {
	const size_t sz = size * nmemb;
	if (sz == 0) { return sz; }

	string_buffer* buf = userp;

	// Copy as many bytes as we can
	size_t cpy = sz;
	if (buf->len + sz + 1 > sizeof(buf->str)) {
		cpy = sizeof(buf->str) - 1 - buf->len;
	}

	memcpy(&buf->str[buf->len], contents, cpy);
	buf->len += cpy;
	buf->str[buf->len] = '\0';

	return cpy;
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

static struct curl_httppost* build_form(const char* host, const struct Uptime* up,
		const struct MemInfo* mem, const struct LoadAvg* avg) {
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

int notifier_send(notifier* n, const char* host, struct LoadAvg* avg,
					struct MemInfo* mem, struct Uptime* up)
{
	struct curl_httppost* formpost = build_form(host, up, mem, avg);

	curl_easy_setopt(n->curl, CURLOPT_URL, n->url);
	curl_easy_setopt(n->curl, CURLOPT_HTTPPOST, formpost);
	curl_easy_setopt(n->curl, CURLOPT_USERAGENT, "uptimed http://github.com/nibroc/UptimeMonitor/");

	string_buffer rs = {0};
	curl_easy_setopt(n->curl, CURLOPT_WRITEFUNCTION, &write_func);
	curl_easy_setopt(n->curl, CURLOPT_WRITEDATA, &rs);

	CURLcode res = curl_easy_perform(n->curl);

	curl_formfree(formpost);

	if (res == CURLE_OK) {
		return strcmp(rs.str, "ok");
	} else {
		return 1;
	}
}
