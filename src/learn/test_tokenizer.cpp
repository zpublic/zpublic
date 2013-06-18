#include "stdafx.h"
#include "test_tokenizer.h"

using boost::tokenizer;
using boost::char_separator;
using boost::escaped_list_separator;
using boost::offset_separator;

void test_tokenizer_char_delimiters_separator()
{
    string s = "This is,  a test";
    tokenizer<> tok(s);
    for each (auto item in tok)
    {
        cout << item << endl;
    }
}

void test_tokenizer_char_separator()
{
    string s = "This is,  a test";
    tokenizer<char_separator<char>> tok(s);
    for each (auto item in tok)
    {
        cout << item << endl;
    }
    tok.assign(s, char_separator<char>(" ,"));
    for each (auto item in tok)
    {
        cout << item << endl;
    }
    tok.assign(s, char_separator<char>(" ,", "i", boost::keep_empty_tokens));
    for each (auto item in tok)
    {
        cout << item << endl;
    }
}

void test_tokenizer_escaped_list_separator()
{
    ///> CSV格式，即 , 分隔（默认转义为 \  分隔为 ， 引号为 "）
    string s = "Field 1,\"putting quotes around fields, allows commas\",Field 3";
    tokenizer<escaped_list_separator<char> > tok(s);
    for each (auto item in tok)
    {
        cout << item << endl;
    }
}

void test_tokenizer_offset_separator()
{
    string s = "1225200112345";
    int offsets[] = {2,2,4};
    offset_separator f(offsets, offsets+3);
    tokenizer<offset_separator> tok(s,f);
    for each (auto item in tok)
    {
        cout << item << endl;
    }
}

void test_tokenizer()
{
    test_tokenizer_char_delimiters_separator();
    test_tokenizer_char_separator();
    test_tokenizer_escaped_list_separator();
    test_tokenizer_offset_separator();
}
