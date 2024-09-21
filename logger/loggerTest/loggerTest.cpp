#include <gtest/gtest.h> // googletest header file

#include "../src/logger.h"
#include <filesystem>
// ---------------------------------------------------------------------
// LOGGER TEST
// ---------------------------------------------------------------------

class LoggerSuite : public ::testing::Test
{
public:
	static void SetUpTestSuite()
	{
		std::cout << "SetupTestSuite\n";
	}

	static void TearDownTestSuite() 
	{
		std::cout << "TeardownTestSuite\n";
	}
};

TEST_F(LoggerSuite, initialTest)
{
	Logger::getInstance().addLog("gTest", "initialTest", Logger::INFO);
	constexpr char filename[] = "log.txt";
	std::filesystem::path logPath{ filename };
	std::this_thread::sleep_for(std::chrono::milliseconds(50)); // 50 ms sleep for system to create file
	EXPECT_TRUE(std::filesystem::exists(logPath));
}
