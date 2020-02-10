#include "ServiceFactoryIf.hpp"
#include <memory>

class FactoryFacade
{
    public:
    static void setServiceFactory(RestService::ServiceFactoryIf* factory);
    static RestService::ServiceFactoryIf* getServiceFactory();

    private:
    static RestService::ServiceFactoryIf* serviceFactory;
};