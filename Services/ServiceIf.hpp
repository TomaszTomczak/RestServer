#pragma once
#include "Task.hpp"
#include "sc.pb.h"

namespace RestService
{
    class ServiceIf
    {
        public:
        virtual Service::TaskResult handleRequest(const Service::Task& task) = 0;
        virtual ~ServiceIf() = default;
    };
}