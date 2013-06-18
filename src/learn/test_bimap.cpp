#include "stdafx.h"
#include "test_bimap.h"

using boost::bimap;

void test_bimap()
{
    bimap<int, string> bm;
    bm.insert( bimap<int, string>::value_type(1, "one" ) );
    bm.insert( bimap<int, string>::value_type(2, "two" ) );
    assert(bm.size() == 2);
    bm.insert( bimap<int, string>::value_type(3, "one" ) );
    bm.insert( bimap<int, string>::value_type(2, "t" ) );
    assert(bm.size() == 2);
    for each (auto var in bm)
    {
        cout<<var.left<<","<<var.right<<endl;
    }
    for each (auto var in bm.left)
    {
        cout<<var.first<<","<<var.second<<endl;
    }
    auto itFind = bm.left.find(2);
    assert(itFind != bm.left.end());
    assert(itFind->second == "two");
}
