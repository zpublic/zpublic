#ifndef __SREVICE_DEAMON_H__
#define __SREVICE_DEAMON_H__

namespace Venus
{
    template <typename IOServiceType>
    class ServiceDeamon
    {
    protected:
        ServiceDeamon(IOServiceType& io_service)
            : _io_service(io_service)
        {
        }
        virtual ~ServiceDeamon()
        {
        }

    protected:
        virtual void start(){}
        virtual void stop(){}

    protected:
        IOServiceType& service()
        {
            return _io_service;
        }

    protected:
        IOServiceType& _io_service;
    };
}

#endif