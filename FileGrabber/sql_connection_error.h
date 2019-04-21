#pragma once
#include <exception>
#include <stdexcept>

class sql_connection_error : public std::runtime_error
{
public:
	explicit sql_connection_error(int errcode, std::string message) : std::runtime_error(message) {
		this->errcode = errcode;
	}
	inline int getErrorCode() const { return errcode; }
protected:
	int errcode;
};

