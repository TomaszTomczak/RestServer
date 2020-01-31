#include "Task.hpp"

namespace RestService
{
    class ServiceIf
    {
        public:
        TaskResult handleRequest(const Task& task);
    };
}