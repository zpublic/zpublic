#include "stdafx.h"
#include "test_json_cpp.h"
#include "json/json.h"

void test_json_cpp()
{
    Json::Value value;
    Json::Reader reader;
    std::string sJson("{\"a\":\"1\"}");
    bool bRet = reader.parse(sJson, value);
    assert(bRet);
    std::cout<<value.toStyledString()<<std::endl;
}
