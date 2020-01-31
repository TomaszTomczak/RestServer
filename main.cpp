#include <algorithm>

#include <pistache/http.h>
#include <pistache/router.h>
#include <pistache/endpoint.h>
#include "jwt/jwt.hpp"
#include <memory>
#include "Authorization.hpp"

using namespace std;
using namespace Pistache;

#define SECRET_KEY "d0d41f1a3cc3f67dcd74694de9fef1b0"
#define REFRESH_TOKEN_SECRET_KEY "d47db5c4c9cdf99f599a5b0a0d3f1e6e"
void printCookies(const Http::Request &req)
{
    auto cookies = req.cookies();
    std::cout << "Cookies: [" << std::endl;
    const std::string indent(4, ' ');
    for (const auto &c : cookies)
    {
        std::cout << indent << c.name << " = " << c.value << std::endl;
    }
    std::cout << "]" << std::endl;
}

namespace Generic
{

void handleReady(const Rest::Request &, Http::ResponseWriter response)
{
    response.send(Http::Code::Ok, "1");
}

} // namespace Generic

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
class StatsEndpoint
{
public:
    StatsEndpoint(Address addr, std::shared_ptr<RestServer::AuthorizationIf> auth)
        : httpEndpoint(std::make_shared<Http::Endpoint>(addr)), authorization(auth)
    {
    }

    void init(size_t thr = 2)
    {
        auto opts = Http::Endpoint::options()
                        .threads(thr);
        httpEndpoint->init(opts);
        setupRoutes();
    }

    void start()
    {
        httpEndpoint->setHandler(router.handler());
        httpEndpoint->serve();
    }

private:
    void setupRoutes()
    {
        using namespace Rest;

        Routes::Get(router, "/test/*", Routes::bind(&StatsEndpoint::testPath, this));
        Routes::Post(router, "/login", Routes::bind(&StatsEndpoint::login, this));
        Routes::Post(router, "/record/:name/:value?", Routes::bind(&StatsEndpoint::doRecordMetric, this));
        Routes::Get(router, "/value/:name", Routes::bind(&StatsEndpoint::doGetMetric, this));
        Routes::Get(router, "/ready", Routes::bind(&Generic::handleReady));
        Routes::Get(router, "/auth", Routes::bind(&StatsEndpoint::doAuth, this));
        Routes::Get(router, "/protected", Routes::bind(&StatsEndpoint::protectedPath, this));
        
    }
    std::string isAuth(const Rest::Request &request)
    {
        std::cout << "-> Authorization" << std::endl;
        std::string userId;
        std::shared_ptr<const Http::Header::Authorization> authHeader;

        try
        {
            authHeader = request.headers().get<Http::Header::Authorization>();
            std::cout << "-> Authorization header found"<< std::endl;
        }
        catch (const std::exception &e)
        {
            std::cout << "Something wrong with header" << std::endl;
            return userId;
        }
        

        if (authHeader->value().empty())
        {
            std::cout << "->  Couldn't get authorization header. " << std::endl;
            return userId;
        }

        std::vector<std::string> authorizationData;
        split(authHeader->value(), authorizationData);
        std::string authToken = authorizationData.at(1);

        authorization->isAuthorized(authToken, userId);
       
        return userId;
    }
     void testPath(const Rest::Request &request, Http::ResponseWriter response)
    {
        response.send(Http::Code::Ok, "testpath");
    }
    void protectedPath(const Rest::Request &request, Http::ResponseWriter response)
    {
        std::string userId = isAuth(request);
        if (!userId.empty())
        {
            response.send(Http::Code::Ok, "Witam");
        }
        else
        {
            response.send(Http::Code::Forbidden, "unauthorized");
        }
    }
    void login(const Rest::Request &request, Http::ResponseWriter response)
    {
        Guard guard(metricsLock);
        //////
        std::cout << "resource: " << request.resource() << std::endl;
        std::cout << "query: " << request.query().as_str() << std::endl;
        std::cout << "body: " << request.body() << std::endl;
        std::cout << "Method : " << request.method() << std::endl;
        std::cout << "------------" << std::endl;
        /////
        //  auto user = request.param(":user").as<std::string>();
        // auto passwrod = request.param(":password").as<std::string>();
        auto logData = nlohmann::json::parse(request.body());

        std::string user = logData["userId"].get<std::string>();
        std::string password = logData["password"].get<std::string>();

        if (user == "tomek" && password == "test")
        {
            
            nlohmann::json returnData;
            returnData["token"] = authorization->getToken(user);
            returnData["refreshToken"] = authorization->getRefreshToken(user);

            response.send(Http::Code::Ok, returnData.dump());
        }
        else
        {
            response.send(Http::Code::Forbidden, "Fok jo");
        }
    }
    void doRecordMetric(const Rest::Request &request, Http::ResponseWriter response)
    {
        auto name = request.param(":name").as<std::string>();

        Guard guard(metricsLock);
        auto it = std::find_if(metrics.begin(), metrics.end(), [&](const Metric &metric) {
            return metric.name() == name;
        });

        int val = 1;
        if (request.hasParam(":value"))
        {
            auto value = request.param(":value");
            val = value.as<int>();
        }

        if (it == std::end(metrics))
        {
            metrics.push_back(Metric(std::move(name), val));
            response.send(Http::Code::Created, std::to_string(val));
        }
        else
        {
            auto &metric = *it;
            metric.incr(val);
            response.send(Http::Code::Ok, std::to_string(metric.value()));
        }
    }

    void doGetMetric(const Rest::Request &request, Http::ResponseWriter response)
    {
        auto name = request.param(":name").as<std::string>();

        Guard guard(metricsLock);
        auto it = std::find_if(metrics.begin(), metrics.end(), [&](const Metric &metric) {
            return metric.name() == name;
        });

        if (it == std::end(metrics))
        {
            response.send(Http::Code::Not_Found, "Metric does not exist");
        }
        else
        {
            const auto &metric = *it;
            response.send(Http::Code::Ok, std::to_string(metric.value()));
        }
    }

    void doAuth(const Rest::Request &request, Http::ResponseWriter response)
    {
        printCookies(request);
        response.cookies()
            .add(Http::Cookie("lang", "en-US"));
        response.send(Http::Code::Ok);
    }

    class Metric
    {
    public:
        Metric(std::string name, int initialValue = 1)
            : name_(std::move(name)), value_(initialValue)
        {
        }

        int incr(int n = 1)
        {
            int old = value_;
            value_ += n;
            return old;
        }

        int value() const
        {
            return value_;
        }

        std::string name() const
        {
            return name_;
        }

    private:
        std::string name_;
        int value_;
    };

    typedef std::mutex Lock;
    typedef std::lock_guard<Lock> Guard;
    Lock metricsLock;
    std::vector<Metric> metrics;

    std::shared_ptr<Http::Endpoint> httpEndpoint;
    std::shared_ptr<RestServer::AuthorizationIf> authorization;
    Rest::Router router;
};

int main(int argc, char *argv[])
{
    auto key = "secret"; //Secret to use for the algorithm
    std::shared_ptr<RestServer::Authorization> auth = std::make_shared<RestServer::Authorization>(SECRET_KEY, REFRESH_TOKEN_SECRET_KEY);
    //Create JWT object
    jwt::jwt_object obj{jwt::params::algorithm("HS256"), jwt::params::payload({{"some", "payload"}}), jwt::params::secret(key)};
    auto enc_str = obj.signature();
    std::cout << enc_str << std::endl;

    //Decode
    std::cout << "encode" << std::endl;
    auto dec_obj = jwt::decode(enc_str, jwt::params::algorithms({"HS256"}), jwt::params::secret(key));
    std::cout << dec_obj.header() << std::endl;
    std::cout << dec_obj.payload() << std::endl;

    Port port(9080);

    int thr = 2;

    if (argc >= 2)
    {
        port = std::stol(argv[1]);

        if (argc == 3)
            thr = std::stol(argv[2]);
    }

    Address addr(Ipv4::any(), port);

    cout << "Cores = " << hardware_concurrency() << endl;
    cout << "Using " << thr << " threads" << endl;

    StatsEndpoint stats(addr, auth);

    stats.init(thr);
    stats.start();
}