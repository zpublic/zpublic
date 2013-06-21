#include "stdafx.h"
#include "test_protobuf.h"
#include "test_proto.pb.h"

void test_proto_serialize(string& s)
{
    test_proto::TestAllTypes test;
    test.set_optional_int32(11);
    test.set_optional_float(1.1f);
    test.set_optional_string("12345");
    test.add_repeated_str("321");
    test.add_repeated_str("322");
    test.add_repeated_str("323");
    test.set_optional_nested_message(test_proto::TestAllTypes_NestedEnum::TestAllTypes_NestedEnum_BAZ);
    test.mutable_nmsg()->set_bb(22);
    assert (test.SerializeToString(&s));
}

void test_proto_deserialize(const string& s)
{
    test_proto::TestAllTypes test;
    assert(test.ParseFromString(s));
    assert(test.optional_int32() == 11);
    assert(test.optional_float() == 1.1f);
    assert(test.optional_uint32() == 123);
    assert(test.optional_string() == "12345");
    auto arr = test.repeated_str();
    assert(arr.size() == 3);
    assert(arr.Get(0) == "321");
    assert(test.repeated_str(1) == "322");
    assert(test.optional_nested_message() == test_proto::TestAllTypes_NestedEnum::TestAllTypes_NestedEnum_BAZ);
    auto nmsg = test.nmsg();
    assert(nmsg.bb() == 22);
}

void test_protobuf()
{
    string s;
    test_proto_serialize(s);
    test_proto_deserialize(s);
}
