#include "stdafx.h"
#include "test_rapidxml.h"

#include <iterator>

#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_utils.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"
#include "rapidxml-1.13/rapidxml_iterators.hpp"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
using boost::property_tree::ptree;

void create_xml()
{
    ptree pt;
    pt.put("sec1.data1", 1);
    pt.put("sec1.data2.pp", "12345");
    pt.put("sec2.d1", 100);
    pt.push_back(ptree::value_type("sec3", ptree("54321")));
    ptree ptchild;
    ptchild.put("haha.h1", 300);
    ptchild.put("haha2", "hehe");
    pt.put_child("hahaha", ptchild);
    write_xml("./ptree.xml", pt);
}

void test_rapidxml()
{
    create_xml();
    assert(::PathFileExists(L"./ptree.xml"));

    rapidxml::file<wchar_t> xmlFile("./ptree.xml");
    cout<<"FILE size = "<<xmlFile.size()<<endl;
    rapidxml::xml_document<wchar_t> xmlDoc;
    xmlDoc.parse<0>(xmlFile.data());

    ///> 感觉rapidxml的迭代器设计很不科学啊，下列注释的是错误使用
    /*
    rapidxml::node_iterator<wchar_t> nodeIt = xmlDoc.first_node();
    while (nodeIt != xmlDoc.last_node())
    {
        if (wcscmp(nodeIt->name(), L"hahaha") == 0)
        {
            break;
        }
        ++nodeIt;
    }
    }*/

    rapidxml::node_iterator<wchar_t> nullIt;
    rapidxml::node_iterator<wchar_t> nodeIt((rapidxml::xml_node<wchar_t> *)(&xmlDoc));
    while (nodeIt != nullIt)
    {
        if (wcscmp(nodeIt->name(), L"hahaha") == 0)
        {
            break;
        }
        ++nodeIt;
    }
    assert(nodeIt != nullIt);

    assert(rapidxml::count_children(&(*nodeIt)) == 2);
    assert(rapidxml::count_attributes(&(*nodeIt)) == 0);

    std::wstring strKey(L"qq2");
    nodeIt->append_node(xmlDoc.allocate_node(rapidxml::node_element, L"pp", L"ppv"));
    nodeIt->append_attribute(xmlDoc.allocate_attribute(L"qq", L"qqv"));
    // nodeIt->append_attribute(xmlDoc.allocate_attribute(strKey.c_str(), L"qqv2")); err!
    nodeIt->append_attribute(xmlDoc.allocate_attribute(xmlDoc.allocate_string(strKey.c_str()), L"qqv2"));
    assert(rapidxml::count_children(&(*nodeIt)) == 3);
    assert(rapidxml::count_attributes(&(*nodeIt)) == 2);

    std::wstring text;
    rapidxml::print(std::back_inserter(text), xmlDoc);  
    wcout<<text<<endl; 


    std::ostream_iterator<wchar_t, wchar_t> output(std::wcout, L"");
    rapidxml::print(std::ostream_iterator<wchar_t, wchar_t>(output), xmlDoc);  

    //rapidxml::print<wchar_t>(wcout, xmlDoc);

    //rapidxml::print(wcout, xmlDoc);

    //wcout<<xmlDoc<<endl;

    ::DeleteFile(L"./ptree.xml");
    assert(!::PathFileExists(L"./ptree.xml"));
}
