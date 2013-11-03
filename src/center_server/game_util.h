#ifndef __GAME_UTIL_H__
#define __GAME_UTIL_H__

#include <common.h>
#include <regex>

class GameUtil
    : public Venus::Singleton<GameUtil>
{
public:
    //通过邮箱帐号转换为唯一ID
    uint64 toUniqueId(const std::string& email)
    {
        size_t len = email.size();
        unsigned long h = (unsigned long)len;
        size_t step = (len >> 5) + 1;
        for (size_t i = len; i >= step; i -= step)
            h = h ^ ((h << 5) + (h >> 2) + (unsigned long)email.at(i - 1));
        return h;
    }

    //验证邮箱帐号合法性
    bool checkEmailValid(const std::string& email)
    {
        std::regex regex("^[a-zA-Z0-9.!#$%&'*+/=?^_`{|}~-]+@[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?(?:\\.[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)*$");
        return std::regex_match(email, regex);
    }

    //验证码密码合法性（是否32位MD5）
    bool checkPasswordHashValid(const std::string& passwordHash)
    {
        std::regex regex("[a-fA-F0-9]{32,32}");
        return std::regex_match(passwordHash, regex);
    }

};

#endif