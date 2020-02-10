#include "HelloWorldService.hpp"

namespace RestService
{

TaskResult HelloWorldService::handleRequest(const Task& task)
{
    TaskResult returnData;
    if(task.request == RequestType::HELLOWORLD)
    {
        returnData.result = true;
        returnData.someData = "HelloWorld";
    }
    else
    {
        returnData.result = false;
    }
    return returnData;    

}

}