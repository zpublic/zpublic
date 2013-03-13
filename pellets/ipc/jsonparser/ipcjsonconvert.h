#ifndef ZPUBLIC_JSON_PARSER_IPC_CONVERT
#define ZPUBLIC_JSON_PARSER_IPC_CONVERT

#include "../../../3rdparty/jsonlib/src/json/json.h"
#include "ipcpublic.h"
#include "ipcutfconv.h"
#include <tchar.h>

namespace zl
{
namespace Ipc
{
    template <class T>
    struct Value2Json 
    {
        Value2Json(const T& v) : json_value_(v)
        {
        }
        Json::Value& JsonValue()
        {
            return json_value_;
        }

    private:
        Json::Value json_value_;
    };

    template <class T>
    struct Value2Json<T&>
    {
        Value2Json(T& v) : json_value_(v)
        {
        }
        Json::Value& JsonValue()
        {
            return json_value_;
        }

    private:
        Json::Value json_value_;
    };

    template <>
    struct Value2Json<const long>
    {
        Value2Json(const long l)
        {
            double d = l;
            json_value_ = d;
        }
        Json::Value& JsonValue()
        {
            return json_value_;
        }

    private:
        Json::Value json_value_;
    };

    template <>
    struct Value2Json<const unsigned long>
    {
        Value2Json(const unsigned long l)
        {
            double d = l;
            json_value_ = d;
        }
        Json::Value& JsonValue()
        {
            return json_value_;
        }

    private:
        Json::Value json_value_;
    };

    template <>
    struct Value2Json<const wchar_t*>
    {
        Value2Json(const wchar_t* s)
        {
            json_value_ = KUTF16ToUTF8(s);
        }
        Json::Value& JsonValue()
        {
            return json_value_;
        }

    private:
        Json::Value json_value_;
    };

    template <>
    struct Value2Json<const std::wstring>
    {
        Value2Json(const std::wstring& v)
        {
            json_value_ = KUTF16ToUTF8(v.c_str());
        }
        Json::Value& JsonValue()
        {
            return json_value_;
        }

    private:
        Json::Value json_value_;
    };

    template <>
    struct Value2Json<const std::wstring&>
    {
        Value2Json(const std::wstring& v)
        {
            json_value_ = KUTF16ToUTF8(v.c_str());
        }
        Json::Value& JsonValue()
        {
            return json_value_;
        }

    private:
        Json::Value json_value_;
    };

    template <>
    struct Value2Json<const std::string>
    {
        Value2Json(const std::string& v)
        {
            json_value_ = v.c_str();
        }
        Json::Value& JsonValue()
        {
            return json_value_;
        }

    private:
        Json::Value json_value_;
    };

    template <>
    struct Value2Json<const std::string&>
    {
        Value2Json(const std::string& v)
        {
            json_value_ = v.c_str();
        }
        Json::Value& JsonValue()
        {
            return json_value_;
        }

    private:
        Json::Value json_value_;
    };

    template <class T>
    struct Value2Json<const std::list<T>>
    {
        Value2Json(const std::list<T>& lst)
        {
            std::list<T>::const_iterator iter;

            for (iter = lst.begin(); iter != lst.end(); ++iter)
            {
                Value2Json<T> temp_json(*iter);
                json_value_.append(temp_json.JsonValue());
            }
        }
        Json::Value& JsonValue()
        {
            return json_value_;
        }

    private:
        Json::Value json_value_;
    };

    template <class T>
    struct Value2Json<const std::list<T>&>
    {
        Value2Json(const std::list<T>& lst)
        {
            std::list<T>::iterator iter;

            for (iter = lst.begin(); iter != lst.end(); ++iter)
            {
                Value2Json<T> temp_json(*iter);
                json_value_.append(temp_json.JsonValue());
            }
        }
        Json::Value& JsonValue()
        {
            return json_value_;
        }

    private:
        Json::Value json_value_;
    };

    template <class T>
    struct Value2Json<const std::vector<T>&>
    {
        Value2Json(const std::vector<T>& vec)
        {
            std::vector<T>::iterator iter;

            for (iter = vec.begin(); iter != vec.end(); ++iter)
            {
                Value2Json<T> temp_json(*iter);
                json_value_.append(temp_json.JsonValue());
            }
        }
        Json::Value& JsonValue()
        {
            return json_value_;
        }

    private:
        Json::Value json_value_;
    };

    template <class T>
    struct Value2Json<const std::vector<T>>
    {
        Value2Json(const std::vector<T>& vec)
        {
            std::vector<T>::const_iterator iter;

            for (iter = vec.begin(); iter != vec.end(); ++iter)
            {
                Value2Json<T> temp_json(*iter);
                json_value_.append(temp_json.JsonValue());
            }
        }
        Json::Value& JsonValue()
        {
            return json_value_;
        }

    private:
        Json::Value json_value_;
    };

    template <class T>
    inline Json::Value GetJsonValue(const T& t)
    {
        Value2Json<const T> temp_json(t);
        return temp_json.JsonValue();
    }

    template <class T>
    inline Json::Value GetJsonValue(const T* t)
    {
        Value2Json<const T*> temp_json(t);
        return temp_json.JsonValue();
    }

    template <class T>
    inline void AssignValue2Json(Json::Value& v, const T& t)
    {
        Value2Json<const T> temp_json(t);
        v = temp_json.JsonValue();
    }

    template <class T>
    inline void AssignValue2Json(Json::Value& v, const T* t)
    {
        Value2Json<const T*> temp_json(t);
        v = temp_json.JsonValue();
    }


    /*
    @ ClassName : Json2Value
    @ Brief		: 将Json字符串转换成对应的数据
    @ Remark	: 支持的数据类型在该文件头处已有声明
    */
    template <class T>
    struct Json2Value 
    {
        Json2Value(Json::Value& v)
        {
            if (v.isBool()) value_ = v.asBool();
            else if (v.isInt()) value_ = v.asInt();
            else if (v.isUInt()) value_ = v.asUInt();
            else if (v.isDouble()) value_ = v.asDouble();
            else THROW_EXCEPTION;
        }

        T Value(){return (T)value_;}

    private:
        double value_;
    };

    template <>
    struct Json2Value<const char*> 
    {
        Json2Value(Json::Value& v)
        {
            if (v.isString()) value_ = v.asString();
            else THROW_EXCEPTION;
        }

        const char* Value(){return value_.c_str();}

    private:
        std::string value_;
    };

    template <>
    struct Json2Value<const wchar_t*> 
    {
        Json2Value(Json::Value& v)
        {
            if (v.isString()) value_ = KUTF8ToUTF16(v.asString().c_str());
            else THROW_EXCEPTION;
        }

        const wchar_t* Value(){return value_.c_str();}

    private:
        std::wstring value_;
    };

    template <>
    struct Json2Value<std::string> 
    {
        Json2Value(Json::Value& v)
        {
            if (v.isString()) value_ = v.asString();
            else THROW_EXCEPTION;
        }

        const char* Value(){return value_.c_str();}

    private:
        std::string value_;
    };

    template <>
    struct Json2Value<std::wstring> 
    {
        Json2Value(Json::Value& v)
        {
            if (v.isString()) value_ = KUTF8ToUTF16(v.asString().c_str());
            else THROW_EXCEPTION;
        }

        const wchar_t* Value(){return value_.c_str();}

    private:
        std::wstring value_;
    };

    template <>
    struct Json2Value<std::string&> 
    {
        Json2Value(Json::Value& v)
        {
            if (v.isString()) value_ = v.asString();
            else THROW_EXCEPTION;
        }

        std::string& Value(){return value_;}

    private:
        std::string value_;
    };

    template <>
    struct Json2Value<std::wstring&> 
    {
        Json2Value(Json::Value& v)
        {
            if (v.isString()) value_ = KUTF8ToUTF16(v.asString().c_str());
            else THROW_EXCEPTION;
        }

        std::wstring& Value(){return value_;}

    private:
        std::wstring value_;
    };

    template <class T>
    struct Json2Value<std::list<T>>
    {
        Json2Value(Json::Value& v)
        {
            if (v.isNull()) return;
            if (v.isArray()) 
            {
                for (int i = 0; i < v.size(); i++)
                {
                    Json2Value<T> temp_v(v[i]);
                    value_.push_back(temp_v.Value());
                }
            }
            else THROW_EXCEPTION;
        }

        std::list<T>& Value()
        {
            return value_;
        }

    private:
        std::list<T> value_;
    };

    template <class T>
    struct Json2Value<std::list<T>&>
    {
        Json2Value(Json::Value& v)
        {
            if (v.isNull()) return;
            if (v.isArray()) 
            {
                for (int i = 0; i < v.size(); i++)
                {
                    Json2Value<T> temp_v(v[i]);
                    value_.push_back(temp_v.Value());
                }
            }
            else THROW_EXCEPTION;
        }

        std::list<T>& Value()
        {
            return value_;
        }

    private:
        std::list<T> value_;
    };

    template <class T>
    struct Json2Value<const std::list<T>&>
    {
        Json2Value(Json::Value& v)
        {
            if (v.isNull()) return;
            if (v.isArray()) 
            {
                for (int i = 0; i < v.size(); i++)
                {
                    Json2Value<T> temp_v(v[i]);
                    value_.push_back(temp_v.Value());
                }
            }
            else THROW_EXCEPTION;
        }

        std::list<T>& Value()
        {
            return value_;
        }

    private:
        std::list<T> value_;
    };

    template <class T>
    struct Json2Value<std::vector<T>>
    {
        Json2Value(Json::Value& v)
        {
            if (v.isNull()) return;
            if (v.isArray()) 
            {
                for (int i = 0; i < v.size(); i++)
                {
                    Json2Value<T> temp_v(v[i]);
                    value_.push_back(temp_v.Value());
                }
            }
            else THROW_EXCEPTION;
        }

        std::vector<T>& Value()
        {
            return value_;
        }

    private:
        std::vector<T> value_;
    };

    template <class T>
    struct Json2Value<std::vector<T>&>
    {
        Json2Value(Json::Value& v)
        {
            if (v.isNull()) return;
            if (v.isArray()) 
            {
                for (int i = 0; i < v.size(); i++)
                {
                    Json2Value<T> temp_v(v[i]);
                    value_.push_back(temp_v.Value());
                }
            }
            else THROW_EXCEPTION;
        }

        std::vector<T>& Value()
        {
            return value_;
        }

    private:
        std::vector<T> value_;
    };

    template <class T>
    struct Json2Value<const std::vector<T>&>
    {
        Json2Value(Json::Value& v)
        {
            if (v.isNull()) return;
            if (v.isArray()) 
            {
                for (int i = 0; i < v.size(); i++)
                {
                    Json2Value<T> temp_v(v[i]);
                    value_.push_back(temp_v.Value());
                }
            }
            else THROW_EXCEPTION;
        }

        std::vector<T>& Value()
        {
            return value_;
        }

    private:
        std::vector<T> value_;
    };

    template <class T>
    inline void AssignJson2Value(T& v, Json::Value& value, size_t size) 
    { 
        Json2Value<T> json_v(value); 
        v = json_v.Value(); 
    }

    inline void AssignJson2Value(char* left, Json::Value& value, size_t size) 
    { 
        Json2Value<const char*> json_v(value); 
        size_t len = strlen(json_v.Value()); 
        if (len >= size) 
        { 
            strncpy(left, json_v.Value(), size - 1); 
            left[size - 1] = 0; 
        } 
        else 
        { 
            strcpy(left, json_v.Value()); 
        } 
    }

    inline void AssignJson2Value(wchar_t* left, Json::Value& value, size_t size) 
    { 
        Json2Value<const wchar_t*> json_v(value); 
        size_t len = wcslen(json_v.Value());  
        size /= 2; 
        if (len >= size) 
        { 
            wcsncpy(left, json_v.Value(), size - 1); 
            left[size - 1] = 0; 
        } 
        else 
        { 
            wcscpy(left, json_v.Value()); 
        } 
    }

    class ipcJsonReturn
    {
    public:
        ipcJsonReturn(enumReturnValule code = enumRet_Succeed, const char* msg = "") : code_(code), msg_(msg)
        {
            value_[defJsonRetCode] = code;
            value_[defJsonRetMessage] = msg;
        }
        template<class T>
        ipcJsonReturn(T& v, enumReturnValule code = enumRet_Succeed, const char* msg = "")
        {
            value_[defJsonRetCode] = code;
            value_[defJsonRetMessage] = msg;
            value_[defJsonRetValue] = GetJsonValue(v);
        }

        template<class T, class RP1>
        ipcJsonReturn(T& v, RP1& rp1, enumReturnValule code = enumRet_Succeed, const char* msg = "")
        {
            value_[defJsonRetCode] = code;
            value_[defJsonRetMessage] = msg;
            Json::Value ret;
            ret[defJsonRetValue] = GetJsonValue(v);
            ret[defJsonRetParam1] = GetJsonValue(rp1);
            value_[defJsonRetValue] = ret;
        }

        template<class T, class RP1, class RP2>
        ipcJsonReturn(T& v, RP1& rp1, RP2& rp2, enumReturnValule code = enumRet_Succeed, const char* msg = "")
        {
            value_[defJsonRetCode] = code;
            value_[defJsonRetMessage] = msg;
            Json::Value ret;
            ret[defJsonRetValue] = GetJsonValue(v);
            ret[defJsonRetParam1] = GetJsonValue(rp1);
            ret[defJsonRetParam2] = GetJsonValue(rp2);
            value_[defJsonRetValue] = ret;
        }
        std::string ToString(void)
        {
            Json::FastWriter writer;
            return writer.write(value_);
        }

    private:
        int code_;
        std::string msg_;
        Json::Value value_;
    };

    struct ipcReturnParse
    {
        ipcReturnParse() : code_(enumRet_Error){}

        ipcReturnParse(std::string& s){ Parse(s); }

        std::string& operator = (std::string& s){ Parse(s); return s; }
        const char* operator = (const char* s){ Parse(s); return s; }

        int Code(){ return code_; }

        bool Error(){ return code_ < 0; }

        bool Succeed(){ return code_ >= 0; }

        std::string& Msg(){ return msg_; }

        template <class T>
        bool Value(T& v){
            if (code_ < 0) return false;
            
            try
            {
                Json2Value<T> value(value_);
                v = value.Value();
                return true;
            }
            catch (...) {
                return false;
            }
        }

        template <class T, class RP1>
        bool Value(T& v, RP1& p1){
            if (code_ < 0) return false;

            try
            {
                Json2Value<T> value(value_[defJsonRetValue]);
                v = value.Value();
                Json2Value<RP1> valuep1(value_[defJsonRetParam1]);
                p1 = valuep1.Value();
                return true;
            }
            catch (...) {
                return false;
            }
        }

        template <class T, class RP1, class RP2>
        bool Value(T& v, RP1& p1, RP2& p2){
            if (code_ < 0) return false;

            try
            {
                Json2Value<T> value(value_[defJsonRetValue]);
                v = value.Value();
                Json2Value<RP1> valuep1(value_[defJsonRetParam1]);
                p1 = valuep1.Value();
                Json2Value<RP2> valuep2(value_[defJsonRetParam2]);
                p2 = valuep2.Value();
                return true;
            }
            catch (...) {
                return false;
            }
        }
    protected:
        void GetCode(Json::Value& v)
        {
            Json::Value code_value;
            code_value = v[defJsonRetCode];
            if (!code_value.isInt()) code_ = enumRet_Error;
            else code_ = code_value.asInt();
        }

        void GetMsg(Json::Value& v)
        {
            Json::Value msg_value;
            msg_value = v[defJsonRetMessage];
            if (msg_value.isString()) msg_ = msg_value.asCString();
        }

        void GetValue(Json::Value& v)
        {
            value_ = v[defJsonRetValue];
        }
        void Parse(std::string& s)
        {
            code_ = enumRet_Error;
            Json::Reader reader;
            Json::Value json_value;
            if (reader.parse(s, json_value))
            {
                GetCode(json_value);
                GetMsg(json_value);
                GetValue(json_value);
            }
            else code_ = enumRet_Error;
        }

        void Parse(const char* s)
        {
            code_ = enumRet_Error;
            Json::Reader reader;
            Json::Value json_value;
            size_t size = strlen(s);
            if (reader.parse(s, s + size,json_value))
            {
                GetCode(json_value);
                GetMsg(json_value);
                GetValue(json_value);
            }
            else code_ = enumRet_Error;
        }

    private:
        int code_;
        std::string msg_;
        Json::Value value_;
    };

}
}

#define STRUCT_TO_JSON_1(struct_name, m1) \
namespace zl { \
namespace Ipc { \
    template <> \
    struct Value2Json<const struct_name> \
    { \
        Value2Json(const struct_name& s) \
        { \
            AssignValue2Json(json_value_[#m1], s.m1); \
        } \
        Json::Value& JsonValue() \
        { \
            return json_value_; \
        } \
    private: \
        Json::Value json_value_; \
    }; \
    template <> \
    struct Value2Json<const struct_name&> \
    { \
        Value2Json(const struct_name& s) \
        { \
            AssignValue2Json(json_value_[#m1], s.m1); \
        } \
        Json::Value& JsonValue() \
        { \
            return json_value_; \
        } \
    private: \
        Json::Value json_value_; \
    }; \
}\
}

#define STRUCT_TO_JSON_2(struct_name, m1, m2) \
namespace zl { \
namespace Ipc { \
    template <> \
    struct Value2Json<const struct_name> \
    { \
        Value2Json(const struct_name& s) \
        { \
            AssignValue2Json(json_value_[#m1], s.m1); \
            AssignValue2Json(json_value_[#m2], s.m2); \
        } \
        Json::Value& JsonValue() \
        { \
            return json_value_; \
        } \
    private: \
        Json::Value json_value_; \
    }; \
    template <> \
    struct Value2Json<const struct_name&> \
    { \
        Value2Json(const struct_name& s) \
        { \
            AssignValue2Json(json_value_[#m1], s.m1); \
            AssignValue2Json(json_value_[#m2], s.m2); \
        } \
        Json::Value& JsonValue() \
        { \
            return json_value_; \
        } \
    private: \
        Json::Value json_value_; \
    }; \
}\
}

#define STRUCT_TO_JSON_3(struct_name, m1, m2, m3) \
namespace zl { \
namespace Ipc { \
    template <> \
    struct Value2Json<const struct_name> \
    { \
        Value2Json(const struct_name& s) \
        { \
            AssignValue2Json(json_value_[#m1], s.m1); \
            AssignValue2Json(json_value_[#m2], s.m2); \
            AssignValue2Json(json_value_[#m3], s.m3); \
        } \
        Json::Value& JsonValue() \
        { \
            return json_value_; \
        } \
    private: \
        Json::Value json_value_; \
    }; \
    template <> \
    struct Value2Json<const struct_name&> \
    { \
        Value2Json(const struct_name& s) \
        { \
            AssignValue2Json(json_value_[#m1], s.m1); \
            AssignValue2Json(json_value_[#m2], s.m2); \
            AssignValue2Json(json_value_[#m3], s.m3); \
        } \
        Json::Value& JsonValue() \
        { \
            return json_value_; \
        } \
    private: \
        Json::Value json_value_; \
    }; \
}\
}

#define STRUCT_TO_JSON_4(struct_name, m1, m2, m3, m4) \
namespace zl { \
namespace Ipc { \
    template <> \
struct Value2Json<const struct_name> \
    { \
    Value2Json(const struct_name& s) \
        { \
        AssignValue2Json(json_value_[#m1], s.m1); \
        AssignValue2Json(json_value_[#m2], s.m2); \
        AssignValue2Json(json_value_[#m3], s.m3); \
        AssignValue2Json(json_value_[#m4], s.m4); \
        } \
        Json::Value& JsonValue() \
        { \
        return json_value_; \
        } \
    private: \
        Json::Value json_value_; \
    }; \
    template <> \
struct Value2Json<const struct_name&> \
    { \
    Value2Json(const struct_name& s) \
        { \
        AssignValue2Json(json_value_[#m1], s.m1); \
        AssignValue2Json(json_value_[#m2], s.m2); \
        AssignValue2Json(json_value_[#m3], s.m3); \
        AssignValue2Json(json_value_[#m4], s.m4); \
        } \
        Json::Value& JsonValue() \
        { \
        return json_value_; \
        } \
    private: \
        Json::Value json_value_; \
    }; \
}\
}

#define STRUCT_TO_JSON_5(struct_name, m1, m2, m3, m4, m5) \
namespace zl { \
namespace Ipc { \
    template <> \
struct Value2Json<const struct_name> \
    { \
    Value2Json(const struct_name& s) \
        { \
        AssignValue2Json(json_value_[#m1], s.m1); \
        AssignValue2Json(json_value_[#m2], s.m2); \
        AssignValue2Json(json_value_[#m3], s.m3); \
        AssignValue2Json(json_value_[#m4], s.m4); \
        AssignValue2Json(json_value_[#m5], s.m5); \
        } \
        Json::Value& JsonValue() \
        { \
        return json_value_; \
        } \
    private: \
        Json::Value json_value_; \
    }; \
    template <> \
struct Value2Json<const struct_name&> \
    { \
    Value2Json(const struct_name& s) \
        { \
        AssignValue2Json(json_value_[#m1], s.m1); \
        AssignValue2Json(json_value_[#m2], s.m2); \
        AssignValue2Json(json_value_[#m3], s.m3); \
        AssignValue2Json(json_value_[#m4], s.m4); \
        AssignValue2Json(json_value_[#m5], s.m5); \
        } \
        Json::Value& JsonValue() \
        { \
        return json_value_; \
        } \
    private: \
        Json::Value json_value_; \
    }; \
}\
}

#define STRUCT_TO_JSON_6(struct_name, m1, m2, m3, m4, m5, m6) \
namespace zl { \
namespace Ipc { \
    template <> \
struct Value2Json<const struct_name> \
    { \
    Value2Json(const struct_name& s) \
        { \
        AssignValue2Json(json_value_[#m1], s.m1); \
        AssignValue2Json(json_value_[#m2], s.m2); \
        AssignValue2Json(json_value_[#m3], s.m3); \
        AssignValue2Json(json_value_[#m4], s.m4); \
        AssignValue2Json(json_value_[#m5], s.m5); \
        AssignValue2Json(json_value_[#m6], s.m6); \
        } \
        Json::Value& JsonValue() \
        { \
        return json_value_; \
        } \
    private: \
        Json::Value json_value_; \
    }; \
    template <> \
struct Value2Json<const struct_name&> \
    { \
    Value2Json(const struct_name& s) \
        { \
        AssignValue2Json(json_value_[#m1], s.m1); \
        AssignValue2Json(json_value_[#m2], s.m2); \
        AssignValue2Json(json_value_[#m3], s.m3); \
        AssignValue2Json(json_value_[#m4], s.m4); \
        AssignValue2Json(json_value_[#m5], s.m5); \
        AssignValue2Json(json_value_[#m6], s.m6); \
        } \
        Json::Value& JsonValue() \
        { \
        return json_value_; \
        } \
    private: \
        Json::Value json_value_; \
    }; \
}\
}

#define STRUCT_TO_JSON_7(struct_name, m1, m2, m3, m4, m5, m6, m7) \
namespace zl { \
namespace Ipc { \
    template <> \
struct Value2Json<const struct_name> \
    { \
    Value2Json(const struct_name& s) \
        { \
        AssignValue2Json(json_value_[#m1], s.m1); \
        AssignValue2Json(json_value_[#m2], s.m2); \
        AssignValue2Json(json_value_[#m3], s.m3); \
        AssignValue2Json(json_value_[#m4], s.m4); \
        AssignValue2Json(json_value_[#m5], s.m5); \
        AssignValue2Json(json_value_[#m6], s.m6); \
        AssignValue2Json(json_value_[#m7], s.m7); \
        } \
        Json::Value& JsonValue() \
        { \
        return json_value_; \
        } \
    private: \
        Json::Value json_value_; \
    }; \
    template <> \
struct Value2Json<const struct_name&> \
    { \
    Value2Json(const struct_name& s) \
        { \
        AssignValue2Json(json_value_[#m1], s.m1); \
        AssignValue2Json(json_value_[#m2], s.m2); \
        AssignValue2Json(json_value_[#m3], s.m3); \
        AssignValue2Json(json_value_[#m4], s.m4); \
        AssignValue2Json(json_value_[#m5], s.m5); \
        AssignValue2Json(json_value_[#m6], s.m6); \
        AssignValue2Json(json_value_[#m7], s.m7); \
        } \
        Json::Value& JsonValue() \
        { \
        return json_value_; \
        } \
    private: \
        Json::Value json_value_; \
    }; \
}\
}

#define STRUCT_TO_JSON_8(struct_name, m1, m2, m3, m4, m5, m6, m7, m8) \
namespace zl { \
namespace Ipc { \
    template <> \
struct Value2Json<const struct_name> \
    { \
    Value2Json(const struct_name& s) \
        { \
        AssignValue2Json(json_value_[#m1], s.m1); \
        AssignValue2Json(json_value_[#m2], s.m2); \
        AssignValue2Json(json_value_[#m3], s.m3); \
        AssignValue2Json(json_value_[#m4], s.m4); \
        AssignValue2Json(json_value_[#m5], s.m5); \
        AssignValue2Json(json_value_[#m6], s.m6); \
        AssignValue2Json(json_value_[#m7], s.m7); \
        AssignValue2Json(json_value_[#m8], s.m8); \
        } \
        Json::Value& JsonValue() \
        { \
        return json_value_; \
        } \
    private: \
        Json::Value json_value_; \
    }; \
    template <> \
struct Value2Json<const struct_name&> \
    { \
    Value2Json(const struct_name& s) \
        { \
        AssignValue2Json(json_value_[#m1], s.m1); \
        AssignValue2Json(json_value_[#m2], s.m2); \
        AssignValue2Json(json_value_[#m3], s.m3); \
        AssignValue2Json(json_value_[#m4], s.m4); \
        AssignValue2Json(json_value_[#m5], s.m5); \
        AssignValue2Json(json_value_[#m6], s.m6); \
        AssignValue2Json(json_value_[#m7], s.m7); \
        AssignValue2Json(json_value_[#m8], s.m8); \
        } \
        Json::Value& JsonValue() \
        { \
        return json_value_; \
        } \
    private: \
        Json::Value json_value_; \
    }; \
}\
}


#define JSON_TO_STRUCT_VALUE_1(struct_name, m1) \
namespace zl { \
namespace Ipc { \
    template <> \
    struct Json2Value<struct_name> \
    { \
        Json2Value(Json::Value& v) \
        { \
            if (v.isObject()) \
            { \
                AssignJson2Value(value_.m1, v[#m1], sizeof(value_.m1)); \
            } \
            else THROW_EXCEPTION; \
        } \
        struct_name& Value() \
        { \
            return value_; \
        } \
    private: \
        struct_name value_; \
    };\
    template <> \
    struct Json2Value<struct_name&> \
    { \
        Json2Value(Json::Value& v) \
        { \
            if (v.isObject()) \
            { \
                AssignJson2Value(value_.m1, v[#m1], sizeof(value_.m1)); \
            } \
            else THROW_EXCEPTION; \
        } \
        struct_name& Value() \
        { \
            return value_; \
        } \
    private: \
        struct_name value_; \
    }; \
}

#define JSON_TO_STRUCT_VALUE_2(struct_name, m1, m2) \
namespace zl { \
namespace Ipc { \
    template <> \
    struct Json2Value<struct_name> \
    { \
        Json2Value(Json::Value& v) \
        { \
            if (v.isObject()) \
            { \
                AssignJson2Value(value_.m1, v[#m1], sizeof(value_.m1)); \
                AssignJson2Value(value_.m2, v[#m2], sizeof(value_.m2)); \
            } \
            else THROW_EXCEPTION; \
        } \
        struct_name& Value() \
        { \
            return value_; \
        } \
    private: \
        struct_name value_; \
    }; \
    template <> \
    struct Json2Value<struct_name&> \
    { \
        Json2Value(Json::Value& v) \
        { \
            if (v.isObject()) \
            { \
                AssignJson2Value(value_.m1, v[#m1], sizeof(value_.m1)); \
                AssignJson2Value(value_.m2, v[#m2], sizeof(value_.m2)); \
            } \
            else THROW_EXCEPTION; \
        } \
        struct_name& Value() \
        { \
            return value_; \
        } \
    private: \
        struct_name value_; \
    }; \
}\
}

#define JSON_TO_STRUCT_VALUE_3(struct_name, m1, m2, m3) \
namespace zl { \
namespace Ipc { \
    template <> \
    struct Json2Value<struct_name> \
    { \
        Json2Value(Json::Value& v) \
        { \
            if (v.isObject()) \
            { \
                AssignJson2Value(value_.m1, v[#m1], sizeof(value_.m1)); \
                AssignJson2Value(value_.m2, v[#m2], sizeof(value_.m2)); \
                AssignJson2Value(value_.m3, v[#m3], sizeof(value_.m3)); \
            } \
            else THROW_EXCEPTION; \
        } \
        struct_name& Value() \
        { \
            return value_; \
        } \
    private: \
        struct_name value_; \
    };\
    template <> \
    struct Json2Value<struct_name&> \
    { \
        Json2Value(Json::Value& v) \
        { \
            if (v.isObject()) \
            { \
                AssignJson2Value(value_.m1, v[#m1], sizeof(value_.m1)); \
                AssignJson2Value(value_.m2, v[#m2], sizeof(value_.m2)); \
                AssignJson2Value(value_.m3, v[#m3], sizeof(value_.m3)); \
            } \
            else THROW_EXCEPTION; \
        } \
        struct_name& Value() \
        { \
            return value_; \
        } \
    private: \
        struct_name value_; \
    }; \
}\
}

#define JSON_TO_STRUCT_VALUE_4(struct_name, m1, m2, m3, m4) \
namespace zl { \
namespace Ipc { \
    template <> \
    struct Json2Value<struct_name> \
    { \
    Json2Value(Json::Value& v) \
        { \
        if (v.isObject()) \
            { \
            AssignJson2Value(value_.m1, v[#m1], sizeof(value_.m1)); \
            AssignJson2Value(value_.m2, v[#m2], sizeof(value_.m2)); \
            AssignJson2Value(value_.m3, v[#m3], sizeof(value_.m3)); \
            AssignJson2Value(value_.m4, v[#m4], sizeof(value_.m4)); \
            } \
            else THROW_EXCEPTION; \
        } \
        struct_name& Value() \
        { \
        return value_; \
        } \
    private: \
        struct_name value_; \
    };\
    template <> \
    struct Json2Value<struct_name&> \
    { \
    Json2Value(Json::Value& v) \
        { \
        if (v.isObject()) \
            { \
            AssignJson2Value(value_.m1, v[#m1], sizeof(value_.m1)); \
            AssignJson2Value(value_.m2, v[#m2], sizeof(value_.m2)); \
            AssignJson2Value(value_.m3, v[#m3], sizeof(value_.m3)); \
            AssignJson2Value(value_.m4, v[#m4], sizeof(value_.m4)); \
            } \
            else THROW_EXCEPTION; \
        } \
        struct_name& Value() \
        { \
        return value_; \
        } \
    private: \
        struct_name value_; \
    }; \
}\
}

#define JSON_TO_STRUCT_VALUE_5(struct_name, m1, m2, m3, m4, m5) \
namespace zl { \
namespace Ipc { \
    template <> \
    struct Json2Value<struct_name> \
    { \
    Json2Value(Json::Value& v) \
        { \
        if (v.isObject()) \
            { \
            AssignJson2Value(value_.m1, v[#m1], sizeof(value_.m1)); \
            AssignJson2Value(value_.m2, v[#m2], sizeof(value_.m2)); \
            AssignJson2Value(value_.m3, v[#m3], sizeof(value_.m3)); \
            AssignJson2Value(value_.m4, v[#m4], sizeof(value_.m4)); \
            AssignJson2Value(value_.m5, v[#m5], sizeof(value_.m5)); \
            } \
            else THROW_EXCEPTION; \
        } \
        struct_name& Value() \
        { \
        return value_; \
        } \
    private: \
        struct_name value_; \
    };\
    template <> \
    struct Json2Value<struct_name&> \
    { \
    Json2Value(Json::Value& v) \
        { \
        if (v.isObject()) \
            { \
            AssignJson2Value(value_.m1, v[#m1], sizeof(value_.m1)); \
            AssignJson2Value(value_.m2, v[#m2], sizeof(value_.m2)); \
            AssignJson2Value(value_.m3, v[#m3], sizeof(value_.m3)); \
            AssignJson2Value(value_.m4, v[#m4], sizeof(value_.m4)); \
            AssignJson2Value(value_.m5, v[#m5], sizeof(value_.m5)); \
            } \
            else THROW_EXCEPTION; \
        } \
        struct_name& Value() \
        { \
        return value_; \
        } \
    private: \
        struct_name value_; \
    }; \
}\
}

#define JSON_TO_STRUCT_VALUE_6(struct_name, m1, m2, m3, m4, m5, m6) \
namespace zl { \
namespace Ipc { \
    template <> \
    struct Json2Value<struct_name> \
    { \
    Json2Value(Json::Value& v) \
        { \
        if (v.isObject()) \
            { \
            AssignJson2Value(value_.m1, v[#m1], sizeof(value_.m1)); \
            AssignJson2Value(value_.m2, v[#m2], sizeof(value_.m2)); \
            AssignJson2Value(value_.m3, v[#m3], sizeof(value_.m3)); \
            AssignJson2Value(value_.m4, v[#m4], sizeof(value_.m4)); \
            AssignJson2Value(value_.m5, v[#m5], sizeof(value_.m5)); \
            AssignJson2Value(value_.m6, v[#m6], sizeof(value_.m6)); \
            } \
            else THROW_EXCEPTION; \
        } \
        struct_name& Value() \
        { \
        return value_; \
        } \
    private: \
        struct_name value_; \
    };\
    template <> \
    struct Json2Value<struct_name&> \
    { \
    Json2Value(Json::Value& v) \
        { \
        if (v.isObject()) \
            { \
            AssignJson2Value(value_.m1, v[#m1], sizeof(value_.m1)); \
            AssignJson2Value(value_.m2, v[#m2], sizeof(value_.m2)); \
            AssignJson2Value(value_.m3, v[#m3], sizeof(value_.m3)); \
            AssignJson2Value(value_.m4, v[#m4], sizeof(value_.m4)); \
            AssignJson2Value(value_.m5, v[#m5], sizeof(value_.m5)); \
            AssignJson2Value(value_.m6, v[#m6], sizeof(value_.m6)); \
            } \
            else THROW_EXCEPTION; \
        } \
        struct_name& Value() \
        { \
        return value_; \
        } \
    private: \
        struct_name value_; \
    }; \
}\
}

#define JSON_TO_STRUCT_VALUE_7(struct_name, m1, m2, m3, m4, m5, m6, m7) \
namespace zl { \
namespace Ipc { \
    template <> \
    struct Json2Value<struct_name> \
    { \
    Json2Value(Json::Value& v) \
        { \
        if (v.isObject()) \
            { \
            AssignJson2Value(value_.m1, v[#m1], sizeof(value_.m1)); \
            AssignJson2Value(value_.m2, v[#m2], sizeof(value_.m2)); \
            AssignJson2Value(value_.m3, v[#m3], sizeof(value_.m3)); \
            AssignJson2Value(value_.m4, v[#m4], sizeof(value_.m4)); \
            AssignJson2Value(value_.m5, v[#m5], sizeof(value_.m5)); \
            AssignJson2Value(value_.m6, v[#m6], sizeof(value_.m6)); \
            AssignJson2Value(value_.m7, v[#m7], sizeof(value_.m7)); \
            } \
            else THROW_EXCEPTION; \
        } \
        struct_name& Value() \
        { \
        return value_; \
        } \
    private: \
        struct_name value_; \
    };\
    template <> \
    struct Json2Value<struct_name&> \
    { \
    Json2Value(Json::Value& v) \
        { \
        if (v.isObject()) \
            { \
            AssignJson2Value(value_.m1, v[#m1], sizeof(value_.m1)); \
            AssignJson2Value(value_.m2, v[#m2], sizeof(value_.m2)); \
            AssignJson2Value(value_.m3, v[#m3], sizeof(value_.m3)); \
            AssignJson2Value(value_.m4, v[#m4], sizeof(value_.m4)); \
            AssignJson2Value(value_.m5, v[#m5], sizeof(value_.m5)); \
            AssignJson2Value(value_.m6, v[#m6], sizeof(value_.m6)); \
            AssignJson2Value(value_.m7, v[#m7], sizeof(value_.m7)); \
            } \
            else THROW_EXCEPTION; \
        } \
        struct_name& Value() \
        { \
        return value_; \
        } \
    private: \
        struct_name value_; \
    }; \
}\
}

#define JSON_TO_STRUCT_VALUE_8(struct_name, m1, m2, m3, m4, m5, m6, m7, m8) \
namespace zl { \
namespace Ipc { \
    template <> \
    struct Json2Value<struct_name> \
    { \
    Json2Value(Json::Value& v) \
        { \
        if (v.isObject()) \
            { \
            AssignJson2Value(value_.m1, v[#m1], sizeof(value_.m1)); \
            AssignJson2Value(value_.m2, v[#m2], sizeof(value_.m2)); \
            AssignJson2Value(value_.m3, v[#m3], sizeof(value_.m3)); \
            AssignJson2Value(value_.m4, v[#m4], sizeof(value_.m4)); \
            AssignJson2Value(value_.m5, v[#m5], sizeof(value_.m5)); \
            AssignJson2Value(value_.m6, v[#m6], sizeof(value_.m6)); \
            AssignJson2Value(value_.m7, v[#m7], sizeof(value_.m7)); \
            AssignJson2Value(value_.m8, v[#m8], sizeof(value_.m8)); \
            } \
            else THROW_EXCEPTION; \
        } \
        struct_name& Value() \
        { \
        return value_; \
        } \
    private: \
        struct_name value_; \
    };\
    template <> \
    struct Json2Value<struct_name&> \
    { \
    Json2Value(Json::Value& v) \
        { \
        if (v.isObject()) \
            { \
            AssignJson2Value(value_.m1, v[#m1], sizeof(value_.m1)); \
            AssignJson2Value(value_.m2, v[#m2], sizeof(value_.m2)); \
            AssignJson2Value(value_.m3, v[#m3], sizeof(value_.m3)); \
            AssignJson2Value(value_.m4, v[#m4], sizeof(value_.m4)); \
            AssignJson2Value(value_.m5, v[#m5], sizeof(value_.m5)); \
            AssignJson2Value(value_.m6, v[#m6], sizeof(value_.m6)); \
            AssignJson2Value(value_.m7, v[#m7], sizeof(value_.m7)); \
            AssignJson2Value(value_.m8, v[#m8], sizeof(value_.m8)); \
            } \
            else THROW_EXCEPTION; \
        } \
        struct_name& Value() \
        { \
        return value_; \
        } \
    private: \
        struct_name value_; \
    }; \
}\
}

#define DECLARE_JSON_CONVERSION_1(struct_name, m1) \
    JSON_TO_STRUCT_VALUE_1(struct_name, m1)\
    STRUCT_TO_JSON_1(struct_name, m1)

#define DECLARE_JSON_CONVERSION_2(struct_name, m1, m2) \
    JSON_TO_STRUCT_VALUE_2(struct_name, m1, m2)\
    STRUCT_TO_JSON_2(struct_name, m1, m2)

#define DECLARE_JSON_CONVERSION_3(struct_name, m1, m2, m3) \
    JSON_TO_STRUCT_VALUE_3(struct_name, m1, m2, m3)\
    STRUCT_TO_JSON_3(struct_name, m1, m2, m3)

#define DECLARE_JSON_CONVERSION_4(struct_name, m1, m2, m3, m4) \
    JSON_TO_STRUCT_VALUE_4(struct_name, m1, m2, m3, m4)\
    STRUCT_TO_JSON_4(struct_name, m1, m2, m3, m4)

#define DECLARE_JSON_CONVERSION_5(struct_name, m1, m2, m3, m4, m5) \
    JSON_TO_STRUCT_VALUE_5(struct_name, m1, m2, m3, m4, m5)\
    STRUCT_TO_JSON_5(struct_name, m1, m2, m3, m4, m5)

#define DECLARE_JSON_CONVERSION_6(struct_name, m1, m2, m3, m4, m5, m6) \
    JSON_TO_STRUCT_VALUE_6(struct_name, m1, m2, m3, m4, m5, m6)\
    STRUCT_TO_JSON_6(struct_name, m1, m2, m3, m4, m5, m6)

#define DECLARE_JSON_CONVERSION_7(struct_name, m1, m2, m3, m4, m5, m6, m7) \
    JSON_TO_STRUCT_VALUE_7(struct_name, m1, m2, m3, m4, m5, m6, m7)\
    STRUCT_TO_JSON_7(struct_name, m1, m2, m3, m4, m5, m6, m7)

#define DECLARE_JSON_CONVERSION_8(struct_name, m1, m2, m3, m4, m5, m6, m7, m8) \
    JSON_TO_STRUCT_VALUE_8(struct_name, m1, m2, m3, m4, m5, m6, m7, m8)\
    STRUCT_TO_JSON_8(struct_name, m1, m2, m3, m4, m5, m6, m7, m8)

#endif
