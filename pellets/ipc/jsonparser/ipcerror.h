#ifndef ZPUBLIC_JSON_PARSER_IPC_ERROR
#define ZPUBLIC_JSON_PARSER_IPC_ERROR

namespace zl
{
namespace Ipc
{
    /*
    @ EnumName	: enumReturnValule
    @ Brief		: 返回值类型。小于0表示失败，大于等于0表示成功
    */
    enum enumReturnValule
    {
        enumRet_Succeed      = 0,
        enumRet_Error        = -1,
        enumRet_FuncNotFound = -2,
        enumRet_BeFilted     = -3,
    };
}
}

#endif