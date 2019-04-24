#include "stdafx.h"
#include "SQLConnection.h"
#include "sql_connection_error.h"
#include <sqlite3.h>
using namespace std;

wstring SystemTimeToString(const SYSTEMTIME& st);

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

	if (!checkTableExistence(L"DISK_RECORD")) {
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

bool SQLConnection::checkTableExistence(wstring name)
{
	if (connection == nullptr) {
		throw sql_connection_error(10001, "FileGrabber SQL Error: Invalid connection");
	}
	wstring strFindTable = L"SELECT COUNT(*) FROM sqlite_master where type ='table' and name ='" + name + L"'";
	
	int rc;
	sqlite3_stmt* stmt;
	rc = sqlite3_prepare16_v2(connection, strFindTable.c_str(), static_cast<int>(strFindTable.length() * 2), &stmt, nullptr);
	if (rc != SQLITE_OK) {
		throw sql_connection_error(sqlite3_errcode(connection), sqlite3_errmsg(connection));
	}
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_ROW) {
		sqlite3_finalize(stmt);
		throw sql_connection_error(sqlite3_errcode(connection), sqlite3_errmsg(connection));
	}
	int row = sqlite3_column_int(stmt, 0);
	sqlite3_finalize(stmt);
	return row > 0;
}

void SQLConnection::insertRecord(Device device)
{
	if (connection == nullptr) {
		throw sql_connection_error(10001, "FileGrabber SQL Error: Invalid connection");
	}

	Device::DiskInformation info = device.GetDiskInformation();
	wstring sql = wstring(L"INSERT INTO DISK_RECORD (Label,SerialNumber,FileSystem,FileSystemFlags,DriveLetter,TotalSpace,FreeSpace,FreeSpaceToCaller,RecordTime) ") +
		L"VALUES(\'" + info.Label + L"\'," + to_wstring(info.VolumeSerialNumber) + L",\'" + info.FileSystem + L"\'," + to_wstring(info.FileSystemFlags) + L"," +
		to_wstring(static_cast<int>(info.DriveLetter)) + L",\'" + to_wstring(info.TotalSpace) + L"\',\'" + to_wstring(info.FreeSpace) + L"\',\'"
		+ to_wstring(info.FreeSpaceToCaller) + L"\',\'" + SystemTimeToString(device.GetDiskArriveTime()) + L"\');";

	int rc;
	sqlite3_stmt* stmt;
	rc = sqlite3_prepare16_v2(connection, sql.c_str(), static_cast<int>(sql.length() * 2), &stmt, nullptr);
	if (rc != SQLITE_OK) {
		throw sql_connection_error(sqlite3_errcode(connection), sqlite3_errmsg(connection));
	}
	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (rc != SQLITE_DONE) {
		throw sql_connection_error(sqlite3_errcode(connection), sqlite3_errmsg(connection));
	}
}

void SQLConnection::createRecordTable()
{
	if (connection == nullptr)
		throw sql_connection_error(10001, "FileGrabber SQL Error: Invalid connection");
	wstring strCreateTable = L"CREATE TABLE DISK_RECORD(\
		ID INTEGER PRIMARY KEY AUTOINCREMENT,\
		Label             CHAR(20) NOT NULL,\
		SerialNumber      INT      NOT NULL,\
		FileSystem        CHAR(15) NOT NULL,\
		FileSystemFlags   INT      NOT NULL,\
		DriveLetter       INT      NOT NULL,\
		TotalSpace        CHAR(40) NOT NULL,\
		FreeSpace         CHAR(40) NOT NULL,\
		FreeSpaceToCaller CHAR(40) NOT NULL,\
		RecordTime        CHAR(22) NOT NULL);";
	int rc;
	sqlite3_stmt* stmt;
	rc = sqlite3_prepare16_v2(connection, strCreateTable.c_str(), static_cast<int>(strCreateTable.length() * 2), &stmt, nullptr);
	if (rc != SQLITE_OK) {
		throw sql_connection_error(sqlite3_errcode(connection), sqlite3_errmsg(connection));
	}
	rc = sqlite3_step(stmt);
	sqlite3_finalize(stmt);
	if (rc != SQLITE_DONE) {
		throw sql_connection_error(sqlite3_errcode(connection), sqlite3_errmsg(connection));
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
