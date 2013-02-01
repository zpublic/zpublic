#pragma once

#include "def.h"
#include "serialize/serialize.h"

using namespace zl::Serialize;

class CTestSerialize : public Suite
{
public:
    CTestSerialize(void);
    ~CTestSerialize(void);

    void testDataFormat()
    {
        DataFormat f1(1, "111");
        TEST_ASSERT(f1.Id == 1);
        TEST_ASSERT(f1.Name == "111");
        DataItem i1("a", DataMType_Array, DataType_String);
        DataItem i2("b", DataMType_Value, DataType_Uint16);
        TEST_ASSERT(i1.Name == "a");
        TEST_ASSERT(i2.Type == DataType_Uint16);
        f1.AddItem(i1);
        f1.AddItem(i2);
        f1.AddItem("c", DataMType_Value, DataType_String);
        f1.AddItem("d", DataMType_Value, DataType_Uint32);
        TEST_ASSERT(f1.ItemMap.size() == 4);
        m_formatMgr.AddFormat(f1);

        DataFormat* pFormat = m_formatMgr.GetFormat(1);
        TEST_ASSERT(pFormat != NULL);
        TEST_ASSERT(pFormat->Name == "111");
        DataFormat* pFormat2 = m_formatMgr.GetFormat("111");
        TEST_ASSERT(pFormat2 != NULL);
        TEST_ASSERT(pFormat2->ItemMap["a"].MType == DataMType_Array);

        m_Eng.Init(&m_formatMgr);
    }

    void testSerialize()
    {
        CData data(1, "111");
        ArrayData* pArr = new ArrayData();
        pArr->Push(new StringData("a1"));
        pArr->Push(new StringData("a2"));
        pArr->Push(new StringData("a3"));
        data.WriteArr("a", pArr);
        data.Write("b", new UInt16Data(123));
        data.Write("c", new StringData("ccccc"));
        data.Write("d", new UInt32Data(12345));

        unsigned int uLen = 0;
        TEST_ASSERT(m_Eng.Serialize(data, NULL, uLen) == true);
        TEST_ASSERT(uLen == 8+ 2+ 2+2+ 2+2+ 2+2+ 2+ 2+5+ 4);
    }

    void testParse()
    {

    }

private:
    CDataFormatManager m_formatMgr;
    CSerializeEngine m_Eng;
};

