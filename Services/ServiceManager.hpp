#include "ServiceManagerIf.hpp"
#include "ServiceIf.hpp"
#include <list>

namespace RestService
{
    class ServiceManager : public ServiceManagerIf
    {
        public:
        void registerService(std::unique_ptr<ServiceIf> service) override;
        TaskResult handleRequest(const Task& task) override;

        private:
        std::list<std::unique_ptr<ServiceIf>> services; 

    };
}