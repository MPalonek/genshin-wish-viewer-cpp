#include "logger.h"

// Logger constructor
// m_level and m_filepath are const, so once constructed they can't be changed
Logger::Logger(logLevel l, std::filesystem::path p) : m_level(l), m_filepath(p)
{
	std::stringstream ss;
	ss << std::string(80, '*') << "\nLogger initialization...\n\tLevel: " << logLevelStr[m_level] << "\n\tDate: " <<
		serializeTimePoint(m_clock.now(), "%Y-%m-%d %H:%M:%S (%Z)") << "\n\tLogPath: " << std::filesystem::absolute(m_filepath);
	m_queue.emplace_back(std::move(ss.str()));
	m_file.open(m_filepath, std::ios::app); // std::ios::app for append mode
	m_writerThread = std::thread(&Logger::writerLoop, this);
}

// Logger destructor
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
// level and path are set on first init for a lifetime!
Logger& Logger::getInstance(logLevel level, std::filesystem::path path)
{
	static Logger instance(level, path);
	return instance;
}

// Add log to the internal queue, that will be printed to cout and file
// Function is thread safe.
// Example usecase:		addLog("main", "debug message", Logger::DEBUG);
// Example output:		18:33:54.208 [DEBUG] main: debug message
void Logger::addLog(std::string callerName, std::string_view msg, logLevel level)
{
	if (m_level <= level)
	{
		// Wait if we have too many entries on queue, writerThread will notify us
		std::unique_lock lock(m_queueMutex);
		m_cv.wait(lock, [&] { return m_queue.size() < m_flushQItemCount; });

		std::string log = serializeTimePoint(m_clock.now()) + " [" + logLevelStr[level] + "] " + callerName + ": " + std::string(msg);
		m_queue.emplace_back(std::move(log));
		// Could unlock before notifing
		m_cv.notify_one();
	}
}

// addLog overload for stringstream
void Logger::addLog(std::string callerName, std::stringstream& msg, logLevel level)
{
	addLog(callerName, msg.str(), level);
}

// Turns std::chrono::system_clock::time_point into specially formatted string.
// Default time_point output:	2023-05-21 10:55:44.7907268
// This function output:		12:55:44.792
// Notice that UTC has been changed into local time zone.
std::string Logger::serializeTimePoint(const std::chrono::system_clock::time_point& time)
{
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()) % 1000;
	std::time_t tt = std::chrono::system_clock::to_time_t(time);
	std::tm tm = localtime_safe(tt); // Locale time-zone

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
	std::tm tm = localtime_safe(tt); // Locale time-zone

	std::stringstream sstream;
	sstream  << std::put_time(&tm, format.data());
	return sstream.str();
}

// std::localtime is not thread-safe because it uses a static buffer (shared between threads)
// Both POSIX and Windows have safe alternatives used in this function
std::tm Logger::localtime_safe(std::time_t& timer)
{
	std::tm tm {};
#if defined(__unix__)
	localtime_r(&timer, &tm);
#elif defined(_MSC_VER)
	localtime_s(&tm, &timer);
#else
	static std::mutex mtx;
	std::lock_guard<std::mutex> lock(mtx);
	bt = *std::localtime(&timer);
#endif
	return tm;
}

// Loop for thread, that will take strings from m_queue and write them to console and file
void Logger::writerLoop()
{
	while (true)
	{
		// Wait until we get enough entries in queue (or timeout) to write
		// cv.wait _Predicate if true, then thread wakes up
		std::unique_lock lock(m_queueMutex);
		m_cv.wait_for(lock, std::chrono::seconds(m_flushPeriodInSec), [&] {
			return (m_queue.size() >= m_flushQItemCount || m_shutdown); });

		// thread woke up! Move everything from m_queue, give up lock and start writing
		std::deque<std::string> buffer(std::make_move_iterator(m_queue.begin()), 
			std::make_move_iterator(m_queue.end()));
		m_queue.clear();
		lock.unlock();
		m_cv.notify_one();

		while (buffer.size())
		{
			std::cout << buffer.front() << "\n";
			m_file << buffer.front() << "\n";
			buffer.pop_front();
		}
		std::cout.flush();
		m_file.flush();

		// shutdown at the end, so we log what we have in queue before exiting
		if (m_shutdown)
		{
			return;
		}
	}
}

/*
void helperfunc(std::string str)
{
	for (int i = 0; i < 50; i++)
	{
		Logger::getInstance().addLog(str, "Some random string__" + std::to_string(i));
	}
}

void printStatfunc(std::vector<std::pair<int, std::chrono::system_clock::time_point>> ar, bool detailed = true)
{
	if (detailed)
	{
		std::cout << "\n******************************************\n";
		for (auto& pair : ar)
		{
			std::cout << "\tsize: " << std::setw(5) << std::right << pair.first << "\ttime: " << pair.second << "\n";
		}
	}
	
	// Do the calculations
	std::sort(ar.begin(), ar.end(), [](const std::pair<int, std::chrono::system_clock::time_point>& a, const std::pair<int, std::chrono::system_clock::time_point>& b) {return a.first < b.first; });
	int min = ar[0].first;
	int median = ar.size() % 2 == 0 ? (ar[(ar.size()/2) - 1].first + ar[ar.size()/2].first)/2 : ar[ar.size()/2].first; // truncate division
	int max = ar[ar.size() - 1].first;

	int sum = 0;
	for (const auto& pair : ar) {
		sum += pair.first;
	}
	int mean = static_cast<int>(sum / ar.size());

	// Printing table
	//int maxNumWidth = std::to_string(ar.size()).length();
	//int maxAvgWidth = std::to_string(mean).length();
	//int maxMinWidth = std::to_string(min).length();
	//int maxMedWidth = std::to_string(median).length();
	//int maxMaxWidth = std::to_string(max).length();

	std::string headerNo = "no. of elem.";
	std::string headerAvg = "avg";
	std::string headerMin = "min";
	std::string headerMed = "median";
	std::string headerMax = "max";
	std::string sep = "   |   ";
	std::string headerRow = "|  " + headerNo + sep + headerAvg + sep + headerMin + sep + headerMed + sep + headerMax + "  |";

	std::cout << std::string(headerRow.length(), '*') << std::endl;
	std::cout << headerRow << std::endl;

	std::cout << "|  " << std::setw(headerNo.length()) << std::right << ar.size() << sep
		<< std::setw(headerAvg.length()) << std::right << mean << sep
		<< std::setw(headerMin.length()) << std::right << min << sep
		<< std::setw(headerMed.length()) << std::right << median << sep
		<< std::setw(headerMax.length()) << std::right << max << "  |" << std::endl;

	std::cout << std::string(headerRow.length(), '*') << std::endl;
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
	// 1st run		20:17:08.380 - 20:17:06.820 = 1.560 s
	// 2nd run		20:17:54.512 - 20:17:52.909 = 1.603 s
	// 3rd run		20:19:21.283 - 20:19:19.732 = 1.551 s
	// 100 threads x 50 logs = 5000 logs
	// 5000 logs / 1.571 s
	// 1 log / 0.314 ms (avg)

}
*/