#include "notifier.h"

#include <curl/curl.h>

#include <cstdlib>
#include <cstring>
#include <string>
#include <stdexcept>

struct Notifier {
	std::string url;
	CURL* curl;
	int notif_sent;
	int notif_successful;
	NotifierResult last_result;
	std::string last_error;

	Notifier(std::string url)
		: url(std::move(url)), curl(nullptr), notif_sent(0), notif_successful(0),
		  last_result(NOTIFIER_SUCCESS), last_error()
	{
		curl_global_init(CURL_GLOBAL_ALL);
		curl = curl_easy_init();

		if (curl == nullptr) {
			throw std::runtime_error("Could not initialize curl");
		}
	}

	Notifier& operator=(const Notifier&) = delete;
	Notifier(const Notifier&) = delete;

	~Notifier() {
		curl_easy_cleanup(curl);
		curl_global_cleanup();
	}
};

Notifier* notifier_create(const char* url)
{
	return new (std::nothrow) Notifier(url);
}

void notifier_destroy(Notifier* n)
{
	delete n;
}

static size_t write_func(void* contents, size_t size, size_t nmemb, void* userp) {
	const size_t sz = size * nmemb;
	if (sz == 0) { return sz; }

	std::string& buf = *static_cast<std::string*>(userp);

	try {
		buf.append(static_cast<char*>(contents), sz);
		return sz;
	} catch (...) {
		return 0;
	}
}

static void quick_formadds(curl_httppost** frm, curl_httppost** lp, const char* key, const char* val) {
	curl_formadd(frm, lp, CURLFORM_COPYNAME, key, CURLFORM_COPYCONTENTS, val, CURLFORM_END);
}

static void quick_formaddi(curl_httppost** frm, curl_httppost** lp, const char* key, long long val) {
	char buf[24];
	sprintf(buf, "%lld", val);
	quick_formadds(frm, lp, key, buf);
}

static void quick_formaddf(curl_httppost** frm, curl_httppost** lp, const char* key, double val) {
	char buf[64];
	sprintf(buf, "%f", val);
	quick_formadds(frm, lp, key, buf);
}

static curl_httppost* build_form(const char* host, const ProcParseUptime* up,
		const ProcParseMemInfo* mem, const ProcParseLoadAvg* avg) {
	curl_httppost* formpost = NULL;
	curl_httppost* lastptr = NULL;
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

enum NotifierResult notifier_send(Notifier* n, const char* host, ProcParseLoadAvg* avg,
					ProcParseMemInfo* mem, ProcParseUptime* up)
{
	curl_httppost* formpost = build_form(host, up, mem, avg);

	curl_easy_setopt(n->curl, CURLOPT_URL, n->url.c_str());
	curl_easy_setopt(n->curl, CURLOPT_HTTPPOST, formpost);
	curl_easy_setopt(n->curl, CURLOPT_USERAGENT, "uptimed http://github.com/nibroc/UptimeMonitor/");

	std::string rs;

	curl_easy_setopt(n->curl, CURLOPT_WRITEFUNCTION, &write_func);
	curl_easy_setopt(n->curl, CURLOPT_WRITEDATA, &rs);

	CURLcode res = curl_easy_perform(n->curl);

	curl_formfree(formpost);

	enum NotifierResult result;

	if (res == CURLE_OK) {
		if (rs == "ok") {
			result = NOTIFIER_SUCCESS;
			n->last_error.clear();
		} else {
			result = NOTIFIER_ERR_RESPONSE;
			n->last_error = "unexpected response from server: " + rs;
		}
	} else {
		result = NOTIFIER_ERR_CONNECTION;
		n->last_error = "error connecting to host";
	}

	return n->last_result = result;
}

const char* notifier_error(const Notifier* n) {
	return n->last_error.c_str();
}
