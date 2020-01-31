#pragma once
#include "DBManagerIf.hpp"
#include <map>

namespace RestServer
{
class FakeDB : public DBManagerIf
{
    public:
    FakeDB()    
    {
        database["tomek"] = "test"; // user and password
        database["admin"] = "dupa.7";
    }
    bool getUserData(const std::string& userId, std::string& userData) override;
    bool createUser(const std::string& userId, const std::string& userPassword) override;
    private:
    std::map<std::string,std::string> database; 
};
}