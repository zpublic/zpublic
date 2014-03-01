// test_pellets.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <fstream>
#include "TestDataBox.h"
#include "TestThreadSync.h"
#include "TestSerialize.h"
#include "TestPeutils.h"

void AddTest(Suite& ts)
{
    ts.add(std::auto_ptr<Suite>(new CTestDataBox));
    ts.add(std::auto_ptr<Suite>(new CTestThreadSync));
    ts.add(std::auto_ptr<Suite>(new CTestSerialize));
    ts.add(std::auto_ptr<Suite>(new CTestPeutils));
}

void UniTest()
{
    Suite ts;
    AddTest(ts);
    std::auto_ptr<Output> output(new XmlOutput);
    ts.run(*output, true);
    Test::XmlOutput* const xml_output = dynamic_cast<XmlOutput*>(output.get());
    if (xml_output)
    {
        std::ofstream fout("./test_pellets.xml");
        xml_output->generate(fout, true, "zpublic");
    }
}

int _tmain(int argc, _TCHAR* argv[])
{
    UniTest();
    return 0;
}

