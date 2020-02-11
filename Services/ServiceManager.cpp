#include "ServiceManager.hpp"

namespace RestService
{
    
void ServiceManager::registerService(std::unique_ptr<ServiceIf> service)
{
    services.push_back(std::move(service));
}

Service::TaskResult ServiceManager::handleRequest(const Service::Task &task)
{
    Service::TaskResult result ;
    for(auto& service : services)
    {
       result = service->handleRequest(task);
       if(result.has_confirm())
       {
           return result;
       }
    }
    result.mutable_reject()->set_error_code(0); //whatever it means
}
} // namespace RestService