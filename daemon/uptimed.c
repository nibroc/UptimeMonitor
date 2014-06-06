#include "notifier.h"
#include "string_buffer.h"

#include "procparse/uptime.h"
#include "procparse/loadavg.h"
#include "procparse/meminfo.h"
#include "procparse/hostname.h"

#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>

const char usageMessage[] =
	"usage: uptimed [options]\n"
	"  -t -- target url to post data to [example: http://example.tld/api.php?token=some_secret_token]\n"
	"  -h -- show this message and exit\n"
	"  -v -- show version and exit\n"
	"  -r num -- repeat num times and exit [default -1 = repeat forever]\n"
	"  -i int -- the time to pause before shooting off another request\n"
	"  -s -- suppress messages\n";

const char versionMessage[] = "uptimed version 0.1 by Corbin Hughes (http://github.com/nibroc/)\n";

void do_or_die(int result, char* msg) {
	if (!result) { return; }
	fprintf(stderr, "Error: %s -- %s\n", msg, procparse_strerr(result));
	exit(EXIT_FAILURE);
}

int main(int argc, char** argv) {
	/* Count -1 means repeat forever */
	int count = -1;

	/* Repeat every 180 seconds by default */
	int repeat_seconds = 180;

	int silent = 0;

	char* url = NULL;

	int opt;
	while (-1 != (opt = getopt(argc, argv, "t:svhr:i:"))) {
		switch (opt) {
			case 't':
				url = optarg;
				break;
			case 'r':
				count = atoi(optarg);
				break;
			case 'i':
				repeat_seconds = atoi(optarg);
				break;
			case 's':
				silent = 1;
				break;
			case 'v':
				fputs(versionMessage, stderr);
				return EXIT_SUCCESS;
			case 'h':
			default:
				fputs(usageMessage, stderr);
				return EXIT_SUCCESS;
		}
	}

	if (url == NULL) {
		fprintf(stderr, "Error: You must provide the -t url option\n");
		fprintf(stderr, "%s", usageMessage);
		return EXIT_FAILURE;
	}

	char host[256];
	if (procparse_hostname(host, sizeof(host))) {
		fprintf(stderr, "Could not read hostname\n");
		return EXIT_FAILURE;
	}

	notifier* notify = notifier_create(url);

	do {
		struct ProcParseUptime up;
		do_or_die(procparse_uptime(&up), "Could not get uptime");
		struct ProcParseMemInfo mem;
		do_or_die(procparse_meminfo(&mem), "Could not get meminfo");
		struct ProcParseLoadAvg avg;
		do_or_die(procparse_loadavg(&avg), "could not get loadavg");

		if (!silent) {
			printf(
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

		struct timespec start, end;

		clock_gettime(CLOCK_MONOTONIC, &start);

		notifier_send(notify, &avg, &mem, &up);

		clock_gettime(CLOCK_MONOTONIC, &end);

		if (count == 0) {
			break;
		}

		double tdiff = ((double) (end.tv_sec - start.tv_sec)) + ( ((double)(end.tv_nsec - start.tv_nsec)) / 1000000000);
		if (repeat_seconds > tdiff) {
			usleep((repeat_seconds - tdiff) * 1000000 );
		}
	} while(1);

	notifier_destroy(notify);

	return 0;
}
