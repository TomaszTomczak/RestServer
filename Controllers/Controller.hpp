#pragma once
#include "ControllerIf.hpp"
#include "AuthorizationIf.hpp"
#include "DBManagerIf.hpp"
//#include "ServiceFactoryIf.hpp"
#include "ServiceManagerIf.hpp"

namespace RestServer
{
class Controller : public ControllerIf
{
    public:
    Controller() = delete;//: authorizeRequests(false)
    // {
    // implement with DBManager
    // }
    Controller(std::shared_ptr<AuthorizationIf> a_authorization, 
               std::shared_ptr<DBManagerIf> dbmanager, 
               std::unique_ptr<RestService::ServiceManagerIf> serviceManager): 
               authorizeRequests(true), 
               authorization(a_authorization), 
               dbmanager(dbmanager), 
               serviceManager(std::move(serviceManager)){}
               
    void login(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) override;
    void protectedRoute(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) override;
    void helloWorld(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) override;

    private: 
    bool getTokenFromHeader(const Pistache::Rest::Request &request, std::string& token);

    private:
    typedef std::mutex Lock;
    typedef std::lock_guard<Lock> Guard;
    Lock controllerLock;
    bool authorizeRequests;
    std::shared_ptr<AuthorizationIf> authorization;
    std::shared_ptr<DBManagerIf> dbmanager;
    std::unique_ptr<RestService::ServiceManagerIf> serviceManager;

};
}