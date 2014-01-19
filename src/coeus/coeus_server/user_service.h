#ifndef __USER_SERVICE_H__
#define __USER_SERVICE_H__

#include "common.h"

class UserService
{
public:
    void userLogin(const std::string& username, const std::string& password);
    void userRegister(const std::string& account, const std::string& password);
};

#endif // !__USER_SERVICE_H__
