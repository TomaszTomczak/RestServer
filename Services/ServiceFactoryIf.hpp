
#include "ServiceIf.hpp"
#include <memory>

namespace RestService
{
    class ServiceFactoryIf
    {
        public:
        virtual std::unique_ptr<ServiceIf> createService(const std::string& serviceType) = 0;
        virtual void registerService(std::unique_ptr<ServiceIf> service, std::string serviceName) = 0;
        virtual ~ServiceFactoryIf();
        
    };
}