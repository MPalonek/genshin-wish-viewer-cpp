#include "logger.h"

// Logger constructor
// m_level is const, so once constructed it can't be changed
Logger::Logger(logLevel l, std::filesystem::path p) : m_level(l), m_filepath(p)
{
	std::stringstream ss;
	ss << "Logger initialization...\n\tLevel: " << logLevelStr[m_level] << "\n\tDate: " << serializeTimePoint(m_clock.now(), "%Y-%m-%d %H:%M:%S (%Z)") << "\n\tLogPath: " << std::filesystem::absolute(m_filepath);
	std::cout << ss.str() << std::endl;
	m_queue.emplace_back(std::move(ss.str()));
	//m_queue.resize(m_queueMaxSize);
	m_file.open(m_filepath, std::ios::app); // std::ios::app for append mode
	m_writerThread = std::thread(&Logger::writerLoop, this);
}

Logger::~Logger()
{
	addLog("Logger", "Logger shutting down...");
	m_shutdown = true;
	m_cv.notify_all();
	if (m_writerThread.joinable())
	{
		m_writerThread.join();
	}
	m_file.close();
}

// Get Logger instance (Singleton)
// LogLevel is set on first init for a lifetime!
Logger& Logger::getInstance(logLevel level, std::filesystem::path path)
{
	static Logger instance(level, path);
	return instance;
}

// Print log...
// TODO
void Logger::addLog(std::string callerName, std::string msg, logLevel level)
{
	if (m_level <= level)
	{
		// lock will be locked only once on construction and unlocked on destruction
		std::lock_guard lock(m_queueMutex);
		std::string log = serializeTimePoint(m_clock.now()) + " [" + logLevelStr[level] + "] " + callerName + ": " + msg;
		m_queue.emplace_back(std::move(log));
		std::cout << m_queue.back() << std::endl;
		m_cv.notify_one();
	}
}

// Turns std::chrono::system_clock::time_point into specially formatted string.
// Default time_point output:	2023-05-21 10:55:44.7907268
// This function output:		12:55:44.792
// Notice that UTC has been changed into local time zone.
std::string Logger::serializeTimePoint(const std::chrono::system_clock::time_point& time)
{
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()) % 1000;
	std::time_t tt = std::chrono::system_clock::to_time_t(time);
	std::tm tm = *std::localtime(&tt); // Locale time-zone

	std::stringstream sstream;
	sstream  << std::put_time(&tm, "%H:%M:%S");
	sstream << '.' << std::setfill('0') << std::setw(3) << ms.count();
	return sstream.str();
}

// Turns std::chrono::system_clock::time_point into string, according to passed format.
// Example usecase:		serializeTimePoint(system_clock.now(), "%Y-%m-%d %H:%M:%S (%Z)")
// Default time_point output:	2023-05-21 10:55:44.7907268
// This function output:		2023-05-21 12:55:44 (Central European Summer Time)
// Notice that UTC has been changed into local time zone.
std::string Logger::serializeTimePoint(const std::chrono::system_clock::time_point& time, std::string_view format)
{
	std::time_t tt = std::chrono::system_clock::to_time_t(time);
	//std::tm tm = *std::gmtime(&tt); // GMT (UTC)
	std::tm tm = *std::localtime(&tt); // Locale time-zone

	std::stringstream sstream;
	sstream  << std::put_time(&tm, format.data());
	return sstream.str();
}

void Logger::writerLoop()
{
	while (true)
	{
		// Wait until main() sends data
		// cv.wait _Predicate if true, then thread wakes up  
		std::unique_lock lock(m_queueMutex);
		m_cv.wait(lock, [&] {
			return (!m_queue.empty() || m_shutdown); });

		if (m_shutdown)
		{
			return;
		}
		m_file << m_queue.front() << std::endl;
		m_queue.pop_front();
	}
}

void helperfunc(std::string str)
{
	for (int i = 0; i < 10; i++)
	{
		Logger::getInstance().addLog(str, "Some random string__" + std::to_string(i));
	}
}

int main()
{
	std::cout << "test\n";

	Logger::getInstance(Logger::logLevel::DEBUG).addLog("main", "info message");
	Logger::getInstance().addLog("main", "warning message", Logger::WARNING);
	Logger::getInstance().addLog("main", "debug message", Logger::DEBUG);

	std::this_thread::sleep_for(std::chrono::seconds(4));

	for (int i = 0; i < 50; i++)
	{
		std::string str = "thread_" + std::to_string(i);
		std::thread([str] { helperfunc(str); }).detach();
	}
	std::this_thread::sleep_for(std::chrono::seconds(10));
	// performance:
	// 1st run		18:34:38.445 - 18:34:38.272 = 173
	// 2nd run		18:42:36.322 - 18:42:36.150 = 172
	// 3rd run		18:45:55.266 - 18:45:55.093 = 173
	// 50 threads x 10 logs = 500 logs
	// 500 logs / 173 ms
	// 1 log / 0.346 ms (avg)

}