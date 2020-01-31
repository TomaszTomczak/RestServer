#include "RestEndpoint.hpp"
namespace RestServer
{
void RestEndpoint::init(size_t thr = 2)
{
    auto opts = Pistache::Http::Endpoint::options()
                    .threads(thr);
    httpEndpoint->init(opts);
    setupRoutes();
}

void RestEndpoint::start()
{
    httpEndpoint->setHandler(router.handler());
    httpEndpoint->serve();
}

void RestEndpoint::setupRoutes()
{
    using namespace Pistache::Rest;

    Routes::Post(router, "/login", Routes::bind(&ControllerIf::login, controller.get()));
    Routes::Get(router, "/protected", Routes::bind(&ControllerIf::protectedRoute, controller.get()));
}

} // namespace RestServer