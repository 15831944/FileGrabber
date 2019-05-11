#pragma once
#include "Device.h"
struct sqlite3;

class SQLConnection
{
public:
	SQLConnection(std::wstring filename);
	void open();
	void close();
	bool checkTableExistence(const std::wstring name);
	void insertRecord(Device device);
	~SQLConnection();
protected:
	std::wstring filename;
	sqlite3* connection = nullptr;
	void createRecordTable();
	void initDatabase();
	void versionCheck();
	int sqlite3_step16(const std::wstring& sql) const;
	friend void InitProgram();
};

