#include "ServiceIf.hpp"

namespace RestService
{
    class HelloWorldService : public ServiceIf
    {
        public:
        TaskResult handleRequest(const Task& task) override;
    };
}