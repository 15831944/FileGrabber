#include "stdafx.h"
#include "SQLConnection.h"
#include "sql_connection_error.h"
#include <sqlite3.h>
using namespace std;

SQLConnection::SQLConnection(wstring filename)
{
	this->filename = filename;
}

void SQLConnection::open()
{
	if (connection != nullptr)
		return;
	int rc;
	rc = sqlite3_open16(filename.c_str(), &connection);
	if (rc != SQLITE_OK) {
		throw sql_connection_error(sqlite3_errcode(connection), sqlite3_errmsg(connection));
	}

	if (!isTableExist("DISK_RECORD")) {
		createRecordTable();
	}
}

void SQLConnection::close()
{
	if (connection == nullptr)
		return;
	int rc;
	rc = sqlite3_close(connection);
	if (rc != SQLITE_OK) {
		throw sql_connection_error(sqlite3_errcode(connection), sqlite3_errmsg(connection));
	}
	connection = nullptr;
}

bool SQLConnection::isTableExist(string name)
{
	if (connection == nullptr)
		throw sql_connection_error(10001, "FileGrabber SQL Error: Invalid connection");
	string strFindTable = "SELECT COUNT(*) FROM sqlite_master where type ='table' and name ='" + name + "'";
	char* sErrMsg = nullptr;
	int nTableNums = 0;

	auto callback = [](void* pHandle, int iRet, char** szSrc, char** szDst) -> int {
		if (1 == iRet) {
			int iTableExist = atoi(*(szSrc));
			if (pHandle != nullptr) {
				int* pRes = (int*)pHandle;
				*pRes = iTableExist;
			}
		}
		return 0;
	};

	if (sqlite3_exec(connection, strFindTable.c_str(), callback, &nTableNums, &sErrMsg) != SQLITE_OK) {
		return false;
	}
	return nTableNums > 0;
}

void SQLConnection::createRecordTable()
{
	if (connection == nullptr)
		throw sql_connection_error(10001, "FileGrabber SQL Error: Invalid connection");
	string strCreateTable = "CREATE TABLE DISK_RECORD("  \
		"ID INT PRIMARY    KEY      NOT NULL," \
		"Label             TEXT     NOT NULL," \
		"SerialNumber      INT      NOT NULL," \
		"FileSystem        CHAR(50) NOT NULL," \
		"FileSystemFlags   INT      NOT NULL," \
		"DriveLetter       INT      NOT NULL," \
		"TotalSpace        CHAR(20) NOT NULL," \
		"FreeSpace         CHAR(20) NOT NULL," \
		"FreeSpaceToCaller CHAR(20) NOT NULL);";
	char* errMsg;
	int rc = sqlite3_exec(connection, strCreateTable.c_str(), nullptr, nullptr, &errMsg);
	if (rc != SQLITE_OK) {
		string msg = errMsg;
		free(errMsg);
		throw sql_connection_error(sqlite3_errcode(connection), msg);
	}
}


SQLConnection::~SQLConnection()
{
	try {
		if (connection != nullptr)
			this->close();
	}
	catch (...) {}
}
