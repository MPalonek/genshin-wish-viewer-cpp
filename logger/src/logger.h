#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>
#include <chrono>
#include <array>


class Logger
{
public:
	enum logLevel {
		DEBUG = 0,
		INFO = 1,
		WARNING = 2,
		ERROR = 3
	};

	std::array<std::string, 4> logLevelStr = {
		"DEBUG",
		"INFO",
		"WARNING",
		"ERROR"
	};
	
	// disable copy and move
	Logger(const Logger&) = delete;
	void operator=(const Logger&) = delete;
	Logger(Logger&&) = delete;
	void operator=(Logger&&) = delete;

	// actual functions
	static Logger& getInstance(logLevel level = INFO);
	void addLog(std::string callerName, std::string msg, logLevel level = INFO);

private:
	Logger(logLevel l);
	~Logger() = default;

	std::string serializeTimePoint(const std::chrono::system_clock::time_point& time);
	std::string serializeTimePoint(const std::chrono::system_clock::time_point& time, std::string_view format);
	

	std::chrono::system_clock m_clock;
	std::stringstream m_sstream; // shared var...
	const std::string m_filepath; // TODO
	const logLevel m_level;

	// thread
	// queue
	// cond_var
	// mutex
	// flush_period

};

#endif /* LOGGER_H */