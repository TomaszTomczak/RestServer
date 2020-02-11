#pragma once
#include "Task.hpp"
#include "ServiceIf.hpp"
#include "sc.pb.h"
#include <memory>
namespace RestService
{
    class ServiceManagerIf
    {
        public:
        virtual Service::TaskResult handleRequest(const Service::Task& task) = 0;
        virtual void registerService(std::unique_ptr<ServiceIf> service) = 0;
        virtual ~ServiceManagerIf() = default;
    };
}