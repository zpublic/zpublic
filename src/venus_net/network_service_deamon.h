#ifndef __NETWORK_SREVICE_DEAMON_H__
#define __NETWORK_SREVICE_DEAMON_H__

#include "common.h"
#include "io_service.h"
#include "service_deamon.h"

namespace Venus
{
    class NetworkServiceDeamon
        : public Venus::ServiceDeamon<IOService>
    {
    protected:
        NetworkServiceDeamon(const std::string& service_name, IOService& io_service)
            : _service_name(service_name), ServiceDeamon(io_service)
        {
        }
        virtual ~NetworkServiceDeamon()
        {
        }

    protected:
        __forceinline const std::string& serviceName() const
        {
            return _service_name;
        }

    private:
        std::string _service_name;
    };
}

#endif