#pragma once
#include <stdexcept>

class XMLParseError : public std::runtime_error
{
public:
	XMLParseError(const std::string& message) : std::runtime_error(message) {}
	XMLParseError(const char* message) : std::runtime_error(message) {}
};

