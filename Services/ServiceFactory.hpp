
#include "ServiceFactoryIf.hpp"

namespace RestService
{
    class ServiceFactory: public ServiceFactoryIf
    {
        public:
        std::unique_ptr<ServiceIf> createService(const std::string& serviceType) override;
        void registerService(std::unique_ptr<ServiceIf> service, std::string serviceName) override;
    };
}