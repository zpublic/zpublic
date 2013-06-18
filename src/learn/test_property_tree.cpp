#include "stdafx.h"
#include "test_property_tree.h"

using boost::property_tree::ptree;

void test_property_tree_write()
{
    ptree pt;
    pt.put("sec1.data1", 1);
    pt.put("sec1.data2.pp", "12345");
    pt.put("sec2.d1", 100);
    pt.push_back(ptree::value_type("sec3", ptree("54321")));
    ptree ptchild;
    ptchild.put("haha.h1", 300);
    pt.put_child("hahaha", ptchild);

    write_xml("./ptree.xml", pt);
    write_ini("./ptree.ini", ptchild);
    write_json("./ptree.json", pt);
    write_info("./ptree.info", pt);
}

void test_property_tree_read()
{
    ptree pt;
    read_xml("./ptree.xml", pt);
    assert(pt.get<int>("sec1.data1") == 1);
    assert(pt.get<string>("sec1.data2.pp") == "12345");
    pt.clear();
    read_ini("./ptree.ini", pt);
    assert(pt.get<string>("haha.h1") == "300");
    pt.clear();
    read_json("./ptree.json", pt);
    assert(pt.get<int>("hahaha.haha.h1") == 300);
    pt.clear();
    read_info("./ptree.info", pt);
    ptree ptchild = pt.get_child("hahaha.haha");
    assert(ptchild.get<int>("h1") == 300);
}

void test_property_tree()
{
    test_property_tree_write();
    test_property_tree_read();
}

