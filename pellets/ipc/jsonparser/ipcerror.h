#ifndef ZPUBLIC_JSON_PARSER_IPC_ERROR
#define ZPUBLIC_JSON_PARSER_IPC_ERROR

namespace zl
{
namespace Ipc
{
    /*
    @ EnumName	: enumReturnValule
    @ Brief		: ����ֵ���͡�С��0��ʾʧ�ܣ����ڵ���0��ʾ�ɹ�
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