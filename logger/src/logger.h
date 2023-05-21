#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <array>
#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>


/*
To make a fast logger you need to:

	minimise the use of mutexes.
	detach the logging from the writing.
	minimise copying
	pre-alloc the buffers
		no allocs during logging.
	make a ring buffer
		fixed number of logging
		decide what should happen if its full
		slightly slower using a dequeue with minimises allocs
	use atomic indexes to the (ring) buffer
		if your system if fully loaded, you have to log often use the cv or wait until the logger thread gets scheduled by the OS.
		use the cv.notice_one where the index is updated if the front is sufficient in front of the back end.
*/


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
	static Logger& getInstance(logLevel level = INFO, std::filesystem::path path = "log.txt");
	void addLog(std::string callerName, std::string msg, logLevel level = INFO);

private:
	Logger(logLevel l, std::filesystem::path p);
	~Logger();

	std::string serializeTimePoint(const std::chrono::system_clock::time_point& time);
	std::string serializeTimePoint(const std::chrono::system_clock::time_point& time, std::string_view format);
	void writerLoop();

	std::chrono::system_clock m_clock;
	std::ofstream m_file;
	const std::filesystem::path m_filepath;
	const logLevel m_level;

	std::deque<std::string> m_queue;
	const size_t m_queueMaxSize = 10; //TODO not used currently
	std::mutex m_queueMutex;

	std::thread m_writerThread;	// thread
	std::condition_variable m_cv;
	bool m_shutdown = false;
	// flush_period

};

#endif /* LOGGER_H */