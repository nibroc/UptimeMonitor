
#include "post.h"

#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

static size_t write_func_ignore(void* contents, size_t size, size_t nmemb, void* userp) {
	(void) contents; (void) userp; // Suppress unused argument warnings
	return size * nmemb;
}

static void quick_formadds(struct curl_httppost** frm, struct curl_httppost** lp, const char* key, const char* val) {
	curl_formadd(frm, lp, CURLFORM_COPYNAME, key, CURLFORM_COPYCONTENTS, val, CURLFORM_END);
}

static void quick_formaddi(struct curl_httppost** frm, struct curl_httppost** lp, const char* key, long int val) {
	char buf[24];
	sprintf(buf, "%ld", val);
	quick_formadds(frm, lp, key, buf);
}

static void quick_formaddf(struct curl_httppost** frm, struct curl_httppost** lp, const char* key, double val) {
	char buf[64];
	sprintf(buf, "%f", val);
	quick_formadds(frm, lp, key, buf);
}

static void build_form(struct curl_httppost** formpost, struct curl_httppost** lastptr, const char* host,
						const struct Uptime* up, const struct MemInfo* mem, const struct LoadAvg* avg) {
	quick_formadds(formpost, lastptr, "hostname", host);
	quick_formaddf(formpost, lastptr, "uptime[total]", up->total);
	quick_formaddf(formpost, lastptr, "uptime[idle]", up->idle);
	quick_formaddi(formpost, lastptr, "mem[free]", mem->free);
	quick_formaddi(formpost, lastptr, "mem[total]", mem->total);
	quick_formaddf(formpost, lastptr, "load[l1]", avg->load1);
	quick_formaddf(formpost, lastptr, "load[l5]", avg->load5);
	quick_formaddf(formpost, lastptr, "load[l15]", avg->load15);
	quick_formaddi(formpost, lastptr, "load[runningProcs]", avg->runningProcs);
	quick_formaddi(formpost, lastptr, "load[totalProcs]", avg->totalProcs);
}

int post(const char* url, const char* host, const struct Uptime* up, const struct MemInfo* mem, const struct LoadAvg* avg) {
	CURL* curl = curl_easy_init();
	if (!curl) { return 1; }

	struct curl_httppost *formpost = NULL;
	struct curl_httppost *lastptr = NULL;
	build_form(&formpost, &lastptr, host, up, mem, avg);

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
	curl_easy_setopt(curl, CURLOPT_USERAGENT, "uptimed http://github.com/nibroc/UptimeMonitor/");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &write_func_ignore);

	CURLcode res = curl_easy_perform(curl);

	curl_easy_cleanup(curl);

	curl_formfree(formpost);

	return res;
}
