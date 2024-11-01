#include <gtest/gtest.h> // googletest header file

#include "importer/src/importer.h"
#include <vector>
#include <thread>
#include <chrono>

// ---------------------------------------------------------------------
// IMPORTER TEST
// ---------------------------------------------------------------------
class ImporterSuite : public ::testing::Test
{
public:
	static void SetUpTestSuite()
	{
		std::cout << "SetupTestSuite\n";
		// Setup logger with ERROR so we dont see logs in gTest output and wait 2 seconds for init msg
		Logger::getInstance(Logger::ERROR);
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}

	static void TearDownTestSuite() 
	{
		std::cout << "TeardownTestSuite\n";
	}
};

TEST_F(ImporterSuite, ExtractTextFromImageStyle1_01)
{
	std::filesystem::path imgPath{ "img/Style1_01.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Cool Steel", "2020-11-07 14:53:16", 3);
	wishEntry wish2("Weapon", "Harbinger of Dawn", "2020-11-07 14:53:16", 3);
	wishEntry wish3("Weapon", "Skyrider Sword", "2020-11-07 14:53:16", 3);
	wishEntry wish4("Weapon", "Ferrous Shadow", "2020-11-07 14:53:16", 3);
	wishEntry wish5("Weapon", "Emerald Orb", "2020-11-07 14:53:16", 3);
	wishEntry wish6("Character", "Diluc", "2020-11-07 14:53:16", 5);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5, wish6 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle1_02)
{
	std::filesystem::path imgPath{ "img/Style1_2.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	
	wishEntry wish1("Weapon", "Bloodtainted Greatsword", "2021-02-08 17:24:17", 3);
	wishEntry wish2("Weapon", "Sacrificial Sword", "2021-02-08 17:24:06", 4);
	std::vector<wishEntry> wishList = { wish1, wish2 };

	Importer importer = Importer();
	std::vector<wishEntry> wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle1_03)
{
	std::filesystem::path imgPath{ "img/Style1_03.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Harbinger of Dawn", "2021-03-17 11:28:02", 3);
	wishEntry wish2("Weapon", "Raven Bow", "2021-03-17 11:28:02", 3);
	wishEntry wish3("Weapon", "Favonius Lance", "2021-03-17 11:28:02", 4);
	wishEntry wish4("Weapon", "Ferrous Shadow", "2021-03-17 11:28:02", 3);
	wishEntry wish5("Weapon", "Emerald Orb", "2021-03-17 11:28:02", 3);
	wishEntry wish6("Weapon", "Ferrous Shadow", "2021-03-17 11:28:02", 3);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5, wish6 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle1_04)
{
	std::filesystem::path imgPath{ "img/Style1_04.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Cool Steel", "2021-03-17 11:35:21", 3);
	wishEntry wish2("Weapon", "Emerald Orb", "2021-03-17 11:35:21", 3);
	wishEntry wish3("Weapon", "Bloodtainted Greatsword", "2021-03-17 11:35:21", 3);
	wishEntry wish4("Weapon", "Raven Bow", "2021-03-17 11:35:21", 3);
	wishEntry wish5("Character", "Sucrose", "2021-03-17 11:35:21", 4);
	wishEntry wish6("Weapon", "Harbinger of Dawn", "2021-03-17 11:35:21", 3);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5, wish6 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle1_05)
{
	std::filesystem::path imgPath{ "img/Style1_05.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Debate Club", "2021-05-22 14:07:04", 3);
	wishEntry wish2("Weapon", "Skyrider Sword", "2021-05-22 14:07:04", 3);
	wishEntry wish3("Weapon", "Raven Bow", "2021-05-22 14:07:04", 3);
	wishEntry wish4("Weapon", "Magic Guide", "2021-05-22 14:07:04", 3);
	wishEntry wish5("Weapon", "Cool Steel", "2021-05-22 14:07:04", 3);
	wishEntry wish6("Weapon", "Skyrider Sword", "2021-05-22 14:07:04", 3);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5, wish6 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle1_06)
{
	std::filesystem::path imgPath{ "img/Style1_06.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Rust", "2021-06-01 10:59:22", 4);
	//wishEntry wish1("Weapon", "xx", "xx", 3);
	std::vector<wishEntry> wishList = { wish1 };

	Importer importer = Importer();
	std::vector<wishEntry> wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle1_07)
{
	std::filesystem::path imgPath{ "img/Style1_07.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Harbinger of Dawn", "2021-09-01 08:45:47", 3);
	wishEntry wish2("Weapon", "Slingshot", "2021-09-01 08:45:47", 3);
	wishEntry wish3("Weapon", "Magic Guide", "2021-09-01 08:45:47", 3);
	wishEntry wish4("Character", "Kujou Sara", "2021-09-01 08:45:12", 4);
	wishEntry wish5("Weapon", "Sharpshooter's Oath", "2021-09-01 08:45:12", 3);
	wishEntry wish6("Weapon", "Harbinger of Dawn", "2021-09-01 08:45:12", 3);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5, wish6 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle1_08)
{
	std::filesystem::path imgPath{ "img/Style1_08.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Slingshot", "2021-09-01 09:24:29", 3);
	wishEntry wish2("Weapon", "Bloodtainted Greatsword", "2021-09-01 09:24:25", 3);
	wishEntry wish3("Character", "Xiangling", "2021-09-01 09:24:19", 4);
	wishEntry wish4("Weapon", "Thrilling Tales of Dragon Slayers", "2021-09-01 09:24:01", 3);
	wishEntry wish5("Weapon", "Raven Bow", "2021-09-01 09:24:01", 3);
	wishEntry wish6("Weapon", "Cool Steel", "2021-09-01 09:24:01", 3);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5, wish6 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle1_09)
{
	std::filesystem::path imgPath{ "img/Style1_09.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Thrilling Tales of Dragon Slayers", "2021-09-01 09:24:01", 3);
	wishEntry wish2("Weapon", "Raven Bow", "2021-09-01 09:23:27", 3);
	wishEntry wish3("Weapon", "The Widsith", "2021-09-01 09:23:27", 4);
	wishEntry wish4("Weapon", "Skyrider Sword", "2021-09-01 09:23:27", 3);
	wishEntry wish5("Weapon", "Bloodtainted Greatsword", "2021-09-01 09:23:27", 3);
	wishEntry wish6("Weapon", "Cool Steel", "2021-09-01 09:23:27", 3);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5, wish6 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle1_10)
{
	std::filesystem::path imgPath{ "img/Style1_10.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Magic Guide", "2021-10-17 19:55:10", 3);
	wishEntry wish2("Weapon", "Slingshot", "2021-10-17 19:55:10", 3);
	wishEntry wish3("Weapon", "Skyrider Sword", "2021-10-17 19:55:10", 3);
	wishEntry wish4("Weapon", "Favonius Greatsword", "2021-10-17 19:55:10", 4);
	wishEntry wish5("Weapon", "Skyrider Sword", "2021-10-17 19:55:10", 3);
	wishEntry wish6("Weapon", "Sharpshooter's Oath", "2021-10-17 19:55:10", 3);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5, wish6 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle1_11)
{
	std::filesystem::path imgPath{ "img/Style1_11.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Bloodtainted Greatsword", "2021-11-02 19:03:13", 3);
	wishEntry wish2("Weapon", "Thrilling Tales of Dragon Slayers", "2021-11-02 19:03:13", 3);
	wishEntry wish3("Weapon", "Thrilling Tales of Dragon Slayers", "2021-11-02 19:03:13", 3);
	wishEntry wish4("Weapon", "Slingshot", "2021-11-02 19:03:13", 3);
	wishEntry wish5("Weapon", "Magic Guide", "2021-11-02 19:03:13", 3);
	wishEntry wish6("Character", "Yanfei", "2021-11-02 19:03:13", 4);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5, wish6 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle1_12)
{
	std::filesystem::path imgPath{ "img/Style1_12.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Emerald Orb", "2021-11-02 19:03:37", 3);
	wishEntry wish2("Weapon", "Black Tassel", "2021-11-02 19:03:37", 3);
	wishEntry wish3("Weapon", "Magic Guide", "2021-11-02 19:03:37", 3);
	wishEntry wish4("Weapon", "Harbinger of Dawn", "2021-11-02 19:03:37", 3);
	wishEntry wish5("Weapon", "Slingshot", "2021-11-02 19:03:37", 3);
	wishEntry wish6("Weapon", "Wavebreaker's Fin", "2021-11-02 19:03:37", 4);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5, wish6 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle1_13)
{
	std::filesystem::path imgPath{ "img/Style1_13.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Skyrider Sword", "2021-11-02 21:01:22", 3);
	wishEntry wish2("Character", "Sayu", "2021-11-02 21:00:50", 4);
	wishEntry wish3("Character", "Yanfei", "2021-11-02 21:00:50", 4);
	wishEntry wish4("Weapon", "Thrilling Tales of Dragon Slayers", "2021-11-02 21:00:50", 3);
	wishEntry wish5("Character", "Thoma", "2021-11-02 21:00:50", 4);
	wishEntry wish6("Weapon", "Thrilling Tales of Dragon Slayers", "2021-11-02 21:00:50", 3);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5, wish6 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle1_14)
{
	std::filesystem::path imgPath{ "img/Style1_14.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Sharpshooter's Oath", "2021-11-02 20:45:25", 3);
	wishEntry wish2("Weapon", "Black Tassel", "2021-11-02 20:45:25", 3);
	wishEntry wish3("Weapon", "Debate Club", "2021-11-02 20:45:25", 3);
	wishEntry wish4("Weapon", "Emerald Orb", "2021-11-02 20:45:12", 3);
	wishEntry wish5("Character", "Hu Tao", "2021-11-02 18:40:12", 5);
	wishEntry wish6("Character", "Thoma", "2021-11-02 18:39:26", 4);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5, wish6 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle2_01)
{
	std::filesystem::path imgPath{ "img/Style2_01.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Slingshot", "2022-01-05 15:15:50", 3);
	wishEntry wish2("Weapon", "Thrilling Tales of Dragon Slayers", "2022-01-05 15:15:43", 3);
	wishEntry wish3("Weapon", "Black Tassel", "2022-01-05 15:14:37", 3);
	wishEntry wish4("Weapon", "Debate Club", "2022-01-05 15:14:37", 3);
	wishEntry wish5("Weapon", "Bloodtainted Greatsword", "2022-01-05 15:14:37", 3);
	wishEntry wish6("Character", "Yun Jin", "2022-01-05 15:14:37", 4);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5, wish6 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle2_02)
{
	std::filesystem::path imgPath{ "img/Style2_02.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Thrilling Tales of Dragon Slayers", "2022-01-25 18:12:21", 3);
	wishEntry wish2("Weapon", "Harbinger of Dawn", "2022-01-25 18:12:15", 3);
	wishEntry wish3("Weapon", "Ferrous Shadow", "2022-01-25 18:12:10", 3);
	wishEntry wish4("Weapon", "Thrilling Tales of Dragon Slayers", "2022-01-25 18:12:02", 3);
	wishEntry wish5("Weapon", "Harbinger of Dawn", "2022-01-25 18:11:51", 3);
	wishEntry wish6("Weapon", "Bloodtainted Greatsword", "2022-01-25 18:11:51", 3);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5, wish6 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle2_03)
{
	std::filesystem::path imgPath{ "img/Style2_03.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Raven Bow", "2022-01-28 21:39:46", 3);
	wishEntry wish2("Character", "Beidou", "2022-01-28 21:39:38", 4);
	wishEntry wish3("Weapon", "Skyrider Sword", "2022-01-28 21:39:26", 3);
	wishEntry wish4("Weapon", "Skyrider Sword", "2022-01-28 21:39:26", 3);
	wishEntry wish5("Weapon", "Harbinger of Dawn", "2022-01-28 21:39:26", 3);
	wishEntry wish6("Weapon", "Ferrous Shadow", "2022-01-28 21:39:26", 3);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5, wish6 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle2_04)
{
	std::filesystem::path imgPath{ "img/Style2_04.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Character", "Razor", "2022-01-28 21:56:11", 4);
	wishEntry wish2("Weapon", "Thrilling Tales of Dragon Slayers", "2022-01-28 21:56:07", 3);
	wishEntry wish3("Weapon", "Raven Bow", "2022-01-28 21:56:01", 3);
	wishEntry wish4("Character", "Ganyu", "2022-01-28 21:50:50", 5);
	wishEntry wish5("Weapon", "Harbinger of Dawn", "2022-01-28 21:50:46", 3);
	wishEntry wish6("Weapon", "Raven Bow", "2022-01-28 21:50:42", 3);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5, wish6 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle2_05)
{
	std::filesystem::path imgPath{ "img/Style2_05.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Sharpshooter's Oath", "2022-01-31 21:30:24", 3);
	wishEntry wish2("Weapon", "Cool Steel", "2022-01-31 21:30:21", 3);
	wishEntry wish3("Weapon", "Raven Bow", "2022-01-31 21:30:17", 3);
	wishEntry wish4("Weapon", "Emerald Orb", "2022-01-31 21:30:13", 3);
	wishEntry wish5("Weapon", "Debate Club", "2022-01-31 21:30:09", 3);
	wishEntry wish6("Weapon", "Emerald Orb", "2022-01-31 21:30:03", 3);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5, wish6 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle2_06)
{
	std::filesystem::path imgPath{ "img/Style2_06.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Character", "Razor", "2022-04-29 00:38:33", 4);
	wishEntry wish2("Weapon", "Emerald Orb", "2022-04-29 00:38:29", 3);
	wishEntry wish3("Weapon", "Skyrider Sword", "2022-04-29 00:38:26", 3);
	wishEntry wish4("Weapon", "Emerald Orb", "2022-04-29 00:38:21", 3);
	wishEntry wish5("Weapon", "Skyrider Sword", "2022-04-29 00:38:17", 3);
	wishEntry wish6("Weapon", "Skyrider Sword", "2022-04-29 00:38:10", 3);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5, wish6 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle2_07)
{
	std::filesystem::path imgPath{ "img/Style2_07.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "The Bell", "2022-05-12 22:43:41", 4);
	wishEntry wish2("Weapon", "Ferrous Shadow", "2022-05-12 22:43:32", 3);
	wishEntry wish3("Weapon", "Sharpshooter's Oath", "2022-05-12 22:43:23", 3);
	wishEntry wish4("Weapon", "Thrilling Tales of Dragon Slayers", "2022-05-12 22:43:16", 3);
	wishEntry wish5("Weapon", "Debate Club", "2022-05-12 22:43:10", 3);
	wishEntry wish6("Weapon", "Raven Bow", "2022-05-12 22:43:04", 3);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5, wish6 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle2_08)
{
	std::filesystem::path imgPath{ "img/Style2_08.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Thrilling Tales of Dragon Slayers", "2022-07-22 22:59:04", 3);
	wishEntry wish2("Weapon", "Skyrider Sword", "2022-07-22 22:59:04", 3);
	wishEntry wish3("Character", "Shikanoin Heizou", "2022-07-22 22:59:04", 4);
	wishEntry wish4("Weapon", "Cool Steel", "2022-07-22 22:59:04", 3);
	wishEntry wish5("Weapon", "Emerald Orb", "2022-07-22 22:59:04", 3);
	wishEntry wish6("Weapon", "Raven Bow", "2022-07-22 22:59:04", 3);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5, wish6 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle3_01)
{
	std::filesystem::path imgPath{ "img/Style3_01.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Character", "Collei", "2022-08-24 22:12:51", 4);
	wishEntry wish2("Weapon", "Debate Club", "2022-08-24 22:12:51", 3);
	wishEntry wish3("Weapon", "Harbinger of Dawn", "2022-08-24 22:12:51", 3);
	wishEntry wish4("Weapon", "Magic Guide", "2022-08-24 22:12:51", 3);
	wishEntry wish5("Weapon", "Raven Bow", "2022-08-24 22:12:51", 3);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle3_02)
{
	std::filesystem::path imgPath{ "img/Style3_02.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Cool Steel", "2022-11-02 11:35:09", 3);
	wishEntry wish2("Character", "Razor", "2022-11-02 11:35:09", 4);
	wishEntry wish3("Weapon", "Favonius Lance", "2022-11-02 11:35:09", 4);
	wishEntry wish4("Weapon", "Bloodtainted Greatsword", "2022-11-02 11:35:09", 3);
	wishEntry wish5("Weapon", "Harbinger of Dawn", "2022-11-02 11:35:09", 3);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle3_03)
{
	std::filesystem::path imgPath{ "img/Style3_03.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Debate Club", "2023-04-12 10:28:52", 3);
	wishEntry wish2("Weapon", "Thrilling Tales of Dragon Slayers", "2023-04-12 10:28:52", 3);
	wishEntry wish3("Weapon", "Sharpshooter's Oath", "2023-04-12 10:28:52", 3);
	wishEntry wish4("Character", "Candace", "2023-04-12 10:28:52", 4);
	wishEntry wish5("Weapon", "Bloodtainted Greatsword", "2023-04-12 10:28:52", 3);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle3_04)
{
	std::filesystem::path imgPath{ "img/Style3_04.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Black Tassel", "2023-04-17 12:36:02", 3);
	wishEntry wish2("Character", "Nahida", "2023-04-17 12:36:02", 5);
	wishEntry wish3("Weapon", "Cool Steel", "2023-04-17 12:36:02", 3);
	wishEntry wish4("Character", "Beidou", "2023-04-17 12:36:02", 4);
	wishEntry wish5("Weapon", "Ferrous Shadow", "2023-04-17 12:36:02", 3);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle3_05)
{
	std::filesystem::path imgPath{ "img/Style3_05.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Emerald Orb", "2023-04-17 12:37:34", 3);
	wishEntry wish2("Weapon", "Debate Club", "2023-04-17 12:37:34", 3);
	wishEntry wish3("Weapon", "Raven Bow", "2023-04-17 12:37:34", 3);
	wishEntry wish4("Weapon", "Sharpshooter's Oath", "2023-04-17 12:37:34", 3);
	wishEntry wish5("Character", "Kuki Shinobu", "2023-04-17 12:37:34", 4);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle3_06)
{
	std::filesystem::path imgPath{ "img/Style3_06.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Black Tassel", "2023-12-20 22:05:53", 3);
	wishEntry wish2("Weapon", "Bloodtainted Greatsword", "2023-12-20 22:05:53", 3);
	wishEntry wish3("Weapon", "Mistsplitter Reforged", "2023-12-20 22:05:53", 5);
	wishEntry wish4("Weapon", "Magic Guide", "2023-12-20 22:05:53", 3);
	wishEntry wish5("Weapon", "Dragon's Bane", "2023-12-20 22:05:53", 4);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle3_07)
{
	std::filesystem::path imgPath{ "img/Style3_07.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Harbinger of Dawn", "2024-04-21 10:49:22", 3);
	wishEntry wish2("Weapon", "Skyrider Sword", "2024-04-21 10:49:22", 3);
	wishEntry wish3("Weapon", "Slingshot", "2024-04-21 10:49:22", 3);
	wishEntry wish4("Weapon", "Thrilling Tales of Dragon Slayers", "2024-04-21 10:49:22", 3);
	wishEntry wish5("Weapon", "Thrilling Tales of Dragon Slayers", "2024-04-21 10:49:22", 3);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle3_08)
{
	std::filesystem::path imgPath{ "img/Style3_08.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Bloodtainted Greatsword", "2024-04-21 10:50:25", 3);
	wishEntry wish2("Weapon", "Cool Steel", "2024-04-21 10:50:25", 3);
	wishEntry wish3("Weapon", "The Stringless", "2024-04-21 10:50:25", 4);
	wishEntry wish4("Weapon", "Raven Bow", "2024-04-21 10:50:25", 3);
	wishEntry wish5("Weapon", "Debate Club", "2024-04-21 10:50:25", 3);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle3_09)
{
	std::filesystem::path imgPath{ "img/Style3_09.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Sacrificial Bow", "2024-07-09 23:29:11", 4);
	wishEntry wish2("Weapon", "Silvershower Heartstrings", "2024-07-09 23:29:11", 5);
	wishEntry wish3("Weapon", "Sharpshooter's Oath", "2024-07-09 23:29:11", 3);
	wishEntry wish4("Weapon", "Raven Bow", "2024-07-09 23:29:11", 3);
	wishEntry wish5("Weapon", "Thrilling Tales of Dragon Slayers", "2024-07-09 23:29:11", 3);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle3_10)
{
	std::filesystem::path imgPath{ "img/Style3_10.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Debate Club", "2024-07-09 23:29:11", 3);
	wishEntry wish2("Weapon", "Bloodtainted Greatsword", "2024-07-09 23:29:11", 3);
	wishEntry wish3("Weapon", "Emerald Orb", "2024-07-09 23:29:11", 3);
	wishEntry wish4("Weapon", "Raven Bow", "2024-07-09 23:29:11", 3);
	wishEntry wish5("Weapon", "Lithic Blade", "2024-07-09 23:29:11", 4);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle3_11)
{
	std::filesystem::path imgPath{ "img/Style3_11.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Ferrous Shadow", "2024-07-18 20:36:03", 3);
	wishEntry wish2("Weapon", "Skyrider Sword", "2024-07-18 20:36:03", 3);
	wishEntry wish3("Weapon", "Thrilling Tales of Dragon Slayers", "2024-07-18 20:36:03", 3);
	wishEntry wish4("Weapon", "Slingshot", "2024-07-18 20:36:03", 3);
	wishEntry wish5("Weapon", "Xiphos' Moonlight", "2024-07-18 20:36:03", 4);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle3_12)
{
	std::filesystem::path imgPath{ "img/Style3_12.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Rust", "2024-07-18 20:36:03", 4);
	wishEntry wish2("Weapon", "Dragon's Bane", "2024-07-18 20:36:03", 4);
	wishEntry wish3("Weapon", "Thrilling Tales of Dragon Slayers", "2024-07-18 20:36:03", 3);
	wishEntry wish4("Weapon", "Magic Guide", "2024-07-18 20:36:03", 3);
	wishEntry wish5("Weapon", "Skyrider Sword", "2024-07-18 20:36:03", 3);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle3_13)
{
	std::filesystem::path imgPath{ "img/Style3_13.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Weapon", "Cool Steel", "2024-07-18 21:27:44", 3);
	wishEntry wish2("Weapon", "Rust", "2024-07-18 21:27:44", 4);
	wishEntry wish3("Weapon", "Magic Guide", "2024-07-18 21:27:44", 3);
	wishEntry wish4("Weapon", "Black Tassel", "2024-07-18 21:27:44", 3);
	wishEntry wish5("Weapon", "Eye of Perception", "2024-07-18 21:27:44", 4);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

TEST_F(ImporterSuite, ExtractTextFromImageStyle3_14)
{
	std::filesystem::path imgPath{ "img/Style3_14.JPG" };
	ASSERT_EQ(std::filesystem::exists(imgPath), true) << "Image file doesn't exist!";

	wishEntry wish1("Character", "Razor", "2024-07-18 21:27:44", 4);
	wishEntry wish2("Weapon", "Ferrous Shadow", "2024-07-18 21:27:44", 3);
	wishEntry wish3("Weapon", "Cool Steel", "2024-07-18 21:27:44", 3);
	wishEntry wish4("Weapon", "Verdict", "2024-07-18 21:27:44", 5);
	wishEntry wish5("Weapon", "Sharpshooter's Oath", "2024-07-18 21:27:44", 3);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5 };

	auto importer = Importer();
	auto wishList2 = importer.ExtractWishesFromImage(imgPath.string());

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}

/*
TEST_F(SQLSuite, FirstTimeCreatingDbWithDiffFilename)
{
	std::string filename = "customName.db";
	std::unique_ptr<Database> db = std::make_unique<SQLDatabase>(filename);

	std::filesystem::path dbPath{ filename };
	ASSERT_EQ(std::filesystem::exists(dbPath), true) << "Database file isn't created!";

	sqlite3* cHandle;
	int ret = sqlite3_open(filename.c_str(), &cHandle);

	std::unordered_set<std::string> tables;
	dbTest::getTables(cHandle, tables);

	EXPECT_TRUE((tables.size() >= 5));
	EXPECT_EQ(tables.contains("systemInfo"), true);
	EXPECT_EQ(tables.contains("wishCharacter"), true);
	EXPECT_EQ(tables.contains("wishWeapon"), true);
	EXPECT_EQ(tables.contains("wishStandard"), true);
	EXPECT_EQ(tables.contains("wishBeginner"), true);

	int version = dbTest::getVersion(cHandle);
	EXPECT_EQ(version, g_version);

	sqlite3_close(cHandle);
}

TEST_F(SQLSuite, UpgradeDb)
{
	//TBD...
}

TEST_F(SQLSuite, DbNewerThanApp)
{
	//DeathTest
}

TEST_F(SQLSuite, InsertAndGetWish)
{
	std::unique_ptr<Database> db = std::make_unique<SQLDatabase>();
	std::string tableName = "wishStandard";
	wishEntry wish("Character", "Diluc", "2020-11-07 14:53:16", 5);
	
	db->insertWish(tableName, wish);
	
	std::vector<wishEntry> wishList;
	db->getWishes(tableName, wishList);

	EXPECT_EQ(wishList[0].itemType, wish.itemType);
	EXPECT_EQ(wishList[0].itemName, wish.itemName);
	EXPECT_EQ(wishList[0].date, wish.date);
	EXPECT_EQ(wishList[0].itemRarity, wish.itemRarity);
}

TEST_F(SQLSuite, InsertAndGet6Wishes)
{
	std::unique_ptr<Database> db = std::make_unique<SQLDatabase>();
	std::string tableName = "wishCharacter";

	wishEntry wish1("Weapon", "Skyrider Sword", "2021-01-12 18:37:29", 3);
	wishEntry wish2("Character", "Ganyu", "2021-01-12 18:37:29", 5);
	wishEntry wish3("Weapon", "Raven Bow", "2021-01-12 18:39:21", 3);
	wishEntry wish4("Character", "Xingqiu", "2021-01-12 18:39:21", 4);
	wishEntry wish5("Weapon", "Magic Guide", "2021-01-12 18:39:21", 3);
	wishEntry wish6("Weapon", "Slingshot", "2021-01-12 18:39:21", 3);
	std::vector<wishEntry> wishList = { wish1, wish2, wish3, wish4, wish5, wish6 };

	db->insertWishes(tableName, wishList);

	std::vector<wishEntry> wishList2;
	db->getWishes(tableName, wishList2);

	for (int i = 0; i < wishList.size(); i++)
	{
		EXPECT_EQ(wishList[i].itemType, wishList2[i].itemType);
		EXPECT_EQ(wishList[i].itemName, wishList2[i].itemName);
		EXPECT_EQ(wishList[i].date, wishList2[i].date);
		EXPECT_EQ(wishList[i].itemRarity, wishList2[i].itemRarity);
	}
}
*/
