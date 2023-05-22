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
		//std::cout << m_queue.back() << std::endl;
		// Could unlock before notifing
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
		// Wait until we get enough entries in queue (or timeout) to write to file
		// cv.wait _Predicate if true, then thread wakes up  
		std::unique_lock lock(m_queueMutex);
		m_cv.wait_for(lock, std::chrono::seconds(3), [&] {
			return (m_queue.size() >= 20 || m_shutdown); });

		// thread woke up! Move everything to from m_queue, give up lock and start writing
		std::deque<std::string> buffer(std::make_move_iterator(m_queue.begin()), 
			std::make_move_iterator(m_queue.end()));
		m_queue.clear();
		lock.unlock();

		while (buffer.size())
		{
			std::cout << buffer.front() << std::endl;
			m_file << buffer.front() << "\n";
			buffer.pop_front();
		}
		m_file.flush();


		// shutdown at the end, so we flush what we had to file before exiting
		if (m_shutdown)
		{
			return;
		}
	}
}

void helperfunc(std::string str)
{
	for (int i = 0; i < 50; i++)
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

	for (int i = 0; i < 100; i++)
	{
		std::string str = "thread_" + std::to_string(i);
		std::thread([str] { helperfunc(str); }).detach();
	}
	std::this_thread::sleep_for(std::chrono::seconds(10));
	// performance:
	// 1st run		00:28:35.568 - 00:28:35.417 = 0.151 s
	// 2nd run		00:30:18.977 - 00:30:18.825 = 0.152 s
	// 3rd run		00:31:28.556 - 00:31:28.403 = 0.153 s
	// 100 threads x 50 logs = 5000 logs
	// 5000 logs / 0.152 s
	// 1 log / 0.030 ms (avg)

}