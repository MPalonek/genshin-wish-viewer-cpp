#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include "sqlite3.h"
#include <unordered_set>
#include <vector>

constexpr char g_filename[] = "data.db";
constexpr unsigned int g_version = 1;

struct wishEntry
{
	std::string itemType;
	std::string itemName;
	std::string date;
	unsigned int itemRarity;

	wishEntry(std::string t, std::string n, std::string d, unsigned int r) : itemType(t), itemName(n), date(d), itemRarity(r) {}
};

class Database
{
public:
	virtual ~Database() {};
	virtual void getWishes(std::string tableName, std::vector<wishEntry> &wishList) = 0;
	virtual void insertWish(std::string tableName, wishEntry wish) = 0;
	virtual void insertWishes(std::string tableName, std::vector<wishEntry> wishVec) = 0;
};

class SQLDatabase : public Database
{
public:
	SQLDatabase(std::string dbF = g_filename);
	~SQLDatabase() override;
	void getWishes(std::string tableName, std::vector<wishEntry> &wishList) override;
	void insertWish(std::string tableName, wishEntry wish) override;
	void insertWishes(std::string tableName, std::vector<wishEntry> wishVec) override;
private:
	void init();
	void setupDB();
	void getTables(std::unordered_set<std::string> &tables);
	int infoTableGetLatestVersion();
	void createTables();
	void createInfoTable();
	void createWishTable();

	void executeCommand(std::string command);

	sqlite3* connectionHandle;
	std::string dbFilename;
};

class PostgreDatabase : public Database
{
public:
	PostgreDatabase() {};
	~PostgreDatabase() override {};
	void getWishes(std::string tableName, std::vector<wishEntry> &wishList) override {};
	void insertWish(std::string tableName, wishEntry wish) override {};
	void insertWishes(std::string tableName, std::vector<wishEntry> wishVec) override {};
};

#endif /* DATABASE_H */