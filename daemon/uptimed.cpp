#include "notifier.hpp"

#include "procparse/uptime.h"
#include "procparse/loadavg.h"
#include "procparse/meminfo.h"
#include "procparse/hostname.h"

#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <functional>

#include <unistd.h>
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

static void do_or_die(enum procparse_result result, const char* msg) {
	if (!result) { return; }
	std::fprintf(stderr, "Error: %s -- %s\n", msg, procparse_strerr(result));
	exit(EXIT_FAILURE);
}

static void print_uptime_info(FILE* stream, const char* host, const struct ProcParseUptime* up,
						const struct ProcParseMemInfo* mem, const struct ProcParseLoadAvg* avg) {
	std::fprintf(stream, "%s %.0f/%.0f %d/%d %.2f %.2f %.2f %d/%d\n",
		host, up->total, up->idle,
		mem->free, mem->total,
		avg->load1, avg->load5, avg->load15,
		avg->runningProcs, avg->totalProcs
	);
}

typedef struct ProgramOptions {
	int repetitions;
	int repeat_interval;
	bool silent;
	char* url;
} ProgramOptions;

static ProgramOptions parse_args(int argc, char** argv) {
	// By default, repeat forever, once every 3 minutes
	ProgramOptions opts = {-1, 180};

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
				opts.silent = true;
				break;
			case 'v':
				std::fputs(versionMessage, stderr);
				exit(EXIT_SUCCESS);
			case 'h':
			default:
				std::fputs(usageMessage, stderr);
				std::exit(EXIT_SUCCESS);
		}
	}

	if (opts.url == NULL) {
		std::fprintf(stderr, "Error: You must provide the -t url option\n");
		std::fprintf(stderr, "%s", usageMessage);
		std::exit(EXIT_FAILURE);
	}

	return opts;
}

static timespec get_time() {
	timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return now;
}

static double elapsed_seconds(const timespec& start, const timespec& end) {
	double seconds = end.tv_sec - start.tv_sec;
	double nanoseconds = end.tv_nsec - start.tv_nsec;
	return seconds + nanoseconds / 1.0e9;
}

template<typename Func>
static void repeat(int times, double interval, Func func) {
	do {
		timespec start_time = get_time();
		func();
		if (times >= 0 && --times == 0) {
			break;
		} else {
			double elapsed_secs = elapsed_seconds(start_time, get_time());
			double sleep_us = (interval - elapsed_secs) * 1.0e6;
			if (sleep_us > 0) {
				usleep(sleep_us);
			}
		}
	} while (1);
}

void notify_server(Notifier& notify, const ProgramOptions& opts) {
	char host[256];
	do_or_die(procparse_hostname(host, sizeof(host)), "Could not read hostname");
	struct ProcParseUptime up;
	do_or_die(procparse_uptime(&up), "Could not get uptime");
	struct ProcParseMemInfo mem;
	do_or_die(procparse_meminfo(&mem), "Could not get meminfo");
	struct ProcParseLoadAvg avg;
	do_or_die(procparse_loadavg(&avg), "could not get loadavg");

	if (!opts.silent) {
		print_uptime_info(stdout, host, &up, &mem, &avg);
	}
	auto result = notify.send(host, &avg, &mem, &up);
	if (result.is_failure()) {
		std::fprintf(stderr, "%s\n", result.error_message().c_str());
	}
}

int main(int argc, char** argv) {
	ProgramOptions opts = parse_args(argc, argv);

	Notifier notify(opts.url);

	repeat(opts.repetitions, opts.repeat_interval,
		std::bind(notify_server, std::ref(notify), opts));
}
