#pragma once
#include <pistache/http.h>
#include <pistache/router.h>
namespace RestServer
{
class ControllerIf
{
public:
    virtual ~ControllerIf() = default;
    virtual void login(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) = 0;
    virtual void protectedRoute(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response) = 0;
};
} 