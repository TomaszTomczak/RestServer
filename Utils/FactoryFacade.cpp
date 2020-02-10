#include "FactoryFacade.hpp"

RestService::ServiceFactoryIf* FactoryFacade::serviceFactory = 0;

void FactoryFacade::setServiceFactory(RestService::ServiceFactoryIf* factory)
{
    serviceFactory = factory;
}

RestService::ServiceFactoryIf* FactoryFacade::getServiceFactory()
{
    return serviceFactory;
}