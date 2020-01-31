#include "FakeDB.hpp"

namespace RestServer
{
bool FakeDB::getUserData(const std::string& userId, std::string& userData)
{
    auto search = database.find(userId);
    if(search != database.end())
    {
        userData = search->second;
        return true;
    }
    else
    {
        return false;
    }
    
}

bool FakeDB::createUser(const std::string& userId, const std::string& userPassword)
{
    auto search = database.find(userId);
    if(search == database.end())
    {
        database[userId] = userPassword;
        return true;
    }
    else
    {
        return false;
    }
}
}
