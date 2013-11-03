#ifndef __SERVICE_H__
#define __SERVICE_H__

#include <string>

namespace Venus
{
    class Service
    {
    protected:
        Service(){}
        virtual ~Service(){}

    protected:
        virtual bool initialize() = 0;
        virtual void destroy() = 0;
    };
}

#endif // !__SERVICE_H__
