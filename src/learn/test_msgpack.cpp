#include "stdafx.h"
#include "test_msgpack.h"
#include "msgpack\msgpack.hpp"

void test_msgpack_base()
{
    std::vector<std::string> vec;
    vec.push_back("Hello");
    vec.push_back("MessagePack");

    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, vec);

    msgpack::unpacked msg;
    msgpack::unpack(&msg, sbuf.data(), sbuf.size());

    msgpack::object obj = msg.get();
    std::cout << obj << std::endl;

    std::vector<std::string> rvec;
    obj.convert(&rvec);
}

void test_msgpack_stream()
{
    msgpack::sbuffer buffer;
    msgpack::packer<msgpack::sbuffer> pk(&buffer);
    pk.pack(std::string("Log message ... 1"));
    pk.pack(std::string("Log message ... 2"));
    pk.pack(std::string("Log message ... 3"));

    msgpack::unpacker pac;
    pac.reserve_buffer(buffer.size());
    memcpy(pac.buffer(), buffer.data(), buffer.size());
    pac.buffer_consumed(buffer.size());

    msgpack::unpacked result;
    while (pac.next(&result))
    {
        std::cout << result.get() << std::endl;
    }
}

void test_msgpack_stream2()
{
    msgpack::sbuffer buffer;
    msgpack::packer<msgpack::sbuffer> pk(&buffer);
    pk.pack_array(3);
    pk.pack(std::string("Log message ... 1"));
    pk.pack(std::string("Log message ... 2"));
    pk.pack(std::string("Log message ... 3"));

    msgpack::sbuffer buffer2;
    msgpack::packer<msgpack::sbuffer> pk2(&buffer2);
    pk2.pack_map(2);
    pk2.pack(std::string("x"));
    pk2.pack(3);
    pk2.pack(std::string("y"));
    pk2.pack(3.4321);
}

class msg_class
{
public:
    MSGPACK_DEFINE(m_str, m_vec);

public:
    std::string         m_str;
    std::vector<int>    m_vec;
};

void test_msgpack_class()
{
    msg_class m1 = { "111", { 1, 2, 3 } };
    msg_class m2 = { "222", { 9, 8, 7 } };
    std::vector<msg_class> vec = { m1, m2 };

    msgpack::sbuffer sbuf;
    msgpack::pack(sbuf, vec);

    msgpack::unpacked msg;
    msgpack::unpack(&msg, sbuf.data(), sbuf.size());

    msgpack::object obj = msg.get();
    std::vector<msg_class> rvec;
    obj.convert(&rvec);

    std::cout << rvec[0].m_str << std::endl;
    std::cout << rvec[1].m_vec[2] << std::endl;
}

void test_msgpack()
{
    test_msgpack_base();
    test_msgpack_stream();
    test_msgpack_stream2();
    test_msgpack_class();
}
