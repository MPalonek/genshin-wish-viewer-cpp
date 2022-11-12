#include <gtest/gtest.h> // googletest header file

#include "../src/db.h"
#include "utilityTest.h"
#include <filesystem>

// ---------------------------------------------------------------------
// SQLITE DB TEST
// ---------------------------------------------------------------------

class SQLSuite : public ::testing::Test
{
public:
	static void SetUpTestSuite()
	{
		std::cout << "SetupTestSuite\n";
		// Remove db files, after previous tests
		dbTest::removeDbFiles();
	}

	static void TearDownTestSuite() 
	{
		std::cout << "TeardownTestSuite\n";
	}
};

TEST_F(SQLSuite, FirstTimeCreatingDb)
{
	std::unique_ptr<Database> db = std::make_unique<SQLDatabase>();
	
	std::filesystem::path dbPath{ g_filename};
	ASSERT_EQ(std::filesystem::exists(dbPath), true) << "Database file isn't created!";

	sqlite3* cHandle;
	int ret = sqlite3_open(g_filename, &cHandle);

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
