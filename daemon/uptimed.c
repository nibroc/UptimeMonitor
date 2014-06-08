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

const char versionMessage[] = "uptimed version 0.2 by Corbin Hughes (http://github.com/nibroc/)\n";

void do_or_die(int result, char* msg) {
	if (!result) { return; }
	fprintf(stderr, "Error: %s -- %s\n", msg, procparse_strerr(result));
	exit(EXIT_FAILURE);
}

void print_uptime_info(FILE* stream, const char* host, const struct ProcParseUptime* up,
						const struct ProcParseMemInfo* mem, const struct ProcParseLoadAvg* avg) {
	fprintf(stream, "%s %.0f/%.0f %d/%d %.2f %.2f %.2f %d/%d\n",
		host, up->total, up->idle,
		mem->free, mem->total,
		avg->load1, avg->load5, avg->load15,
		avg->runningProcs, avg->totalProcs
	);
}

typedef struct program_options {
	int repetitions;
	int repeat_interval;
	int silent;
	char* url;
} program_options;

program_options parse_args(int argc, char** argv) {
	// By default, repeat forever, once every 3 minutes
	program_options opts = {-1, 180};

	int opt;
	while (-1 != (opt = getopt(argc, argv, "t:svhr:i:"))) {
		switch (opt) {
			case 't':
				opts.url = optarg;
				break;
			case 'r':
				opts.repetitions = atoi(optarg);
				break;
			case 'i':
				opts.repeat_interval = atoi(optarg);
				break;
			case 's':
				opts.silent = 1;
				break;
			case 'v':
				fputs(versionMessage, stderr);
				exit(EXIT_SUCCESS);
			case 'h':
			default:
				fputs(usageMessage, stderr);
				exit(EXIT_SUCCESS);
		}
	}

	if (opts.url == NULL) {
		fprintf(stderr, "Error: You must provide the -t url option\n");
		fprintf(stderr, "%s", usageMessage);
		exit(EXIT_FAILURE);
	}

	return opts;
}

int main(int argc, char** argv) {
	program_options opts = parse_args(argc, argv);

	char host[256];
	do_or_die(procparse_hostname(host, sizeof(host)), "Could not read hostname");

	Notifier* notify = notifier_create(opts.url);

	do {
		struct ProcParseUptime up;
		do_or_die(procparse_uptime(&up), "Could not get uptime");
		struct ProcParseMemInfo mem;
		do_or_die(procparse_meminfo(&mem), "Could not get meminfo");
		struct ProcParseLoadAvg avg;
		do_or_die(procparse_loadavg(&avg), "could not get loadavg");

		if (!opts.silent) {
			print_uptime_info(stdout, host, &up, &mem, &avg);
		}

		struct timespec start, end;

		clock_gettime(CLOCK_MONOTONIC, &start);
		if (notifier_send(notify, host, &avg, &mem, &up) != NOTIFIER_SUCCESS) {
			fprintf(stderr, "%s\n", notifier_error(notify));
		}
		clock_gettime(CLOCK_MONOTONIC, &end);

		if (opts.repetitions != -1 && --opts.repetitions == 0) {
			break;
		}

		double tdiff = ((double) (end.tv_sec - start.tv_sec)) + ( ((double)(end.tv_nsec - start.tv_nsec)) / 1000000000);
		if (opts.repeat_interval > tdiff) {
			usleep((opts.repeat_interval - tdiff) * 1000000 );
		}
	} while(1);

	notifier_destroy(notify);

	return 0;
}
