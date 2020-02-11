#include "ServiceIf.hpp"

namespace RestService
{
    class HelloWorldService : public ServiceIf
    {
        public:
        Service::TaskResult handleRequest(const Service::Task& task) override;
    };
}