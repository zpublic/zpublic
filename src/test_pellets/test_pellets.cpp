// test_pellets.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <fstream>
#include "TestDataBox.h"
#include "TestThreadSync.h"
#include "TestSerialize.h"
#include "TestPeutils.h"
#include "TestThreadPool.h"
#include "TestDesignPattern.h"
#include "TestEvent.h"
#include "TestTimer.h"
#include "TestLuaBind.h"
#include "z_win_utils\TestWinUtils.h"
#include "z_http\TestHttp.h"
#include "TestHarold.h"
#include "z_platform\TestPlatform.h"

void AddTest(Suite& ts)
{
//     ts.add(std::auto_ptr<Suite>(new CTestDataBox));
//     ts.add(std::auto_ptr<Suite>(new CTestThreadSync));
//     ts.add(std::auto_ptr<Suite>(new CTestSerialize));
//     ts.add(std::auto_ptr<Suite>(new CTestPeutils));
//     ts.add(std::auto_ptr<Suite>(new CTestThreadPool));
//     ts.add(std::auto_ptr<Suite>(new CTestDesignPattern));
//     ts.add(std::auto_ptr<Suite>(new CTestEvent));
//     ts.add(std::auto_ptr<Suite>(new CTestTimer));
//     ts.add(std::auto_ptr<Suite>(new CTestLuaBind));
//    ts.add(std::auto_ptr<Suite>(new CTestWinUtils));
//    ts.add(std::auto_ptr<Suite>(new CTestHttp));
//    ts.add(std::auto_ptr<Suite>(new CTestHarold));
    ts.add(std::auto_ptr<Suite>(new CTestPlatform));
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

