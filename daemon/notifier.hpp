#ifndef UPMON_NOTIFIER_H
#define	UPMON_NOTIFIER_H

#include "procparse/loadavg.h"
#include "procparse/meminfo.h"
#include "procparse/uptime.h"

#include <curl/curl.h>

#include <cstdlib>
#include <stdexcept>
#include <string>

class NotifierResult {
public:
	enum class ResultCode {
		SUCCESS = 0,
		ERR_RESPONSE = 1,
		ERR_CONNECTION = 2
	};

	NotifierResult(ResultCode result_code, std::string error_message = std::string())
		: result_code_(result_code), error_message_(error_message)
	{ }

	bool is_success() const {
		return result_code() == ResultCode::SUCCESS;
	}

	bool is_failure() const {
		return !is_success();
	}

	ResultCode result_code() const {
		return result_code_;
	}

	const std::string& error_message() const {
		return error_message_;
	}

private:
	const ResultCode result_code_;
	const std::string error_message_;
};

class Notifier {
public:
	Notifier(std::string url);

	Notifier& operator=(const Notifier&) = delete;
	Notifier(const Notifier&) = delete;

	NotifierResult send(const char* host, struct ProcParseLoadAvg* avg,
						struct ProcParseMemInfo* mem, struct ProcParseUptime* up);

	~Notifier();

	int notifications_sent() const { return notif_sent; }
	int notifications_successful() const { return notif_successful; }

private:
	std::string url;
	CURL* curl;
	int notif_sent;
	int notif_successful;
};

#endif
