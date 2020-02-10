#include "Task.hpp"
namespace RestService
{
    class ServiceManagerIf
    {
        public:
        virtual TaskResult handleRequest(const Task& task) = 0;
        virtual void registerService(std::unique_ptr<ServiceIf> service) = 0;
        virtual ~ServiceManagerIf();
    };
}