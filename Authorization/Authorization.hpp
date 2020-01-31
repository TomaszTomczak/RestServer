#pragma once
#include "AuthorizationIf.hpp"

namespace RestServer
{
class Authorization final : public AuthorizationIf
{
public:

    Authorization(const std::string &secret, const std::string &secretRefresh) : secret(secret), secretRefresh(secretRefresh) {}

    std::string getToken(const std::string &userId) override;
    std::string getRefreshToken(const std::string &userId) override;
    bool isAuthorized(const std::string &token, std::string &userId) override;
    ~Authorization() = default;
    Authorization() = delete;
    
private:
    std::string secret;
    std::string secretRefresh;
};
} // namespace RestServer