#include "db.h"
#include <iostream>
#include <format>


// https://videlais.com/2018/12/13/c-with-sqlite3-part-3-inserting-and-selecting-data/


SQLDatabase::SQLDatabase(std::string dbF)
{
	dbFilename = dbF;
	init();
	setupDB();
}

SQLDatabase::~SQLDatabase()
{
	int ret = sqlite3_close(connectionHandle);
}

int callback(void* notUsed, int argc, char** argv, char** colName)
{
	// Return successful
	return 0;
}

int infoTableCallback(void* notUsed, int argc, char** argv, char** colName)
{
	for (int i = 0; i < argc; i++)
	{
		// Show column name, value, and newline
		std::cout << colName[i] << ": " << argv[i] << std::endl;
	}

	// Return successful
	return 0;
}

void SQLDatabase::getWishes(std::string tableName, std::vector<wishEntry> &wishList)
{
	sqlite3_stmt* stmt;
	std::string command = std::format("SELECT * FROM {};", tableName);
	int rc = sqlite3_prepare_v2(connectionHandle, command.c_str(), -1, &stmt, NULL);

	if (rc != SQLITE_OK) {
		std::cout << "error: " << sqlite3_errmsg(connectionHandle);
		return;
	}

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		// Convert const unsigned char* to const char*. 
		// People typically use an (unsigned char *) type to indicate that the data is binary and not plain ASCII text.
		// Meaning you might need some encoding/decoding here, if data is UTF-8 symbol? With ICU library?
		// Example of expected row:
		// 1|Character|Diluc|2020-11-07 14:53:16|5
		const char* type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		const char* name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
		const char* date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
		int rarity = sqlite3_column_int(stmt, 4);

		if (!type || !name || !date)
		{
			// One of the pointers is null, db is corrupted
			exit(1);
		}

		wishList.push_back(wishEntry(type, name, date, rarity));
	}

	if (rc != SQLITE_DONE) {
		std::cout << "error: " << sqlite3_errmsg(connectionHandle);
	}
	sqlite3_finalize(stmt);
}

void SQLDatabase::insertWish(std::string tableName, wishEntry wish)
{
	char* errMsg = 0;
	std::string command = std::format("INSERT INTO {}(itemType, itemName, timeReceived, itemRarity) "
		"VALUES(\"{}\",\"{}\",\"{}\",\"{}\");",
		tableName, wish.itemType, wish.itemName, wish.date, wish.itemRarity);
	int rc = sqlite3_exec(connectionHandle, command.c_str(), callback, 0, &errMsg);
	if (rc != SQLITE_OK)
	{
		// error occured
		std::cout << "Error: " << errMsg << std::endl;
	}

	std::cout << "inserted Wish\n";
}

void SQLDatabase::insertWishes(std::string tableName, std::vector<wishEntry> wishVec)
{
	char* errMsg = 0;
	std::vector<std::string> commands = { "BEGIN;" };
	
	for (auto &wish : wishVec)
	{
		commands.push_back(std::format("INSERT INTO {}(itemType, itemName, timeReceived, itemRarity) "
			"VALUES(\"{}\",\"{}\",\"{}\",\"{}\");",
			tableName, wish.itemType, wish.itemName, wish.date, wish.itemRarity));
	}
	commands.push_back("COMMIT;");

	for (auto &stmt : commands)
	{
		int rc = sqlite3_exec(connectionHandle, stmt.c_str(), callback, 0, &errMsg);
		if (rc != SQLITE_OK)
		{
			// error occured
			std::cout << "Error: " << errMsg << std::endl;
		}
	}
	std::cout << "inserted multiple Wishes\n";
}

void SQLDatabase::init()
{
	int ret = sqlite3_open(dbFilename.c_str(), &connectionHandle);
	if (ret != SQLITE_OK)
	{
		// error occured
		return;
	}
}

void SQLDatabase::setupDB()
{
	// First search db for systemInfo table
	std::unordered_set<std::string> tables;
	getTables(tables);

	if (tables.contains("systemInfo"))
	{
		// We got systemInfo table, let's check it's latest version
		int version = infoTableGetLatestVersion();
		if (version == 0)
		{
			// There was error - what now?
			std::cout << "Version error, what now?\n";

		}
		else
		{
			if (g_version > version)
			{
				// upgrade
				std::cout << "Upgrade...\n";
			}
			else if (g_version < version)
			{
				// database version is higher than app, we cannot handle that so terminate
				std::cout << "Terminate...\n";
				exit(1);
			}
			else
			{
				// version matches, we can proceed
				std::cout << "Version matches!\n";
			}
		}
	}
	else
	{
		// Didn't find systemInfo, we assume that it must be empty db
		// Let's create all the required tables
		createTables();
	}

}

void SQLDatabase::getTables(std::unordered_set<std::string> &tables)
{
	sqlite3_stmt* stmt;
	const char* sql = "SELECT * FROM sqlite_master WHERE type='table';";
	int rc = sqlite3_prepare_v2(connectionHandle, sql, -1, &stmt, NULL);

	if (rc != SQLITE_OK) {
		std::cout << "error: " << sqlite3_errmsg(connectionHandle);
		return;
	}

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		// Convert const unsigned char* to const char*. 
		// People typically use an (unsigned char *) type to indicate that the data is binary and not plain ASCII text.
		// Meaning you might need some encoding/decoding here, if data is UTF-8 symbol? With ICU library?
		// Example of expected row:
		// table|systemInfo|systemInfo|2|CREATE TABLE systemInfo (...)
		const char* p = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		if (p == NULL)
			p = "";
		tables.insert(std::string(p));
	}

	if (rc != SQLITE_DONE) {
		std::cout << "error: " << sqlite3_errmsg(connectionHandle);
	}
	sqlite3_finalize(stmt);
}

int SQLDatabase::infoTableGetLatestVersion()
{
	sqlite3_stmt* stmt;
	const char* sql = "SELECT version FROM systemInfo";
	int rc = sqlite3_prepare_v2(connectionHandle, sql, -1, &stmt, NULL);
	int version = 0;

	if (rc != SQLITE_OK) {
		std::cout << "error: " << sqlite3_errmsg(connectionHandle);
		return version;
	}

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		int id = sqlite3_column_int(stmt, 0);
		std::cout << id;
		if (id > version)
			version = id;
	}

	if (rc != SQLITE_DONE) {
		std::cout << "error: " << sqlite3_errmsg(connectionHandle);
	}
	sqlite3_finalize(stmt);

	return version;
}

void SQLDatabase::createTables()
{
	createInfoTable();
	createWishTable();
}

void SQLDatabase::createInfoTable()
{
	char* errMsg = 0;
	std::string command = "CREATE TABLE IF NOT EXISTS systemInfo (creationDate date DEFAULT CURRENT_TIMESTAMP,"
		"version integer NOT NULL PRIMARY KEY);";
	sqlite3_exec(connectionHandle, command.c_str(), callback, 0, &errMsg);

	command = "INSERT INTO systemInfo (version) VALUES (" + std::to_string(g_version) + ");";
	auto rc = sqlite3_exec(connectionHandle, command.c_str(), callback, 0, &errMsg);

	if (rc != SQLITE_OK)
	{
		// error occured
		std::cout << "Error: " << errMsg << std::endl;
	}
	std::cout << "created Info Table\n";
}

void SQLDatabase::createWishTable()
{
	char* errMsg = 0;
	std::vector<std::string> commands = { "BEGIN;",
		"CREATE TABLE IF NOT EXISTS wishCharacter(id integer PRIMARY KEY AUTOINCREMENT, itemType text"
		" NOT NULL, itemName text NOT NULL, timeReceived date NOT NULL, itemRarity integer NOT NULL);",
		"CREATE TABLE IF NOT EXISTS wishWeapon (id integer PRIMARY KEY AUTOINCREMENT, itemType text"
		" NOT NULL, itemName text NOT NULL, timeReceived date NOT NULL, itemRarity integer NOT NULL);",
		"CREATE TABLE IF NOT EXISTS wishStandard (id integer PRIMARY KEY AUTOINCREMENT, itemType text"
		" NOT NULL, itemName text NOT NULL, timeReceived date NOT NULL, itemRarity integer NOT NULL);",
		"CREATE TABLE IF NOT EXISTS wishBeginner (id integer PRIMARY KEY AUTOINCREMENT, itemType text"
		" NOT NULL, itemName text NOT NULL, timeReceived date NOT NULL, itemRarity integer NOT NULL);",
		"COMMIT;" };
	for (auto &stmt : commands)
	{
		int rc = sqlite3_exec(connectionHandle, stmt.c_str(), callback, 0, &errMsg);
		if (rc != SQLITE_OK)
		{
			// error occured
			std::cout << "Error: " << errMsg << std::endl;
		}
	}
	std::cout << "created Wish Table\n";
}

void SQLDatabase::executeCommand(std::string command)
{
	/*
	sqlite3_stmt* stmt;

	int ret = sqlite3_prepare_v2(connectionHandle, command.c_str(), -1, &stmt, NULL);
	if (ret != SQLITE_OK) 
	{
		std::cout << "error: " << sqlite3_errmsg(connectionHandle) << std::endl;
		return;
	}

	while ((ret = sqlite3_step(stmt)) == SQLITE_ROW) {
		int id = sqlite3_column_int(stmt, 0);
		const char* name = sqlite3_column_text(stmt, 1);
		// ...
	}
	if (ret != SQLITE_DONE) {
		print("error: ", sqlite3_errmsg(db));
	}
	sqlite3_finalize(stmt);
	*/
}

//int main()
//{
//	std::cout << "test\n";
//	std::unique_ptr<Database> db = std::make_unique<SQLDatabase>();
//
//	wishEntry wish("typ", "nazwa", "10.10.22 15:15", 4);
//	db->insertWish("wishCharacter", wish);
//
//	wishEntry wish1("typ1", "nazwa1", "10.10.22 15:16", 5);
//	wishEntry wish2("typ2", "nazwa2", "10.10.22 15:17", 5);
//	wishEntry wish3("typ3", "nazwa3", "10.10.22 15:18", 5);
//	std::vector<wishEntry> vect = { wish1, wish2, wish3 };
//	db->insertWishes("wishCharacter", vect);
//
//	std::vector<wishEntry> takietam;
//	db->getWishes("wishCharacter", takietam);
//
//	for (auto &item : takietam)
//	{
//		std::cout << item.itemName << std::endl;
//	}
//
//}