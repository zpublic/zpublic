/*************************************************************************
 *                                                                       *
 *  I|*j^3Cl|a   "+!*%                  qt          Nd   gW              *
 *  l]{y+l?MM*  !#Wla\NNP               NW          MM   I|              *
 *        PW    ?E|    tWg              Wg  sC!     AW           ~@v~    *
 *       NC     ?M!    yN|  WW     MK   MW@K1Y%M@   RM   #Q    QP@tim    *
 *     CM|      |WQCljAE|   MD     Mg   RN     cM~  NM   WQ   MQ         *
 *    #M        aQ?         MW     M3   Mg      Q(  HQ   YR  IM|         *
 *   Dq         {Ql         MH    iMX   Mg     MM   QP   QM   Eg         *
 * !EWNaPRag2$  +M"          $WNaHaN%   MQE$%EXW    QQ   CM    %M%a$D    *
 *                                                                       *
 *                               ZPublic                                 *
 *                  Developer: zapline(278998871@qq.com)                 *
 *               Website: https://github.com/zpublic/zpublic             *
 *                                                                       *
 ************************************************************************/
#pragma once
#include <map>
#include <string>

class z_lua_function_reg
{
public:
    z_lua_function_reg();
    ~z_lua_function_reg();

    ///> 加入和删除新函数，如果函数名重复会覆盖
    void insert_no_prefix(const char *name, lua_CFunction func);
    void insert_no_prefix(const luaL_Reg* pfuncs);
    void insert(const char *name, lua_CFunction func);
    void insert(const luaL_Reg* pfuncs);
    void erase(const char *name);

    ///> 用于设置统一前缀
    void prefix(std::string val) { prefix_ = val; }

    ///> 库名
    std::string libname() const { return libname_; }
    void libname(std::string val) { libname_ = val; }

    ///> 生成luaL_Reg表，需要调用来释放内存
    luaL_Reg* create();
    void close(luaL_Reg* pfuncs);


private:
    std::map<std::string, lua_CFunction> funcs_;
    std::string prefix_;
    std::string libname_;
};
