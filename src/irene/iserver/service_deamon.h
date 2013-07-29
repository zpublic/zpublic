#ifndef SERVICE_DEAMON_H_
#define SERVICE_DEAMON_H_

template <typename ServiceType>
class ServiceDeamon
{
protected:
    virtual void start(const std::string&, 
        const uint32_t&) = 0;
    virtual void stop() = 0;

protected:
    ServiceType* _service;
};

#endif