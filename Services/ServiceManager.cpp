#include "ServiceManager.hpp"
namespace RestService
{
    
void ServiceManager::registerService(std::unique_ptr<ServiceIf> service)
{
    services.push_back(std::move(service));
}

TaskResult ServiceManager::handleRequest(const Task &task)
{
    for(auto& service : services)
    {
       TaskResult tskres = service->handleRequest(task);
       if(tskres.result)
       {
           return tskres;
       }

    }
}
} // namespace RestService