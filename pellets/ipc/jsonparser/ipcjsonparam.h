#ifndef ZPUBLIC_JSON_PARSER_IPC_PARAM
#define ZPUBLIC_JSON_PARSER_IPC_PARAM

#include "json/json.h"
#include "ipcpublic.h"
#include "ipcjsonConvert.h"

namespace zl
{
namespace Ipc
{

    inline std::string CreateGlobalCall(const char* func_name)
    {
        Json::Value v;
        Json::FastWriter writer;
        v[defJsonOperation] = defJsonOperateRun;
        v[defJsonFuncName] = func_name;
        v[defJsonPID] = (unsigned int)::GetCurrentProcessId();
        return writer.write(v);
    }

    template <class P1>
    inline std::string CreateGlobalCall(const char* func_name, const P1& p1)
    {
        Json::Value v;
        Json::FastWriter writer;
        v[defJsonOperation] = defJsonOperateRun;
        v[defJsonFuncName] = func_name;
        v[defJsonPID] = (unsigned int)::GetCurrentProcessId();
        v[defJsonParamArray].clear();
        v[defJsonParamArray].append(GetJsonValue(p1));
        return writer.write(v);
    }

    template <class P1, class P2>
    inline std::string CreateGlobalCall(const char* func_name, const P1& p1, const P2& p2)
    {
        Json::Value v;
        Json::FastWriter writer;
        v[defJsonOperation] = defJsonOperateRun;
        v[defJsonFuncName] = func_name;
        v[defJsonPID] = (unsigned int)::GetCurrentProcessId();
        v[defJsonParamArray].clear();
        v[defJsonParamArray].append(GetJsonValue(p1));
        v[defJsonParamArray].append(GetJsonValue(p2));
        return writer.write(v);
    }

    template <class P1, class P2, class P3>
    inline std::string CreateGlobalCall(const char* func_name, const P1& p1, const P2& p2, const P3& p3)
    {
        Json::Value v;
        Json::FastWriter writer;
        v[defJsonOperation] = defJsonOperateRun;
        v[defJsonFuncName] = func_name;
        v[defJsonPID] = (unsigned int)::GetCurrentProcessId();
        v[defJsonParamArray].clear();
        v[defJsonParamArray].append(GetJsonValue(p1));
        v[defJsonParamArray].append(GetJsonValue(p2));
        v[defJsonParamArray].append(GetJsonValue(p3));
        return writer.write(v);
    }

    template <class P1, class P2, class P3, class P4>
    inline std::string CreateGlobalCall(const char* func_name, const P1& p1, const P2& p2, const P3& p3, const P4& p4)
    {
        Json::Value v;
        Json::FastWriter writer;
        v[defJsonOperation] = defJsonOperateRun;
        v[defJsonFuncName] = func_name;
        v[defJsonPID] = (unsigned int)::GetCurrentProcessId();
        v[defJsonParamArray].clear();
        v[defJsonParamArray].append(GetJsonValue(p1));
        v[defJsonParamArray].append(GetJsonValue(p2));
        v[defJsonParamArray].append(GetJsonValue(p3));
        v[defJsonParamArray].append(GetJsonValue(p4));
        return writer.write(v);
    }

    template <class P1, class P2, class P3, class P4, class P5>
    inline std::string CreateGlobalCall(const char* func_name, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5)
    {
        Json::Value v;
        Json::FastWriter writer;
        v[defJsonOperation] = defJsonOperateRun;
        v[defJsonFuncName] = func_name;
        v[defJsonPID] = (unsigned int)::GetCurrentProcessId();
        v[defJsonParamArray].clear();
        v[defJsonParamArray].append(GetJsonValue(p1));
        v[defJsonParamArray].append(GetJsonValue(p2));
        v[defJsonParamArray].append(GetJsonValue(p3));
        v[defJsonParamArray].append(GetJsonValue(p4));
        v[defJsonParamArray].append(GetJsonValue(p5));
        return writer.write(v);
    }

    inline std::string CreateClassCall(int id, const char* func_name)
    {
        Json::Value v;
        Json::FastWriter writer;
        v[defJsonOperation] = defJsonOperateRun;
        v[defJsonInstID] = id;
        v[defJsonFuncName] = func_name;
        v[defJsonPID] = (unsigned int)::GetCurrentProcessId();
        return writer.write(v);
    }

    template <class P1>
    inline std::string CreateClassCall(int id, const char* func_name, const P1& p1)
    {
        Json::Value v;
        Json::FastWriter writer;
        v[defJsonOperation] = defJsonOperateRun;
        v[defJsonInstID] = id;
        v[defJsonFuncName] = func_name;
        v[defJsonPID] = (unsigned int)::GetCurrentProcessId();
        v[defJsonParamArray].clear();
        v[defJsonParamArray].append(GetJsonValue(p1));
        return writer.write(v);
    }

    template <class P1, class P2>
    inline std::string CreateClassCall(int id, const char* func_name, const P1& p1, const P2& p2)
    {
        Json::Value v;
        Json::FastWriter writer;
        v[defJsonOperation] = defJsonOperateRun;
        v[defJsonInstID] = id;
        v[defJsonFuncName] = func_name;
        v[defJsonPID] = (unsigned int)::GetCurrentProcessId();
        v[defJsonParamArray].clear();
        v[defJsonParamArray].append(GetJsonValue(p1));
        v[defJsonParamArray].append(GetJsonValue(p2));
        return writer.write(v);
    }

    template <class P1, class P2, class P3>
    inline std::string CreateClassCall(int id, const char* func_name, const P1& p1, const P2& p2, const P3& p3)
    {
        Json::Value v;
        Json::FastWriter writer;
        v[defJsonOperation] = defJsonOperateRun;
        v[defJsonInstID] = id;
        v[defJsonFuncName] = func_name;
        v[defJsonPID] = (unsigned int)::GetCurrentProcessId();
        v[defJsonParamArray].clear();
        v[defJsonParamArray].append(GetJsonValue(p1));
        v[defJsonParamArray].append(GetJsonValue(p2));
        v[defJsonParamArray].append(GetJsonValue(p3));
        return writer.write(v);
    }

    template <class P1, class P2, class P3, class P4>
    inline std::string CreateClassCall(int id, const char* func_name, const P1& p1, const P2& p2, const P3& p3, const P4& p4)
    {
        Json::Value v;
        Json::FastWriter writer;
        v[defJsonOperation] = defJsonOperateRun;
        v[defJsonInstID] = id;
        v[defJsonFuncName] = func_name;
        v[defJsonPID] = (unsigned int)::GetCurrentProcessId();
        v[defJsonParamArray].clear();
        v[defJsonParamArray].append(GetJsonValue(p1));
        v[defJsonParamArray].append(GetJsonValue(p2));
        v[defJsonParamArray].append(GetJsonValue(p3));
        v[defJsonParamArray].append(GetJsonValue(p4));
        return writer.write(v);
    }

    template <class P1, class P2, class P3, class P4, class P5>
    inline std::string CreateClassCall(int id, const char* func_name, const P1& p1, const P2& p2, const P3& p3, const P4& p4, const P5& p5)
    {
        Json::Value v;
        Json::FastWriter writer;
        v[defJsonOperation] = defJsonOperateRun;
        v[defJsonInstID] = id;
        v[defJsonFuncName] = func_name;
        v[defJsonPID] = (unsigned int)::GetCurrentProcessId();
        v[defJsonParamArray].clear();
        v[defJsonParamArray].append(GetJsonValue(p1));
        v[defJsonParamArray].append(GetJsonValue(p2));
        v[defJsonParamArray].append(GetJsonValue(p3));
        v[defJsonParamArray].append(GetJsonValue(p4));
        v[defJsonParamArray].append(GetJsonValue(p5));
        return writer.write(v);
    }

    inline std::string CreateObjectCreateCall(const char* class_name)
    {
        Json::Value v;
        Json::FastWriter writer;
        v[defJsonOperation] = defJsonOperateAlloc;
        v[defJsonClassName] = class_name;
        v[defJsonPID] = (unsigned int)::GetCurrentProcessId();
        return writer.write(v);
    }

    inline std::string CreateObjectDeleteCall(int inst_id)
    {
        Json::Value v;
        Json::FastWriter writer;
        v[defJsonOperation] = defJsonOperateDealloc;
        v[defJsonInstID] = inst_id;
        v[defJsonPID] = (unsigned int)::GetCurrentProcessId();		
        return writer.write(v);
    }

}
}

#endif