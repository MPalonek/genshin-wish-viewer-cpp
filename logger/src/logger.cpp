#include "logger.h"

// Logger constructor
// m_level is const, so once constructed it can't be changed
Logger::Logger(logLevel l) : m_level(l)
{
	std::stringstream ss;
	ss << "Logger initialization. Level: " << logLevelStr[m_level] << " - " << serializeTimePoint(m_clock.now(), "%Y-%m-%d %H:%M:%S (%Z)");
	std::cout << ss.str() << std::endl;
}

// Get Logger instance (Singleton)
// LogLevel is set on first init for a lifetime!
Logger& Logger::getInstance(logLevel level)
{
	static Logger instance(level);
	return instance;
}

// Print log...
// TODO
void Logger::addLog(std::string callerName, std::string msg, logLevel level)
{
	if (m_level <= level)
	{
		std::cout << m_clock.now() << "\n";
		std::cout << serializeTimePoint(m_clock.now()) << " [" << logLevelStr[level] << "] " << callerName << ": " << msg << std::endl;
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

	m_sstream.str(""); // Clear the existing content
	m_sstream << std::put_time(&tm, "%H:%M:%S");
	m_sstream << '.' << std::setfill('0') << std::setw(3) << ms.count();
	return m_sstream.str();
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

	m_sstream.str(""); // Clear the existing content
	m_sstream << std::put_time(&tm, format.data());
	return m_sstream.str();
}

int main()
{
	std::cout << "test\n";

	Logger::getInstance(Logger::logLevel::DEBUG).addLog("main", "info message");
	Logger::getInstance().addLog("main", "warning message", Logger::WARNING);
	Logger::getInstance().addLog("main", "debug message", Logger::DEBUG);

}