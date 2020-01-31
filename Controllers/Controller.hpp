#pragma once
#include "ControllerIf.hpp"
#include "AuthorizationIf.hpp"
#include "DBManagerIf.hpp"
//#include "ServiceHandlerIf.hpp"

namespace RestServer
{
class Controller : public ControllerIf
{
    public:
    Controller() = delete;//: authorizeRequests(false)
    // {
    // implement with DBManager
    // }
    Controller(std::shared_ptr<AuthorizationIf> a_authorization, std::shared_ptr<DBManagerIf> dbmanager): authorizeRequests(true), 
    authorization(a_authorization), dbmanager(dbmanager){}
    void login(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) override;
    void protectedRoute(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) override;

    private: 
    bool getTokenFromHeader(const Pistache::Rest::Request &request, std::string& token);

    private:
    typedef std::mutex Lock;
    typedef std::lock_guard<Lock> Guard;
    Lock controllerLock;
    bool authorizeRequests;
    std::shared_ptr<AuthorizationIf> authorization;
    std::shared_ptr<DBManagerIf> dbmanager;

};
}