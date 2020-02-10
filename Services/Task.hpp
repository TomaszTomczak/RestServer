
#pragma once
#include <string>

namespace RestService
{

enum class RequestType // It should be included in google protocol buffer
{
    HELLOWORLD,
    SOMEOTHERREQUEST,
    PROTECTEDROUTE
};

struct Task // protobuf
{
   RequestType request;
};
struct TaskResult
{
    bool result;
    std::string someData; 
};
}