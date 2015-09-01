#include "stdafx.h"
#include "test_kapok.h"
#include "kapok/Kapok.hpp"
#include "msgpack/msgpack.hpp"

template < class Func >
void time_call(std::string name, Func&& f)
{
    Sleep(100);
    DWORD dwBegin = ::GetTickCount();
    f();
    std::cout << name << " : " << ::GetTickCount() - dwBegin << std::endl;
}

template < class Func >
void run_times(int times, Func&& f)
{
    for (int i = 0; i < times; ++i)
    {
        f();
    }
}

struct Person
{
    int age;
    string name;
    string city;

    META(age, name, city);
    MSGPACK_DEFINE(age, name, city);
};

void Performance_kapok()
{
    time_call("kapok all", []
    {
        Serializer sr;
        DeSerializer dr;
        Person tp = { 18, "bb", "aa" };
        for (size_t i = 0; i < 100000; i++)
        {
            sr.Serialize(tp, "tuple");
            dr.Parse(sr.GetString());
            Person p;
            dr.Deserialize(p, "tuple");
        }
    });
    time_call("kapok all2", []
    {
        Person tp = { 18, "bb", "aa" };
        for (size_t i = 0; i < 100000; i++)
        {
            Serializer sr;
            DeSerializer dr;
            sr.Serialize(tp, "tuple");
            dr.Parse(sr.GetString());
            Person p;
            dr.Deserialize(p, "tuple");
        }
    });

    time_call("kapok Serializer", []
    {
        Serializer sr;
        Person tp = { 18, "bb", "aa" };
        for (size_t i = 0; i < 100000; i++)
        {
            sr.Serialize(tp, "tuple");
        }
    });
    time_call("kapok Serializer2", []
    {
        Person tp = { 18, "bb", "aa" };
        for (size_t i = 0; i < 100000; i++)
        {
            Serializer sr;
            sr.Serialize(tp, "tuple");
        }
    });

    Serializer sr;
    Person tp = { 18, "bb", "aa" };
    sr.Serialize(tp, "tuple");
    time_call("kapok DeSerializer", [&]
    {
        DeSerializer dr;
        for (size_t i = 0; i < 100000; i++)
        {
            dr.Parse(sr.GetString());
            Person p;
            dr.Deserialize(p, "tuple");
        }
    });
    time_call("kapok DeSerializer", [&]
    {
        for (size_t i = 0; i < 100000; i++)
        {
            DeSerializer dr;
            dr.Parse(sr.GetString());
            Person p;
            dr.Deserialize(p, "tuple");
        }
    });
}

void Performance_msgpack()
{
    Person src = { 18, "bb", "aa" };
    Person dst;

    time_call("msgpack all", [&]
    {
        for (size_t i = 0; i < 100000; i++)
        {
            std::stringstream buffer;
            msgpack::pack(buffer, src);
            buffer.seekg(0);
            std::string str(buffer.str());
            msgpack::unpacked result;
            msgpack::unpack(result, str.data(), str.size());
            msgpack::object deserialized = result.get();
            deserialized.convert(&dst);
        }
    });
}

void Performance_cmp()
{
    time_call("kapok all", []
    {
        Serializer sr;
        DeSerializer dr;
        Person tp = { 18, "bb", "aa" };
        Person p;
        for (size_t i = 0; i < 200000; i++)
        {
            sr.Serialize(tp, "tuple");
            dr.Parse(sr.GetString());
            dr.Deserialize(p, "tuple");
        }
    });
    time_call("kapok all2", []
    {
        for (size_t i = 0; i < 200000; i++)
        {
            Person tp = { 18, "bb", "aa" };
            Serializer sr;
            DeSerializer dr;
            sr.Serialize(tp, "tuple");
            dr.Parse(sr.GetString());
            Person p;
            dr.Deserialize(p, "tuple");
        }
    });
    time_call("msgpack all", []
    {
        Person src = { 18, "bb", "aa" };
        Person dst;
        msgpack::unpacked result;
        for (size_t i = 0; i < 200000; i++)
        {
            msgpack::sbuffer buffer;
            msgpack::pack(buffer, src);
            msgpack::unpack(result, buffer.data(), buffer.size());
            result.get().convert(&dst);
        }
    });
    time_call("msgpack all2", []
    {
        for (size_t i = 0; i < 200000; i++)
        {
            Person src = { 18, "bb", "aa" };
            std::stringstream buffer;
            msgpack::pack(buffer, src);
            buffer.seekg(0);
            std::string str(buffer.str());
            msgpack::unpacked result;
            msgpack::unpack(result, str.data(), str.size());
            msgpack::object deserialized = result.get();
            Person dst;
            deserialized.convert(&dst);
        }
    });
}

void test_kapok()
{
    //Performance_kapok();
    //Performance_msgpack();
    run_times(5, []
    {
        Performance_cmp();
    });
}
