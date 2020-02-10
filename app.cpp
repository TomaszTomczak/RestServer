#include "Authorization.hpp"
#include "Controller.hpp"
#include "FakeDB.hpp"
#include "RestEndpoint.hpp"
#include "ServiceManager.hpp"

#include <memory>

#include <pistache/http.h>

int main(int argc, char *argv[])
{
    auto key = "secret"; //Secret to use for the algorithm
   
    Pistache::Port port(9080);

    int thr = 2;

    if (argc >= 2)
    {
        port = std::stol(argv[1]);

        if (argc == 3)
            thr = std::stol(argv[2]);
    }

    Pistache::Address addr(Pistache::Ipv4::any(), port);

    std::cout << "Cores = " << Pistache::hardware_concurrency() << std::endl;
    std::cout << "Using " << thr << " threads" << std::endl;

    //StatsEndpoint stats(addr, auth);
    std::unique_ptr<RestService::ServiceManagerIf> serviceManager = std::make_unique<RestService::ServiceManager>();
   // serviceManager->registerService(std::move(RestService::Se));

    std::shared_ptr<RestServer::AuthorizationIf> authorization = std::make_shared<RestServer::Authorization>(key, key);
    std::shared_ptr<RestServer::DBManagerIf> dbmanager = std::make_shared<RestServer::FakeDB>();
    std::shared_ptr<RestServer::ControllerIf> controller = std::make_shared<RestServer::Controller>(authorization, 
                                                                                                    dbmanager);
    std::unique_ptr<RestServer::RestEndpoint> endpoint = std::make_unique<RestServer::RestEndpoint>(addr, controller);


    endpoint->init(thr);
    endpoint->start();
}