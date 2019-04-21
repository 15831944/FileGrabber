#pragma once

struct sqlite3;

class SQLConnection
{
public:
	SQLConnection(std::wstring filename);
	void open();
	void close();
	bool isTableExist(std::string name);
	~SQLConnection();
protected:
	std::wstring filename;
	sqlite3* connection = nullptr;
	void createRecordTable();
};

