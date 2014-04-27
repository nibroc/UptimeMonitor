#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <getopt.h>

#include <ctime>

#include <curl/curl.h>

#include "post.h"

#include "procparse/uptime.h"
#include "procparse/loadavg.h"
#include "procparse/meminfo.h"
#include "procparse/hostname.h"

struct MemoryStruct {
	char* memory;
	size_t size;
};

const char usageMessage[] =
"usage: uptimed [options]\n"
"  -t -- target url to post data to [example: http://someesite.com/page.php?token=blah]\n"
"  -h -- show this message and exit\n"
"  -v -- show version and exit\n"
"  -r num -- repeat num times and exit [default -1 = repeat forever]\n"
"  -i int -- the time to pause before shooting off another request\n"
"  -s -- suppress messages\n";

const char versionMessage[] = "uptimed version 0.1 by Corbin Hughes\n";

using namespace std;

int main(int argc, char** argv)
{

	int opt;

	/* Count -1 means forever */
	int count = -1;

	/* Repeat every 180 seconds by default */
	int interval = 180;
	double dinterval;

	int silent = 0;

	char* url = NULL;

	struct Uptime up;
	struct MemInfo mem;
	struct LoadAvg avg;

	struct timespec start;
	struct timespec end;
	double tdiff;

	char host[128];

	while (-1 != (opt = getopt(argc, argv, "t:svhr:i:"))) {
		switch (opt) {
			case 't':
				url = optarg;
				break;
			case 'r':
				count = atoi(optarg);
				break;
			case 'i':
				interval = atoi(optarg);
				break;
			case 's':
				silent = 1;
				break;
			case 'v':
				fwrite(versionMessage, sizeof(char), strlen(versionMessage), stderr);
				return 1;
			case 'h':
			default:
				fwrite(usageMessage, sizeof(char), strlen(usageMessage), stderr);
				return 1;
		}
	}

	if (url == NULL) {
		fprintf(stderr, "Error: You must provide the -t url option\n");
		fprintf(stderr, "%s", usageMessage);
		return 1;
	}

	dinterval = (double) interval;

	hostname(host, 128);

	curl_global_init(CURL_GLOBAL_NOTHING);

	do {

		uptime(&up);
		meminfo(&mem);
		loadavg(&avg);

		if (!silent) {
			fprintf(
				stderr,
				"%s %.0f/%.0f %d/%d %.2f %.2f %.2f %d/%d\n",
				host, up.total, up.idle,
				mem.free, mem.total,
				avg.load1, avg.load5, avg.load15,
				avg.runningProcs, avg.totalProcs
			);
		}

		if (count != -1) {
			--count;
		}

		clock_gettime(CLOCK_MONOTONIC, &start);

		post(url, host, &up, &mem, &avg);

		clock_gettime(CLOCK_MONOTONIC, &end);

		tdiff = ((double) (end.tv_sec - start.tv_sec)) + ( ((double)(end.tv_nsec - start.tv_nsec)) / 1000000000 );

		if (count == 0) {
			break;
		}

		usleep((dinterval - tdiff) * 1000000 );

	} while(1);

	curl_global_cleanup();

	return 0;

}
