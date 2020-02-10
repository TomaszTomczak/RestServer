#include "Task.hpp"

namespace RestService
{
    class ServiceIf
    {
        public:
        virtual TaskResult handleRequest(const Task& task) = 0;
        virtual ~ServiceIf();
    };
}