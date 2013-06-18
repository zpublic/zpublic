#include "stdafx.h"
#include "test_dynamic_bitset.h"

using boost::dynamic_bitset;

void test_dynamic_bitset()
{
    dynamic_bitset<> db;
    db.push_back(true);
    db.push_back(false);
    assert(db.size() == 2);
    assert(db.to_ulong() == 1);
    db.flip();
    assert(db.to_ulong() == 2);

    dynamic_bitset<> db2(5, 888);
    cout<<boost::lexical_cast<int>(db2)<<endl;
}
