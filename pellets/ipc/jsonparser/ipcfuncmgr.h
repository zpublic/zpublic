#ifndef ZPUBLIC_JSON_PARSER_IPC_FUNCMGR
#define ZPUBLIC_JSON_PARSER_IPC_FUNCMGR

#include "../../../3rdparty/jsonlib/src/json/json.h"
#include "ipcpublic.h"
#include "ipclocker.h"
#include "iipccallfilter.h"

namespace zl
{
namespace Ipc
{
    struct IGlobalFunc
    {
        virtual ~IGlobalFunc(){}
        virtual const std::string& FuncName(void) = 0;
        virtual int ParamCount(void) = 0;
        virtual std::string Run(Json::Value& param_array) = 0;
    };

    template <class Class>
    class IClassFunc
    {
    public:
        virtual ~IClassFunc(){}
        virtual const std::string& FuncName(void) = 0;
        virtual int ParamCount(void) = 0;
        virtual std::string Run(Class* p_inst, Json::Value& param_array) = 0;
    };

    struct IClassFuncSet 
    {
        virtual ~IClassFuncSet(){}
        virtual std::string& ClassName(void) = 0;
    };

    struct IClassObjectInst
    {
        virtual ~IClassObjectInst(){}
        virtual int InstID(void) = 0;
        virtual std::string Run(const char* func_name, Json::Value& param_array) = 0;
    };

    template <class R>
    class ipcGlobalFunc_0 : public IGlobalFunc
    {
    public:
        typedef R (*FuncType)(void);

        ipcGlobalFunc_0(const char* func_name, FuncType func_ptr)
            : func_name_(func_name), func_ptr_(func_ptr){}

        virtual const std::string& FuncName(void){return func_name_;}
        virtual int ParamCount(void){return 0;}
        virtual std::string Run(Json::Value& param_array)
        {
            return RunByType(param_array, Type2Type<R>());
        }
        
    protected:
        template <class T>
        std::string RunByType(Json::Value& param_array, Type2Type<T>)
        {
            R ret_value = func_ptr_();
            return ipcJsonReturn(ret_value).ToString();
        }

        std::string RunByType(Json::Value& param_array, Type2Type<void>)
        {
            func_ptr_();
            return ipcJsonReturn().ToString();
        }

    private:
        std::string func_name_;
        FuncType func_ptr_;
    };

    template <class R, class P1>
    class ipcGlobalFunc_1 : public IGlobalFunc
    {
    public:
        typedef R (*FuncType)(P1);

        ipcGlobalFunc_1(const char* func_name, FuncType func_ptr)
            : func_name_(func_name), func_ptr_(func_ptr){}

        virtual const std::string& FuncName(void){return func_name_;}
        virtual int ParamCount(void){return 1;}
        virtual std::string Run(Json::Value& param_array)
        {
            return RunByType(param_array, Type2Type<R>());
        }

    protected:
        template <class T>
        std::string RunByType(Json::Value& param_array, Type2Type<T>)
        {
            int n_param_pos_0 = 0;
            Json2Value<P1> p1(param_array[n_param_pos_0]);
            R ret_value = func_ptr_(p1.Value());
            return ipcJsonReturn(ret_value).ToString();
        }

        std::string RunByType(Json::Value& param_array, Type2Type<void>)
        {
            int n_param_pos_0 = 0;
            Json2Value<P1> p1(param_array[n_param_pos_0]);
            func_ptr_(p1.Value());
            return ipcJsonReturn().ToString();
        }

    private:
        std::string func_name_;
        FuncType func_ptr_;
    };

    template <class R, class P1, class P2>
    class ipcGlobalFunc_2 : public IGlobalFunc
    {
    public:
        typedef R (*FuncType)(P1, P2);

        ipcGlobalFunc_2(const char* func_name, FuncType func_ptr)
            : func_name_(func_name), func_ptr_(func_ptr){}

        virtual const std::string& FuncName(void){return func_name_;}
        virtual int ParamCount(void){return 2;}
        virtual std::string Run(Json::Value& param_array)
        {
            return RunByType(param_array, Type2Type<R>());
        }
        
    protected:
        template <class T>
        std::string RunByType(Json::Value& param_array, Type2Type<T>)
        {
            int n_param_pos_0 = 0;
            Json2Value<P1> p1(param_array[n_param_pos_0]);
            Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
            R ret_value = func_ptr_(p1.Value(), p2.Value());
            return ipcJsonReturn(ret_value).ToString();
        }

        std::string RunByType(Json::Value& param_array, Type2Type<void>)
        {
            int n_param_pos_0 = 0;
            Json2Value<P1> p1(param_array[n_param_pos_0]);
            Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
            func_ptr_(p1.Value(), p2.Value());
            return ipcJsonReturn().ToString();
        }

    private:
        std::string func_name_;
        FuncType func_ptr_;
    };

    template <class R, class P1, class P2, class P3>
    class ipcGlobalFunc_3 : public IGlobalFunc
    {
    public:
        typedef R (*FuncType)(P1, P2, P3);

        ipcGlobalFunc_3(const char* func_name, FuncType func_ptr)
            : func_name_(func_name), func_ptr_(func_ptr){}

        virtual const std::string& FuncName(void){return func_name_;}
        virtual int ParamCount(void){return 3;}
        virtual std::string Run(Json::Value& param_array)
        {
            return RunByType(param_array, Type2Type<R>());
        }

    protected:
        template <class T>
        std::string RunByType(Json::Value& param_array, Type2Type<T>)
        {
            int n_param_pos_0 = 0;
            Json2Value<P1> p1(param_array[n_param_pos_0]);
            Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
            Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
            R ret_value = func_ptr_(p1.Value(), p2.Value(), p3.Value());
            return ipcJsonReturn(ret_value).ToString();
        }

        std::string RunByType(Json::Value& param_array, Type2Type<void>)
        {
            int n_param_pos_0 = 0;
            Json2Value<P1> p1(param_array[n_param_pos_0]);
            Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
            Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
            func_ptr_(p1.Value(), p2.Value(), p3.Value());
            return ipcJsonReturn().ToString();
        }

    private:
        std::string func_name_;
        FuncType func_ptr_;
    };

    template <class R, class P1, class P2, class P3, class P4>
    class ipcGlobalFunc_4 : public IGlobalFunc
    {
    public:
        typedef R (*FuncType)(P1, P2, P3, P4);

        ipcGlobalFunc_4(const char* func_name, FuncType func_ptr)
            : func_name_(func_name), func_ptr_(func_ptr){}

        virtual const std::string& FuncName(void){return func_name_;}
        virtual int ParamCount(void){return 4;}
        virtual std::string Run(Json::Value& param_array)
        {
            return RunByType(param_array, Type2Type<R>());
        }

    protected:
        template <class T>
        std::string RunByType(Json::Value& param_array, Type2Type<T>)
        {
            int n_param_pos_0 = 0;
            Json2Value<P1> p1(param_array[n_param_pos_0]);
            Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
            Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
            Json2Value<P4> p4(param_array[n_param_pos_0 + 3]);
            R ret_value = func_ptr_(p1.Value(), p2.Value(), p3.Value(), p4.Value());
            return ipcJsonReturn(ret_value).ToString();
        }

        std::string RunByType(Json::Value& param_array, Type2Type<void>)
        {
            int n_param_pos_0 = 0;
            Json2Value<P1> p1(param_array[n_param_pos_0]);
            Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
            Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
            Json2Value<P4> p4(param_array[n_param_pos_0 + 3]);
            func_ptr_(p1.Value(), p2.Value(), p3.Value(), p4.Value());
            return ipcJsonReturn().ToString();
        }

    private:
        std::string func_name_;
        FuncType func_ptr_;
    };

    template <class R, class P1, class P2, class P3, class P4, class P5>
    class ipcGlobalFunc_5 : public IGlobalFunc
    {
    public:
        typedef R (*FuncType)(P1, P2, P3, P4, P5);

        ipcGlobalFunc_5(const char* func_name, FuncType func_ptr)
            : func_name_(func_name), func_ptr_(func_ptr){}

        virtual const std::string& FuncName(void){return func_name_;}
        virtual int ParamCount(void){return 5;}
        virtual std::string Run(Json::Value& param_array)
        {
            return RunByType(param_array, Type2Type<R>());
        }

    protected:
        template <class T>
        std::string RunByType(Json::Value& param_array, Type2Type<T>)
        {
            int n_param_pos_0 = 0;
            Json2Value<P1> p1(param_array[n_param_pos_0]);
            Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
            Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
            Json2Value<P4> p4(param_array[n_param_pos_0 + 3]);
            Json2Value<P5> p5(param_array[n_param_pos_0 + 4]);
            R ret_value = func_ptr_(p1.Value(), p2.Value(), p3.Value(), p4.Value(), p5.Valule());
            return ipcJsonReturn(ret_value).ToString();
        }

        std::string RunByType(Json::Value& param_array, Type2Type<void>)
        {
            int n_param_pos_0 = 0;
            Json2Value<P1> p1(param_array[n_param_pos_0]);
            Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
            Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
            Json2Value<P4> p4(param_array[n_param_pos_0 + 3]);
            Json2Value<P5> p5(param_array[n_param_pos_0 + 4]);
            func_ptr_(p1.Value(), p2.Value(), p3.Value(), p4.Value(), p5.Value());
            return ipcJsonReturn().ToString();
        }

    private:
        std::string func_name_;
        FuncType func_ptr_;
    };    

    template <class R, class RP1>
    class ipcGlobalFunc_1_1 : public IGlobalFunc
    {
    public:
        typedef R (*FuncType)(RP1&);

        ipcGlobalFunc_1_1(const char* func_name, FuncType func_ptr)
            : func_name_(func_name), func_ptr_(func_ptr){}

        virtual const std::string& FuncName(void){return func_name_;}
        virtual int ParamCount(void){return 1;}
        virtual std::string Run(Json::Value& param_array)
        {
            return RunByType(param_array, Type2Type<R>());
        }

    protected:
        template <class T>
        std::string RunByType(Json::Value& param_array, Type2Type<T>)
        {
            int n_param_pos_0 = 0;
            RP1 rp1;
            R ret_value = func_ptr_(rp1);
            return ipcJsonReturn(ret_value, rp1).ToString();
        }

        std::string RunByType(Json::Value& param_array, Type2Type<void>)
        {
            int n_param_pos_0 = 0;
            RP1 rp1;
            func_ptr_(rp1);
            return ipcJsonReturn(0, rp1).ToString();
        }

    private:
        std::string func_name_;
        FuncType func_ptr_;
    };

    template <class R, class RP1, class P2>
    class ipcGlobalFunc_2_1 : public IGlobalFunc
    {
    public:
        typedef R (*FuncType)(RP1&, P2);

        ipcGlobalFunc_2_1(const char* func_name, FuncType func_ptr)
            : func_name_(func_name), func_ptr_(func_ptr){}

        virtual const std::string& FuncName(void){return func_name_;}
        virtual int ParamCount(void){return 2;}
        virtual std::string Run(Json::Value& param_array)
        {
            return RunByType(param_array, Type2Type<R>());
        }

    protected:
        template <class T>
        std::string RunByType(Json::Value& param_array, Type2Type<T>)
        {
            int n_param_pos_0 = 0;
            Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
            RP1 rp1;
            R ret_value = func_ptr_(rp1, p2.Value());
            return ipcJsonReturn(ret_value, rp1).ToString();
        }

        std::string RunByType(Json::Value& param_array, Type2Type<void>)
        {
            int n_param_pos_0 = 0;
            Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
            RP1 rp1;
            func_ptr_(rp1, p2.Value());
            return ipcJsonReturn(0, rp1).ToString();
        }

    private:
        std::string func_name_;
        FuncType func_ptr_;
    };

    template <class R, class RP1, class P2, class P3>
    class ipcGlobalFunc_3_1 : public IGlobalFunc
    {
    public:
        typedef R (*FuncType)(RP1&, P2, P3);

        ipcGlobalFunc_3_1(const char* func_name, FuncType func_ptr)
            : func_name_(func_name), func_ptr_(func_ptr){}

        virtual const std::string& FuncName(void){return func_name_;}
        virtual int ParamCount(void){return 3;}
        virtual std::string Run(Json::Value& param_array)
        {
            return RunByType(param_array, Type2Type<R>());
        }

    protected:
        template <class T>
        std::string RunByType(Json::Value& param_array, Type2Type<T>)
        {
            int n_param_pos_0 = 0;
            Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
            Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
            RP1 rp1;
            R ret_value = func_ptr_(rp1, p2.Value(), p3.Value());
            return ipcJsonReturn(ret_value, rp1).ToString();
        }

        std::string RunByType(Json::Value& param_array, Type2Type<void>)
        {
            int n_param_pos_0 = 0;
            Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
            Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
            RP1 rp1;
            func_ptr_(rp1, p2.Value(), p3.Value());
            return ipcJsonReturn(0, rp1).ToString();
        }

    private:
        std::string func_name_;
        FuncType func_ptr_;
    };

    template <class R, class RP1, class RP2>
    class ipcGlobalFunc_2_2 : public IGlobalFunc
    {
    public:
        typedef R (*FuncType)(RP1&, RP2);

        ipcGlobalFunc_2_2(const char* func_name, FuncType func_ptr)
            : func_name_(func_name), func_ptr_(func_ptr){}

        virtual const std::string& FuncName(void){return func_name_;}
        virtual int ParamCount(void){return 2;}
        virtual std::string Run(Json::Value& param_array)
        {
            return RunByType(param_array, Type2Type<R>());
        }

    protected:
        template <class T>
        std::string RunByType(Json::Value& param_array, Type2Type<T>)
        {
            int n_param_pos_0 = 0;
            RP1 rp1;
            RP2 rp2;
            R ret_value = func_ptr_(rp1, rp2);
            return ipcJsonReturn(ret_value, rp1, rp2).ToString();
        }

        std::string RunByType(Json::Value& param_array, Type2Type<void>)
        {
            int n_param_pos_0 = 0;
            RP1 rp1;
            RP2 rp2;
            R ret_value = func_ptr_(rp1, rp2);
            return ipcJsonReturn(0, rp1, rp2).ToString();
        }

    private:
        std::string func_name_;
        FuncType func_ptr_;
    };

    template <class R, class RP1, class RP2, class P3>
    class ipcGlobalFunc_3_2 : public IGlobalFunc
    {
    public:
        typedef R (*FuncType)(RP1&, RP2&, P3);

        ipcGlobalFunc_3_2(const char* func_name, FuncType func_ptr)
            : func_name_(func_name), func_ptr_(func_ptr){}

        virtual const std::string& FuncName(void){return func_name_;}
        virtual int ParamCount(void){return 3;}
        virtual std::string Run(Json::Value& param_array)
        {
            return RunByType(param_array, Type2Type<R>());
        }

    protected:
        template <class T>
        std::string RunByType(Json::Value& param_array, Type2Type<T>)
        {
            int n_param_pos_0 = 0;
            Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
            RP1 rp1;
            RP2 rp2;
            R ret_value = func_ptr_(rp1, rp2, p3.Value());
            return ipcJsonReturn(ret_value, rp1, rp2).ToString();
        }

        std::string RunByType(Json::Value& param_array, Type2Type<void>)
        {
            int n_param_pos_0 = 0;
            Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
            RP1 rp1;
            RP2 rp2;
            func_ptr_(rp1, rp2, p3.Value());
            return ipcJsonReturn(0, rp1, rp2).ToString();
        }

    private:
        std::string func_name_;
        FuncType func_ptr_;
    };



    template <class Class, class R>
    class ipcClassFunc_0 : public IClassFunc<Class>
    {
    public:
        typedef R (Class::*FuncType)(void);
        ipcClassFunc_0(const char* func_name, FuncType func_ptr)
            : func_name_(func_name), func_ptr_(func_ptr){}

        virtual const std::string& FuncName(void){return func_name_;}
        virtual int ParamCount(void){return 0;}
        virtual std::string Run(Class* p_inst, Json::Value& param_array)
        {
            return RunByType(p_inst, param_array, Type2Type<R>());
        }

    protected:
        template <class T>
        std::string RunByType(Class* p_inst, Json::Value& param_array, Type2Type<T>)
        {
            R ret_value = (p_inst->*func_ptr_)();
            return ipcJsonReturn(ret_value).ToString();
        }

        std::string RunByType(Class* p_inst, Json::Value& param_array, Type2Type<void>)
        {
            (p_inst->*func_ptr_)();
            return ipcJsonReturn().ToString();
        }

    private:
        std::string func_name_;
        FuncType func_ptr_;
    };

    template <class Class, class R, class P1>
    class ipcClassFunc_1 : public IClassFunc<Class>
    {
    public:
        typedef R (Class::*FuncType)(P1);
        ipcClassFunc_1(const char* func_name, FuncType func_ptr)
            : func_name_(func_name), func_ptr_(func_ptr){}

        virtual const std::string& FuncName(void){return func_name_;}
        virtual int ParamCount(void){return 1;}
        virtual std::string Run(Class* p_inst, Json::Value& param_array)
        {
            return RunByType(p_inst, param_array, Type2Type<R>());
        }

    protected:
        template <class T>
        std::string RunByType(Class* p_inst, Json::Value& param_array, Type2Type<T>)
        {
            int n_param_pos_0 = 0;
            Json2Value<P1> p1(param_array[n_param_pos_0]);
            R ret_value = (p_inst->*func_ptr_)(p1.Value());
            return ipcJsonReturn(ret_value).ToString();
        }

        std::string RunByType(Class* p_inst, Json::Value& param_array, Type2Type<void>)
        {
            int n_param_pos_0 = 0;
            Json2Value<P1> p1(param_array[n_param_pos_0]);
            (p_inst->*func_ptr_)(p1.Value());
            return ipcJsonReturn().ToString();
        }

    private:
        std::string func_name_;
        FuncType func_ptr_;
    };

    template <class Class, class R, class P1, class P2>
    class ipcClassFunc_2 : public IClassFunc<Class>
    {
    public:
        typedef R (Class::*FuncType)(P1, P2);
        ipcClassFunc_2(const char* func_name, FuncType func_ptr)
            : func_name_(func_name), func_ptr_(func_ptr){}

        virtual const std::string& FuncName(void){return func_name_;}
        virtual int ParamCount(void){return 2;}
        virtual std::string Run(Class* p_inst, Json::Value& param_array)
        {
            return RunByType(p_inst, param_array, Type2Type<R>());
        }
        
    protected:
        template <class T>
        std::string RunByType(Class* p_inst, Json::Value& param_array, Type2Type<T>)
        {
            int n_param_pos_0 = 0;
            Json2Value<P1> p1(param_array[n_param_pos_0]);
            Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
            R ret_value = (p_inst->*func_ptr_)(p1.Value(), p2.Value());
            return ipcJsonReturn(ret_value).ToString();
        }

        std::string RunByType(Class* p_inst, Json::Value& param_array, Type2Type<void>)
        {
            int n_param_pos_0 = 0;
            Json2Value<P1> p1(param_array[n_param_pos_0]);
            Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
            (p_inst->*func_ptr_)(p1.Value(), p2.Value());
            return ipcJsonReturn().ToString();
        }

    private:
        std::string func_name_;
        FuncType func_ptr_;
    };

    template <class Class, class R, class P1, class P2, class P3>
    class ipcClassFunc_3 : public IClassFunc<Class>
    {
    public:
        typedef R (Class::*FuncType)(P1, P2, P3);
        ipcClassFunc_3(const char* func_name, FuncType func_ptr)
            : func_name_(func_name), func_ptr_(func_ptr){}

        virtual const std::string& FuncName(void){return func_name_;}
        virtual int ParamCount(void){return 3;}
        virtual std::string Run(Class* p_inst, Json::Value& param_array)
        {
            return RunByType(p_inst, param_array, Type2Type<R>());
        }

    protected:
        template <class T>
        std::string RunByType(Class* p_inst, Json::Value& param_array, Type2Type<T>)
        {
            int n_param_pos_0 = 0;
            Json2Value<P1> p1(param_array[n_param_pos_0]);
            Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
            Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
            R ret_value = (p_inst->*func_ptr_)(p1.Value(), p2.Value(), p3.Value());
            return ipcJsonReturn(ret_value).ToString();
        }

        std::string RunByType(Class* p_inst, Json::Value& param_array, Type2Type<void>)
        {
            int n_param_pos_0 = 0;
            Json2Value<P1> p1(param_array[n_param_pos_0]);
            Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
            Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
            (p_inst->*func_ptr_)(p1.Value(), p2.Value(), p3.Value());
            return ipcJsonReturn().ToString();
        }

    private:
        std::string func_name_;
        FuncType func_ptr_;
    };

    template <class Class, class R, class P1, class P2, class P3, class P4>
    class ipcClassFunc_4 : public IClassFunc<Class>
    {
    public:
        typedef R (Class::*FuncType)(P1, P2, P3, P4);
        ipcClassFunc_4(const char* func_name, FuncType func_ptr)
            : func_name_(func_name), func_ptr_(func_ptr){}

        virtual const std::string& FuncName(void){return func_name_;}
        virtual int ParamCount(void){return 4;}
        virtual std::string Run(Class* p_inst, Json::Value& param_array)
        {
            return RunByType(p_inst, param_array, Type2Type<R>());
        }

    protected:
        template <class T>
        std::string RunByType(Class* p_inst, Json::Value& param_array, Type2Type<T>)
        {
            int n_param_pos_0 = 0;
            Json2Value<P1> p1(param_array[n_param_pos_0]);
            Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
            Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
            Json2Value<P4> p4(param_array[n_param_pos_0 + 3]);
            R ret_value = (p_inst->*func_ptr_)(p1.Value(), p2.Value(), p3.Value(), p4.Value());
            return ipcJsonReturn(ret_value).ToString();
        }

        std::string RunByType(Class* p_inst, Json::Value& param_array, Type2Type<void>)
        {
            int n_param_pos_0 = 0;
            Json2Value<P1> p1(param_array[n_param_pos_0]);
            Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
            Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
            Json2Value<P4> p4(param_array[n_param_pos_0 + 3]);
            (p_inst->*func_ptr_)(p1.Value(), p2.Value(), p3.Value(), p4.Value());
            return ipcJsonReturn().ToString();
        }

    private:
        std::string func_name_;
        FuncType func_ptr_;
    };

    template <class Class, class R, class P1, class P2, class P3, class P4, class P5>
    class ipcClassFunc_5 : public IClassFunc<Class>
    {
    public:
        typedef R (Class::*FuncType)(P1, P2, P3, P4, P5);
        ipcClassFunc_5(const char* func_name, FuncType func_ptr)
            : func_name_(func_name), func_ptr_(func_ptr){}

        virtual const std::string& FuncName(void){return func_name_;}
        virtual int ParamCount(void){return 5;}
        virtual std::string Run(Class* p_inst, Json::Value& param_array)
        {
            return RunByType(p_inst, param_array, Type2Type<R>());
        }

    protected:
        template <class T>
        std::string RunByType(Class* p_inst, Json::Value& param_array, Type2Type<T>)
        {
            int n_param_pos_0 = 0;
            Json2Value<P1> p1(param_array[n_param_pos_0]);
            Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
            Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
            Json2Value<P4> p4(param_array[n_param_pos_0 + 3]);
            Json2Value<P5> p5(param_array[n_param_pos_0 + 4]);
            R ret_value = (p_inst->*func_ptr_)(p1.Value(), p2.Value(), p3.Value(), p4.Value(), p5.Value());
            return ipcJsonReturn(ret_value).ToString();
        }

        std::string RunByType(Class* p_inst, Json::Value& param_array, Type2Type<void>)
        {
            int n_param_pos_0 = 0;
            Json2Value<P1> p1(param_array[n_param_pos_0]);
            Json2Value<P2> p2(param_array[n_param_pos_0 + 1]);
            Json2Value<P3> p3(param_array[n_param_pos_0 + 2]);
            Json2Value<P4> p4(param_array[n_param_pos_0 + 3]);
            Json2Value<P5> p5(param_array[n_param_pos_0 + 4]);
            (p_inst->*func_ptr_)(p1.Value(), p2.Value(), p3.Value(), p4.Value(), p5.Value());
            return ipcJsonReturn().ToString();
        }

    private:
        std::string func_name_;
        FuncType func_ptr_;
    };

    class ipcGlobalFuncSet
    {
    public:
        ~ipcGlobalFuncSet(void){this->Clear();}

        IGlobalFunc* GetGlobalFunc(const char* func_name, int param_count)
        {
            for (size_t i = 0; i < vec_global_func_.size(); ++i)
            {
                if (vec_global_func_[i]->FuncName() == func_name &&
                    vec_global_func_[i]->ParamCount() == param_count)
                {
                    return vec_global_func_[i];
                }
            }
            return NULL;
        }

        void Add(IGlobalFunc* p_global_func){vec_global_func_.push_back(p_global_func);}
        void Clear(void){for(size_t i = 0; i < vec_global_func_.size(); ++i) delete vec_global_func_[i]; vec_global_func_.clear();}

    private:
        std::vector<IGlobalFunc*> vec_global_func_;
    };

    template <class Class>
    class ipcClassFuncSet : public IClassFuncSet
    {
    public:
        ipcClassFuncSet(const char* class_name) : class_name_(class_name){}
        ~ipcClassFuncSet(void){this->Clear();}

        IClassFunc<Class>* GetClassFunc(const char* func_name, int param_count)
        {
            for (size_t i = 0; i < vec_class_func_.size(); ++i)
            {
                if (vec_class_func_[i]->FuncName() == func_name &&
                    vec_class_func_[i]->ParamCount() == param_count)
                {
                    return vec_class_func_[i];
                }
            }
            return NULL;
        }

        virtual std::string& ClassName(void){return class_name_;}
        void Add(IClassFunc<Class>* p_class_func){vec_class_func_.push_back(p_class_func);}
        void Clear(void){for(size_t i = 0; i < vec_class_func_.size(); ++i) delete vec_class_func_[i]; vec_class_func_.clear();}

    private:
        std::string class_name_;
        std::vector<IClassFunc<Class>*> vec_class_func_;
    };

    class ipcAllClassFuncSets
    {
    public:
        ~ipcAllClassFuncSets(void){this->Clear();}

        IClassFuncSet* GetClassFuncSet(const char* class_name)
        {
            for (size_t i = 0; i < vec_class_func_set_.size(); ++i)
            {
                if (vec_class_func_set_[i]->ClassName() == class_name)
                {
                    return vec_class_func_set_[i];
                }
            }
            return NULL;
        }

        void Add(IClassFuncSet* p_func_set){vec_class_func_set_.push_back(p_func_set);}
        void Clear(void){for(size_t i = 0; i < vec_class_func_set_.size(); ++i) delete vec_class_func_set_[i]; vec_class_func_set_.clear();}

    private:
        std::vector<IClassFuncSet*> vec_class_func_set_;
    };

    template <class Class>
    class ipcClassObjectInst : public IClassObjectInst
    {
    public:
        typedef Class TheClass;
        ipcClassObjectInst(int inst_id, Class* p_inst) : inst_id_(inst_id), p_class_object_inst_(p_inst), p_class_func_set_(NULL){}
        virtual ~ipcClassObjectInst(){ delete p_class_object_inst_; }

        virtual int InstID(){return inst_id_;}
        virtual std::string Run(const char* func_name, Json::Value& param_array)
        {
            if (!p_class_func_set_) return ipcJsonReturn(enumRet_FuncNotFound).ToString();
            IClassFunc<Class>* p_class_func = p_class_func_set_->GetClassFunc(func_name, param_array.size());

            if (p_class_func) return p_class_func->Run(p_class_object_inst_, param_array);
            else return ipcJsonReturn(enumRet_FuncNotFound).ToString();		
        }

        void SetClassFuncSet(ipcClassFuncSet<Class>* p_func_set){p_class_func_set_ = p_func_set;}

    private:
        int inst_id_;
        Class*	p_class_object_inst_;
        ipcClassFuncSet<Class>* p_class_func_set_;
    };

    class ipcClassObjectInstSet
    {
    public:
        ~ipcClassObjectInstSet(){ this->Clear(); }
        void Add(IClassObjectInst* p_inst){ipcLocker lock(&session_lock_);vec_class_object_.push_back(p_inst);}
        void Clear(void){for(size_t i = 0; i < vec_class_object_.size(); ++i) delete vec_class_object_[i]; vec_class_object_.clear();}

        void Erase(int inst_id)
        {
            ipcLocker lock(&session_lock_);
            std::vector<IClassObjectInst*>::iterator iter;
            for (iter = vec_class_object_.begin(); iter != vec_class_object_.end(); ++iter)
            {	
                if ((*iter)->InstID() == inst_id)
                {
                    IClassObjectInst* pTempInst = *iter;
                    vec_class_object_.erase(iter);
                    delete pTempInst;
                    break;
                }
            }
        }
        IClassObjectInst* GetClassObjectInst(int inst_id)
        {
            ipcLocker lock(&session_lock_);
            for (size_t i = 0; i < vec_class_object_.size(); ++i)
            {
                if (vec_class_object_[i]->InstID() == inst_id)
                {
                    return vec_class_object_[i];
                }
            }
            return NULL;
        }

    private:
        ipcCriticalSesion	session_lock_;
        std::vector<IClassObjectInst*> vec_class_object_;
    };

    class ipcInterfaceObjectFactory
    {
    public:
        static ipcInterfaceObjectFactory& Inst()
        {
            static ipcInterfaceObjectFactory _interface_object_factory;
            return _interface_object_factory;
        }

        typedef int(*PFNCreateObject)(void);
        void Add(const char* class_name, PFNCreateObject create_func)
        {
            object_create_func_map_.insert(std::make_pair(std::string(class_name), create_func));
        }

        int Create(const char* class_name)
        {
            std::map<std::string, PFNCreateObject>::iterator iter;
            iter = object_create_func_map_.find(class_name);
            if (iter != object_create_func_map_.end()) return iter->second();
            else return enumRet_FuncNotFound;
        }

    protected:
        DISNABLE_CONSTRUCT_AND_DECONSTRUCT(ipcInterfaceObjectFactory);
        DISNABLE_ASSIGN_OPERATION(ipcInterfaceObjectFactory);

    private:
        std::map<std::string, PFNCreateObject> object_create_func_map_;
    };

    inline int __CreateObjectByClassName(const char* class_name)
    {
        return ipcInterfaceObjectFactory::Inst().Create(class_name);
    }

    class ipcRemoteRunManager
    {
    public:
        static ipcRemoteRunManager& Inst(void){static ipcRemoteRunManager s_manager; return s_manager;}

        void Add(IGlobalFunc* p_global_func){global_func_set_.Add(p_global_func);}

        template <class Class>
        void Add(const char* class_name, IClassFunc<Class>* p_class_func)
        {
            IClassFuncSet* pSet = class_func_sets_.GetClassFuncSet(class_name);
            if (pSet)
            {
                ipcClassFuncSet<Class>* pTempSet = (ipcClassFuncSet<Class>*)pSet;
                pTempSet->Add(p_class_func);
            }
            else
            {
                ipcClassFuncSet<Class>* pTempSet = new ipcClassFuncSet<Class>(class_name);
                pTempSet->Add(p_class_func);
                class_func_sets_.Add(pTempSet);
            }
        }
        void Add(IClassFuncSet* p_func_set){class_func_sets_.Add(p_func_set);}
        void Add(IClassObjectInst* p_inst){class_object_inst_set_.Add(p_inst);}

        std::string Run(const char* json_string)
        {
            Json::Value value;
            std::string str_operation = GetOperation(json_string, value);
            
            if (str_operation == defJsonOperateRun)
            {
                return RunFunc(value);
            }
            else if (str_operation == defJsonOperateAlloc)
            {
                return AllocObject(value);
            }
            else if (str_operation == defJsonOperateDealloc)
            {
                return DeallocObject(value);
            }

            return ipcJsonReturn(enumRet_Error, "unknown operation").ToString();
        }

        std::string GetOperation(const char* json_string, Json::Value& value)
        {
            Json::Reader reader;

            if (!reader.parse(json_string, value))
            {
                return "";
            }

            Json::Value json_operation = value[defJsonOperation];
            if (!json_operation.isString())
            {
                return "";
            }

            return json_operation.asCString();
        }

        std::string AllocObject(Json::Value& value)
        {
            try
            {
                Json::Value json_class_name = value[defJsonClassName];
                if (json_class_name.isString())
                {
                    int inst_id = __CreateObjectByClassName(json_class_name.asCString());
                    return ipcJsonReturn(inst_id).ToString();
                }
                return ipcJsonReturn(enumRet_Error).ToString();
            }
            catch (ipcException* e) {
                return ipcJsonReturn(enumRet_Error, e->what()).ToString();
            }
            catch (std::exception* e)
            {
                return ipcJsonReturn(enumRet_Error, e->what()).ToString();
            }
            catch (...) {
                return ipcJsonReturn(enumRet_Error).ToString();
            }
        }

        std::string DeallocObject(Json::Value& value)
        {
            try
            {
                Json::Value json_inst_id = value[defJsonInstID];
                if (json_inst_id.isInt())
                {
                    int inst_id = json_inst_id.asInt();
                    class_object_inst_set_.Erase(inst_id);
                    return ipcJsonReturn(enumRet_Succeed).ToString();
                }
                return ipcJsonReturn(enumRet_Error).ToString();
            }
            catch (ipcException* e) {
                return ipcJsonReturn(enumRet_Error, e->what()).ToString();
            }
            catch (std::exception* e)
            {
                return ipcJsonReturn(enumRet_Error, e->what()).ToString();
            }
            catch (...) {
                return ipcJsonReturn(enumRet_Error).ToString();
            }
        }

        void DeallocObject(int inst_id)
        {
            class_object_inst_set_.Erase(inst_id);
        }

        std::string RunFunc(Json::Value& value)
        {
            try
            {
                if (m_pICallFilter && m_pICallFilter->IsNeedFilte(value))
                {
                    return ipcJsonReturn(enumRet_BeFilted).ToString();
                }
                Json::Value json_inst_id = value[defJsonInstID];
                Json::Value json_func_name = value[defJsonFuncName];
                Json::Value json_param_array = value[defJsonParamArray];

                if (json_func_name.isNull()) return ipcJsonReturn(enumRet_FuncNotFound).ToString();

                if (json_inst_id.isNull())
                {
                    return RunGlobalFunc(json_func_name.asCString(), json_param_array);
                }
                else
                {
                    int inst_id = json_inst_id.asInt();
                    IClassObjectInst* p_inst = class_object_inst_set_.GetClassObjectInst(inst_id);
                    if (p_inst)
                    {
                        return p_inst->Run(json_func_name.asCString(), json_param_array);
                    }
                    else return ipcJsonReturn(enumRet_FuncNotFound).ToString();
                }
            }
            catch (ipcException* e) {
                return ipcJsonReturn(enumRet_Error, e->what()).ToString();
            }
            catch (std::exception* e)
            {
                return ipcJsonReturn(enumRet_Error, e->what()).ToString();
            }
            catch (...) {
                return ipcJsonReturn(enumRet_Error).ToString();
            }
        }

        IClassFuncSet* GetClassFuncSet(const char* class_name)
        {
            return class_func_sets_.GetClassFuncSet(class_name);
        }
        
        void SetFilter(IIpcCallFilter* pFilter)
        {
            if (m_pICallFilter)
            {
                delete m_pICallFilter;
            }
            m_pICallFilter = pFilter;
        }
    protected:
        std::string RunGlobalFunc(const char* func_name, Json::Value& param_array)
        {
            IGlobalFunc* p_global_func = global_func_set_.GetGlobalFunc(func_name, param_array.size());
            if (p_global_func)
            {
                return p_global_func->Run(param_array);
            }
            return ipcJsonReturn(enumRet_FuncNotFound).ToString();
        }

    protected:
        ipcRemoteRunManager() : m_pICallFilter(NULL){}
        ipcRemoteRunManager(ipcRemoteRunManager&){}
        ~ipcRemoteRunManager(){}
        DISNABLE_ASSIGN_OPERATION(ipcRemoteRunManager);

    private:
        ipcGlobalFuncSet	global_func_set_;
        ipcAllClassFuncSets class_func_sets_;
        ipcClassObjectInstSet class_object_inst_set_;
        IIpcCallFilter* m_pICallFilter;
    };

    template <class R>
    struct __ipcAutoAddGlobalFunc_0
    {
        typedef R (*FuncType)(void);
        __ipcAutoAddGlobalFunc_0(const char* func_name, FuncType func_ptr)
        {
            ipcGlobalFunc_0<R>* p_global_func = new ipcGlobalFunc_0<R>(func_name, func_ptr);
            ipcRemoteRunManager::Inst().Add(p_global_func);
        }
    };

    template <class R, class P1>
    struct __ipcAutoAddGlobalFunc_1
    {
        typedef R (*FuncType)(P1);
        __ipcAutoAddGlobalFunc_1(const char* func_name, FuncType func_ptr)
        {
            ipcGlobalFunc_1<R, P1>* p_global_func = new ipcGlobalFunc_1<R, P1>(func_name, func_ptr);
            ipcRemoteRunManager::Inst().Add(p_global_func);
        }
    };

    template <class R, class P1, class P2>
    struct __ipcAutoAddGlobalFunc_2
    {
        typedef R (*FuncType)(P1, P2);
        __ipcAutoAddGlobalFunc_2(const char* func_name, FuncType func_ptr)
        {
            ipcGlobalFunc_2<R, P1, P2>* p_global_func = new ipcGlobalFunc_2<R, P1, P2>(func_name, func_ptr);
            ipcRemoteRunManager::Inst().Add(p_global_func);
        }
    };

    template <class R, class P1, class P2, class P3>
    struct __ipcAutoAddGlobalFunc_3
    {
        typedef R (*FuncType)(P1, P2, P3);
        __ipcAutoAddGlobalFunc_3(const char* func_name, FuncType func_ptr)
        {
            ipcGlobalFunc_3<R, P1, P2, P3>* p_global_func = new ipcGlobalFunc_3<R, P1, P2, P3>(func_name, func_ptr);
            ipcRemoteRunManager::Inst().Add(p_global_func);
        }
    };

    template <class R, class P1, class P2, class P3, class P4>
    struct __ipcAutoAddGlobalFunc_4
    {
        typedef R (*FuncType)(P1, P2, P3, P4);
        __ipcAutoAddGlobalFunc_4(const char* func_name, FuncType func_ptr)
        {
            ipcGlobalFunc_4<R, P1, P2, P3, P4>* p_global_func = new ipcGlobalFunc_4<R, P1, P2, P3, P4>(func_name, func_ptr);
            ipcRemoteRunManager::Inst().Add(p_global_func);
        }
    };

    template <class R, class P1, class P2, class P3, class P4, class P5>
    struct __ipcAutoAddGlobalFunc_5
    {
        typedef R (*FuncType)(P1, P2, P3, P4, P5);
        __ipcAutoAddGlobalFunc_5(const char* func_name, FuncType func_ptr)
        {
            ipcGlobalFunc_5<R, P1, P2, P3, P4, P5>* p_global_func = new ipcGlobalFunc_5<R, P1, P2, P3, P4, P5>(func_name, func_ptr);
            ipcRemoteRunManager::Inst().Add(p_global_func);
        }
    };

    template <class R, class RP1, class P2>
    struct __ipcAutoAddGlobalFunc_2_1
    {
        typedef R (*FuncType)(RP1&, P2);
        __ipcAutoAddGlobalFunc_2_1(const char* func_name, FuncType func_ptr)
        {
            ipcGlobalFunc_2_1<R, RP1, P2>* p_global_func = new ipcGlobalFunc_2_1<R, RP1, P2>(func_name, func_ptr);
            ipcRemoteRunManager::Inst().Add(p_global_func);
        }
    };

    template <class R, class RP1, class P2, class P3>
    struct __ipcAutoAddGlobalFunc_3_1
    {
        typedef R (*FuncType)(RP1&, P2, P3);
        __ipcAutoAddGlobalFunc_3_1(const char* func_name, FuncType func_ptr)
        {
            ipcGlobalFunc_3_1<R, RP1, P2, P3>* p_global_func = new ipcGlobalFunc_3_1<R, RP1, P2, P3>(func_name, func_ptr);
            ipcRemoteRunManager::Inst().Add(p_global_func);
        }
    };

    template <class R, class RP1, class RP2>
    struct __ipcAutoAddGlobalFunc_2_2
    {
        typedef R (*FuncType)(RP1&, RP2);
        __ipcAutoAddGlobalFunc_2_2(const char* func_name, FuncType func_ptr)
        {
            ipcGlobalFunc_2_2<R, RP1, RP2>* p_global_func = new ipcGlobalFunc_2_2<R, RP1, RP2>(func_name, func_ptr);
            ipcRemoteRunManager::Inst().Add(p_global_func);
        }
    };

    template <class R, class RP1, class RP2, class P3>
    struct __ipcAutoAddGlobalFunc_3_2
    {
        typedef R (*FuncType)(RP1&, RP2, P3);
        __ipcAutoAddGlobalFunc_3_2(const char* func_name, FuncType func_ptr)
        {
            ipcGlobalFunc_3_2<R, RP1, RP2, P3>* p_global_func = new ipcGlobalFunc_3_2<R, RP1, RP2, P3>(func_name, func_ptr);
            ipcRemoteRunManager::Inst().Add(p_global_func);
        }
    };

    template <class R, class RP1>
    struct __ipcAutoAddGlobalFunc_1_1
    {
        typedef R (*FuncType)(RP1&);
        __ipcAutoAddGlobalFunc_1_1(const char* func_name, FuncType func_ptr)
        {
            ipcGlobalFunc_1_1<R, RP1>* p_global_func = new ipcGlobalFunc_1_1<R, RP1>(func_name, func_ptr);
            ipcRemoteRunManager::Inst().Add(p_global_func);
        }
    };


    struct  __ipcAutoAddClass
    {
        typedef int (*PFNCreateObject)(void);
        __ipcAutoAddClass(const char* class_name, PFNCreateObject func_ptr)
        {
            ipcInterfaceObjectFactory::Inst().Add(class_name, func_ptr);
        }
    };

    template <class Class, class R>
    struct __ipcAutoAddClassFunc_0
    {
        typedef R (Class::*FuncType)(void);
        __ipcAutoAddClassFunc_0(const char* class_name, const char* func_name, FuncType func_ptr)
        {
            ipcClassFunc_0<Class, R>* p_class_func = new ipcClassFunc_0<Class, R>(func_name, func_ptr);
            ipcRemoteRunManager::Inst().Add<Class>(class_name, p_class_func);
        }
    };

    template <class Class, class R, class P1>
    struct __ipcAutoAddClassFunc_1
    {
        typedef R (Class::*FuncType)(P1);
        __ipcAutoAddClassFunc_1(const char* class_name, const char* func_name, FuncType func_ptr)
        {
            ipcClassFunc_1<Class, R, P1>* p_class_func = new ipcClassFunc_1<Class, R, P1>(func_name, func_ptr);
            ipcRemoteRunManager::Inst().Add<Class>(class_name, p_class_func);
        }
    };

    template <class Class, class R, class P1, class P2>
    struct __ipcAutoAddClassFunc_2
    {
        typedef R (Class::*FuncType)(P1, P2);
        __ipcAutoAddClassFunc_2(const char* class_name, const char* func_name, FuncType func_ptr)
        {
            ipcClassFunc_2<Class, R, P1, P2>* p_class_func = new ipcClassFunc_2<Class, R, P1, P2>(func_name, func_ptr);
            ipcRemoteRunManager::Inst().Add<Class>(class_name, p_class_func);
        }
    };

    template <class Class, class R, class P1, class P2, class P3>
    struct __ipcAutoAddClassFunc_3
    {
        typedef R (Class::*FuncType)(P1, P2, P3);
        __ipcAutoAddClassFunc_3(const char* class_name, const char* func_name, FuncType func_ptr)
        {
            ipcClassFunc_3<Class, R, P1, P2, P3>* p_class_func = new ipcClassFunc_3<Class, R, P1, P2, P3>(func_name, func_ptr);
            ipcRemoteRunManager::Inst().Add<Class>(class_name, p_class_func);
        }
    };

    template <class Class, class R, class P1, class P2, class P3, class P4>
    struct __ipcAutoAddClassFunc_4
    {
        typedef R (Class::*FuncType)(P1, P2, P3, P4);
        __ipcAutoAddClassFunc_4(const char* class_name, const char* func_name, FuncType func_ptr)
        {
            ipcClassFunc_4<Class, R, P1, P2, P3, P4>* p_class_func = new ipcClassFunc_4<Class, R, P1, P2, P3, P4>(func_name, func_ptr);
            ipcRemoteRunManager::Inst().Add<Class>(class_name, p_class_func);
        }
    };

    template <class Class, class R, class P1, class P2, class P3, class P4, class P5>
    struct __ipcAutoAddClassFunc_5
    {
        typedef R (Class::*FuncType)(P1, P2, P3, P4, P5);
        __ipcAutoAddClassFunc_5(const char* class_name, const char* func_name, FuncType func_ptr)
        {
            ipcClassFunc_5<Class, R, P1, P2, P3, P4, P5>* p_class_func = new ipcClassFunc_5<Class, R, P1, P2, P3, P4, P5>(func_name, func_ptr);
            ipcRemoteRunManager::Inst().Add<Class>(class_name, p_class_func);
        }
    };

    template <class Class>
    struct __ipcAutoAddClassObjectInst 
    {
        __ipcAutoAddClassObjectInst(int inst_id, Class* p_inst, ipcClassFuncSet<Class>* p_func_set)
        {
            ipcClassObjectInst<Class>* p_class_object_inst = new ipcClassObjectInst<Class>(inst_id, p_inst);
            p_class_object_inst->SetClassFuncSet(p_func_set);
            ipcRemoteRunManager::Inst().Add(p_class_object_inst);
        }
    };

}
}

#define EXPORT_GLOBAL_FUNC_0(func_name, R) zl::Ipc::__ipcAutoAddGlobalFunc_0<R> func_name##temp_0(#func_name, func_name)
#define EXPORT_GLOBAL_FUNC_1(func_name, R, P1) zl::Ipc::__ipcAutoAddGlobalFunc_1<R, P1> func_name##temp_1(#func_name, func_name)
#define EXPORT_GLOBAL_FUNC_2(func_name, R, P1, P2) zl::Ipc::__ipcAutoAddGlobalFunc_2<R, P1, P2> func_name##temp_2(#func_name, func_name)
#define EXPORT_GLOBAL_FUNC_3(func_name, R, P1, P2, P3) zl::Ipc::__ipcAutoAddGlobalFunc_3<R, P1, P2, P3> func_name##temp_3(#func_name, func_name)
#define EXPORT_GLOBAL_FUNC_4(func_name, R, P1, P2, P3, P4) zl::Ipc::__ipcAutoAddGlobalFunc_4<R, P1, P2, P3, P4> func_name##temp_4(#func_name, func_name)
#define EXPORT_GLOBAL_FUNC_5(func_name, R, P1, P2, P3, P4, P5) zl::Ipc::__ipcAutoAddGlobalFunc_5<R, P1, P2, P3, P4, P5> func_name##temp_5(#func_name, func_name)

///> 后面的参数 m_n   m表示参数个数，n表示引用参数的个数
///> 注意！使用该宏定义时，应用参数不要写后面的&
#define EXPORT_GLOBAL_FUNC_1_R1(func_name, R, RP1) zl::Ipc::__ipcAutoAddGlobalFunc_1_1<R, RP1> func_name##temp_1_1(#func_name, func_name)
#define EXPORT_GLOBAL_FUNC_2_R1(func_name, R, RP1, P2) zl::Ipc::__ipcAutoAddGlobalFunc_2_1<R, RP1, P2> func_name##temp_2_1(#func_name, func_name)
#define EXPORT_GLOBAL_FUNC_3_R1(func_name, R, RP1, P2, P3) zl::Ipc::__ipcAutoAddGlobalFunc_3_1<R, RP1, P2, P3> func_name##temp_3_1(#func_name, func_name)

#define EXPORT_GLOBAL_FUNC_2_R2(func_name, R, RP1, RP2) zl::Ipc::__ipcAutoAddGlobalFunc_2_2<R, RP1, RP2> func_name##temp_2_2(#func_name, func_name)
#define EXPORT_GLOBAL_FUNC_3_R2(func_name, R, RP1, RP2, P3) zl::Ipc::__ipcAutoAddGlobalFunc_3_2<R, RP1, RP2, P3> func_name##temp_3_2(#func_name, func_name)

#define DECLARE_EXPORT_INTERFACE(class_name) zl::Ipc::__ipcAutoAddClass class_name##auto_add_class(#class_name, class_name::__##class_name##_CreateObject);
#define EXPORT_CLASS_FUNC_0(class_name, func_name, R) zl::Ipc::__ipcAutoAddClassFunc_0<class_name, R> class_name##func_name##temp_0(#class_name, #func_name, &class_name::func_name)
#define EXPORT_CLASS_FUNC_1(class_name, func_name, R, P1) zl::Ipc::__ipcAutoAddClassFunc_1<class_name, R, P1> class_name##func_name##temp_1(#class_name, #func_name, &class_name::func_name)
#define EXPORT_CLASS_FUNC_2(class_name, func_name, R, P1, P2) zl::Ipc::__ipcAutoAddClassFunc_2<class_name, R, P1, P2> class_name##func_name##temp_2(#class_name, #func_name, &class_name::func_name)
#define EXPORT_CLASS_FUNC_3(class_name, func_name, R, P1, P2, P3) zl::Ipc::__ipcAutoAddClassFunc_3<class_name, R, P1, P2, P3> class_name##func_name##temp_3(#class_name, #func_name, &class_name::func_name)
#define EXPORT_CLASS_FUNC_4(class_name, func_name, R, P1, P2, P3, P4) zl::Ipc::__ipcAutoAddClassFunc_4<class_name, R, P1, P2, P3, P4> class_name##func_name##temp_4(#class_name, #func_name, &class_name::func_name)
#define EXPORT_CLASS_FUNC_5(class_name, func_name, R, P1, P2, P3, P4, P5) zl::Ipc::__ipcAutoAddClassFunc_5<class_name, R, P1, P2, P3, P4, P5> class_name##func_name##temp_5(#class_name, #func_name, &class_name::func_name)

#define ADD_CLASS_OBJECT_INST(class_name, id, p_inst) \
    do {	\
        zl::Ipc::ipcClassFuncSet<class_name>* p_func_set = (zl::Ipc::ipcClassFuncSet<class_name>*)zl::Ipc::ipcRemoteRunManager::Inst().GetClassFuncSet(#class_name); \
        zl::Ipc::__ipcAutoAddClassObjectInst<class_name> class_name##temp_inst(id, p_inst, p_func_set); \
    } while(0)

#define DECLARE_CLASS_CREATE_FUNC(class_name) \
    public: \
    static int __##class_name##_CreateObject(void) \
    { \
        int id; \
        class_name* p_inst = new class_name(); \
        id = (int)p_inst;/*  = KJsonRun::KIdGenerate::Inst().Get(); */\
        ADD_CLASS_OBJECT_INST(class_name, id, p_inst); \
        return id; \
    }

#endif