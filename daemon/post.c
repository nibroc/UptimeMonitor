
#include "post.h"

#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct MemoryStruct {
	char* memory;
	size_t size;
};

static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp) {
	size_t realsize = size * nmemb;

	struct MemoryStruct *mem = (struct MemoryStruct *)userp;

	char* ptr;

	mem->memory = (char*) realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory == NULL) {
		/* out of memory! */
		printf("not enough memory (realloc returned NULL)\n");
		exit(1);
	}

	ptr = (char*) mem->memory;
	ptr += mem->size;

	memcpy(ptr, contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
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

int post(const char* url, const char* host, const struct Uptime* up, const struct MemInfo* mem, const struct LoadAvg* avg) {
	CURL *curl;
	CURLcode res;

	struct MemoryStruct memst;
	struct curl_httppost *formpost = NULL;
	struct curl_httppost *lastptr = NULL;

	curl = curl_easy_init();

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

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

		curl_easy_setopt(curl, CURLOPT_USERAGENT, "uptimed/0.1");

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteMemoryCallback);

		memst.memory = (char*) malloc(1);

		if (memst.memory == NULL) {
			printf("Crap\n");
			exit(1);
		}

		memst.size = 0;

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &memst);

		res = curl_easy_perform(curl);

		curl_easy_cleanup(curl);

		curl_formfree(formpost);

		free(memst.memory);

		return res;
	}

	return 1;
}
