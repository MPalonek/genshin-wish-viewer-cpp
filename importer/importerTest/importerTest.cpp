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

TEST_F(ImporterSuite, ExtractTextFromImageStyle1_1)
{
	std::filesystem::path imgPath{ "img\\Style1_1.JPG" };
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

TEST_F(ImporterSuite, ExtractTextFromImageStyle1_2)
{
	std::filesystem::path imgPath{ "img\\Style1_2.JPG" };
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

TEST_F(ImporterSuite, ExtractTextFromImageStyle3_1)
{
	std::filesystem::path imgPath{ "img\\Style3_1.JPG" };
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

TEST_F(ImporterSuite, ExtractTextFromImageStyle3_2)
{
	std::filesystem::path imgPath{ "img\\Style3_2.JPG" };
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

TEST_F(ImporterSuite, ExtractTextFromImageStyle3_3)
{
	std::filesystem::path imgPath{ "img\\Style3_3.JPG"};
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

TEST_F(ImporterSuite, ExtractTextFromImageStyle3_4)
{
	std::filesystem::path imgPath{ "img\\Style3_4.JPG" };
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

TEST_F(ImporterSuite, ExtractTextFromImageStyle3_5)
{
	std::filesystem::path imgPath{ "img\\Style3_5.JPG" };
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
