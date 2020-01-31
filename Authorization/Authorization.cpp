#include "Authorization.hpp"
#include "jwt/jwt.hpp"
#include <iostream>

namespace RestServer 
{
    std::string Authorization::getToken(const std::string &userId)
    {
        jwt::jwt_object token{jwt::params::algorithm("HS256"), jwt::params::payload({{"userId", userId}}), jwt::params::secret(secret)};
        token.add_claim("exp", std::chrono::system_clock::now() + std::chrono::minutes{10}); // test
        return token.signature();
            
    }
    std::string Authorization::getRefreshToken(const std::string &userId)
    {
        jwt::jwt_object refreshToken{jwt::params::algorithm("HS256"), jwt::params::payload({{"userId", userId}}), jwt::params::secret(secretRefresh)};
        refreshToken.add_claim("exp", std::chrono::system_clock::now() + std::chrono::hours{24});
        return refreshToken.signature();
    }
    bool Authorization::isAuthorized(const std::string &token, std::string &userId)
    {
        jwt::jwt_object data;        
        try
        {            
            data = jwt::decode(token, jwt::params::algorithms({"HS256"}), jwt::params::secret(secret));            
        }
        catch (const std::exception &e)
        {
            
            std::cout << "Could not verify request: " << e.what() << std::endl;
        }
        auto payloadData = data.payload();
        if (payloadData.has_claim("userId"))
        {
            userId = data.payload().get_claim_value<std::string>("userId");
            std::cout << "obtained userId " << userId << std::endl;
            return true;
        }
        else
        {
            std::cout<<"# User not found"<<std::endl;
        }

        return false;
    }
}