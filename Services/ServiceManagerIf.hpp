#pragma once
#include "Task.hpp"
#include "ServiceIf.hpp"
#include <memory>
namespace RestService
{
    class ServiceManagerIf
    {
        public:
        virtual TaskResult handleRequest(const Task& task) = 0;
        virtual void registerService(std::unique_ptr<ServiceIf> service) = 0;
        virtual ~ServiceManagerIf() = default;
    };
}