#pragma once
#include <string>

namespace RestServer
{
    class AuthorizationIf
    {
        public:
            virtual std::string getToken(const std::string& userId) = 0;
            virtual std::string getRefreshToken(const std::string& userId) = 0;
            virtual bool isAuthorized(const std::string& token, std::string& userId) = 0;
            virtual ~AuthorizationIf() = default;
    };
}