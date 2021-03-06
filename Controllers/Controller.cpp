#include "Controller.hpp"
#include "jwt/jwt.hpp"
#include "Task.hpp"
#include "sc.pb.h"

namespace RestServer
{

//TODO: this should be put in some Utils file
template <class Container>
void split(const std::string &str, Container &cont, char delim = ' ')
{
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim))
    {
        cont.push_back(token);
    }
}

void Controller::login(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response)
{
        /*std::cout << "resource: " << request.resource() << std::endl;
        std::cout << "query: " << request.query().as_str() << std::endl;
        std::cout << "body: " << request.body() << std::endl;
        std::cout << "Method : " << request.method() << std::endl;
        std::cout << "------------" << std::endl;*/
       
        Guard guard(controllerLock);
        nlohmann::json logData;
        std::string user;
        std::string password;
        try
        {
          logData = nlohmann::json::parse(request.body());
          user = logData["userId"].get<std::string>();
          password = logData["password"].get<std::string>();
        }
        catch(const std::exception& e)
        {
            response.send(Pistache::Http::Code::Forbidden, "Broken data");
            std::cerr << e.what() << std::endl;
        }
   
        std::string dbpassword;
        bool result = dbmanager->getUserData(user, dbpassword);

        if(dbpassword == password && result)
        {
         nlohmann::json returnData;
         returnData["token"] = authorization->getToken(user);
         returnData["refreshToken"] = authorization->getRefreshToken(user);
         response.send(Pistache::Http::Code::Ok, returnData.dump());
        }
        else
        {
            response.send(Pistache::Http::Code::Forbidden, "Fok jo");
        }
        
}
void Controller::protectedRoute(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response)
{
        Guard guard(controllerLock);

        std::string userId;
        std::string token;
        bool result = getTokenFromHeader(request, token);
        result &= authorization->isAuthorized(token, userId);

        if (result)
        {
            response.send(Pistache::Http::Code::Ok, "Witam "+userId);
        }
        else
        {
            response.send(Pistache::Http::Code::Forbidden, "unauthorized");
        }
}

void Controller::helloWorld(const Pistache::Rest::Request &request, Pistache::Http::ResponseWriter response)
{
        Guard guard(controllerLock);

        std::string userId;
        std::string token;
        bool result = getTokenFromHeader(request, token);
        result &= authorization->isAuthorized(token, userId);

        Service::Task helloTask;
        Service::TaskResult taskResult; 
        helloTask.set_request_type(Service::RequestType::HELLOWORLD);
        auto requestData = helloTask.mutable_hello_request_data();
        requestData->set_user_id(userId);
              
        if (result)
        {
          taskResult = serviceManager->handleRequest(helloTask);
        }

        if (result && taskResult.has_confirm())
        {
            response.send(Pistache::Http::Code::Ok, taskResult.confirm().hello_response().response());
        }
        else
        {
            response.send(Pistache::Http::Code::Forbidden, "unauthorized");
        }
}

bool Controller::getTokenFromHeader(const Pistache::Rest::Request &request, std::string& token)
{
        
        std::cout << "-> Authorization" << std::endl;
        std::shared_ptr<const Pistache::Http::Header::Authorization> authHeader;

        try
        {
            authHeader = request.headers().get<Pistache::Http::Header::Authorization>();
            std::cout << "-> Authorization header found"<< std::endl;
        }
        catch (const std::exception &e)
        {
            std::cout << "Something wrong with header" << std::endl;
            return false;
        }
        

        if (authHeader->value().empty())
        {
            std::cout << "->  Couldn't get authorization header. " << std::endl;
            return false;
        }

        std::vector<std::string> authorizationData;
        split(authHeader->value(), authorizationData);
        token = authorizationData.at(1);
        return true;
}

}