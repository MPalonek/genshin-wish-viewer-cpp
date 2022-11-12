#include "utilityTest.h"
#include <filesystem>

void dbTest::getTables(sqlite3* cHandle, std::unordered_set<std::string> &tables)
{
	sqlite3_stmt* stmt;
	const char* sql = "SELECT * FROM sqlite_master WHERE type='table';";
	int rc = sqlite3_prepare_v2(cHandle, sql, -1, &stmt, NULL);

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		const char* p = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
		if (p == NULL)
			p = "";
		tables.insert(std::string(p));
	}
	sqlite3_finalize(stmt);
}

int dbTest::getVersion(sqlite3* cHandle)
{
	sqlite3_stmt* stmt;
	const char* sql = "SELECT version FROM systemInfo";
	int rc = sqlite3_prepare_v2(cHandle, sql, -1, &stmt, NULL);
	int version = 0;

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		int id = sqlite3_column_int(stmt, 0);
		if (id > version)
			version = id;
	}
	sqlite3_finalize(stmt);
	return version;
}

void dbTest::removeDbFiles()
{
	std::vector<std::string> dbNames = { g_filename, "customName.db" };
	for (auto& db : dbNames)
	{
		auto path = std::filesystem::path(db);
		if (std::filesystem::exists(path))
		{
			std::filesystem::remove(path);
		}
		else
		{
			// Doesn't exist
		}
	}
}
