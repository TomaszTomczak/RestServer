
#include "ServiceIf.hpp"
#include <memory>

namespace RestService
{
    class ServiceFactoryIf
    {
        public:
        virtual std::unique_ptr<ServiceIf> createService() = 0;
    };
}