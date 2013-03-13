#ifndef ZPUBLIC_JSON_PARSER_IPC_PUBLIC
#define ZPUBLIC_JSON_PARSER_IPC_PUBLIC

#include <string>
#include <vector>
#include <list>
#include <map>

#include "ipcerror.h"
#include "ipcexception.h"

namespace zl
{
namespace Ipc
{

    #define defJsonInstID		"id"
    #define defJsonClassName	"class_name"
    #define defJsonOperation	"operation"
    #define defJsonFuncName		"func_name"
    #define defJsonParamArray	"param_array"

    #define defJsonOperateRun	"run"
    #define defJsonOperateAlloc	"alloc"
    #define defJsonOperateDealloc "dealloc"

    #define defJsonRetCode		"code"
    #define defJsonRetMessage	"message"
    #define defJsonRetValue		"value"

    #define defJsonRetParam1    "rp1"
    #define defJsonRetParam2    "rp2"

    template <class T>
    struct Type2Type 
    {
        typedef T Type;
    };

    template <>
    struct Type2Type <void>
    {
        typedef void Type;
    };

    #define DISNABLE_CONSTRUCT_AND_DECONSTRUCT(class_name) \
        protected: \
        class_name(){} \
        class_name(class_name&){} \
        ~class_name(){}

    #define DISNABLE_ASSIGN_OPERATION(class_name) \
        protected: \
        class_name& operator = (class_name&){}

    #define THROW_EXCEPTION throw new ipcException(ipcLocation(HERE_LOCATION))

}
}

#endif