#include "ControllerIf.hpp"
#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include <memory>

namespace RestServer
{
class RestEndpoint
{
    public:
    RestEndpoint(Pistache::Address addr, std::shared_ptr<ControllerIf> controller): httpEndpoint(std::make_shared<Pistache::Http::Endpoint>(addr)), 
    controller(controller){}
    void init(size_t thr);
    void start();

    private: 
    void setupRoutes();    

    std::shared_ptr<ControllerIf> controller; 
    std::shared_ptr<Pistache::Http::Endpoint> httpEndpoint;    
    Pistache::Rest::Router router;
};
}