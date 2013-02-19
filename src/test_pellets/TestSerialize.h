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
        pArr->Push(new StringData("a33"));
        data.WriteArr("a", pArr);
        data.Write("b", new UInt16Data(123));
        data.Write("c", new StringData("ccccc"));
        data.Write("d", new UInt32Data(12345));

        unsigned int uLen = 0;
        TEST_ASSERT(m_Eng.Serialize(data, NULL, uLen));
        TEST_ASSERT(uLen == 34); ///> 6+ 2+ 2+2+ 2+2+ 2+3+ 2+ 2+5+ 4

        uLen++;
        m_pBuf = new char[uLen];
        memset(m_pBuf, 0, uLen);

        TEST_ASSERT(m_Eng.Serialize(data, m_pBuf, uLen));

        char pBufTest[] =
        {
            34,0, 1,0,0,0, //size | id
            3,0,
            2,0, 'a','1',
            2,0, 'a','2',
            3,0, 'a','3','3',
            123,0,
            5,0, 'c','c','c','c','c',
            57,48,0,0
        };

        for (unsigned int i = 0; i < uLen; ++i)
        {
            TEST_ASSERT(m_pBuf[i] == pBufTest[i]);
        }
    }

    void testParse()
    {
        return;
        CData data;
        TEST_ASSERT(m_Eng.Parse(m_pBuf, 34, data));

        ArrayData* pArr;
        ValueData* pValue;
        TEST_ASSERT(data.ReadArr("a", pArr));
        TEST_ASSERT(pArr->Size() == 3);

        std::string str;
        uint16 u16 = 0;
        uint32 u32 = 0;
        auto it = pArr->Begin();
        TEST_ASSERT((ValueData*)(*it)->ToStr(str));
        TEST_ASSERT(str == "a1");
        TEST_ASSERT(data.Read("c", pValue));
        TEST_ASSERT(pValue->ToStr(str));
        TEST_ASSERT(str == "ccccc");

        TEST_ASSERT(data.Read("b", pValue));
        TEST_ASSERT(pValue->ToUInt16(u16));
        TEST_ASSERT(u16 == 123);

        TEST_ASSERT(data.Read("d", pValue));
        TEST_ASSERT(pValue->ToUInt32(u32));
        TEST_ASSERT(u32 == 12345);

        delete[] m_pBuf;
    }

private:
    CDataFormatManager m_formatMgr;
    CSerializeEngine m_Eng;

    char* m_pBuf;
};

