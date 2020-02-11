#include "HelloWorldService.hpp"

namespace RestService
{

Service::TaskResult HelloWorldService::handleRequest(const Service::Task& task)
{
    Service::TaskResult returnData;
    if(task.request_type() == Service::RequestType::HELLOWORLD)
    {
        std::string userId = task.hello_request_data().user_id();
        auto helloRetData = returnData.mutable_confirm();
        helloRetData->mutable_hello_response()->set_response("Hello world "+userId);
    }
    else
    {
        returnData.mutable_reject()->set_error_code(5); //5 - whatever it means
    }
    return returnData;    

}

}