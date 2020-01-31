#pragma once
#include <string>

namespace RestServer
{

class DBManagerIf
{
    public:
    virtual ~DBManagerIf() = default;
    virtual bool getUserData(const std::string& userId, std::string& userData) = 0;
    virtual bool createUser(const std::string& userName, const std::string& userPassword) = 0;
};
}