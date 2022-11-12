#ifndef DATABASE_UTILITY_TEST_H
#define DATABASE_UTILITY_TEST_H

#include "../src/db.h"
#include "../src/sqlite3.h"
#include <unordered_set>

namespace dbTest
{
	void getTables(sqlite3* cHandle, std::unordered_set<std::string> &tables);
	int getVersion(sqlite3* cHandle);
	void removeDbFiles();
}

#endif /* DATABASE_UTILITY_TEST_H */