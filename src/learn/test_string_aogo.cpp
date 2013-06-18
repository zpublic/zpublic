#include "stdafx.h"
#include "test_string_aogo.h"

void test_case_conv()
{
    std::string sText = "123WSxx你好";
    std::string sT2 = boost::to_lower_copy(sText);
    assert(sText == "123WSxx你好");
    assert(sT2 == "123wsxx你好");
    boost::to_lower(sText);
    assert(sText == "123wsxx你好");
    boost::to_upper(sText);
    assert(sText == "123WSXX你好");
}

struct string_pred
{
    bool operator()(char p)
    {
        if (p>'0'&&p<'9' || p>'a'&&p<'z')
        {
            return true;
        }
        return false;
    }
};
struct string_pred2
{
    bool operator()(char p)
    {
        if (p>'0'&&p<'9')
        {
            return true;
        }
        return false;
    }
};

void test_predicates()
{
    std::string sText = "123WSxx你好";
    assert(boost::starts_with(sText, "123"));
    assert(boost::istarts_with(sText, "123wsXX"));
    assert(boost::ends_with(sText, "123") == false);
    assert(boost::contains(sText, "x你"));
    std::string sText2 = "123sss";
    assert(boost::all(sText2, string_pred()));
    assert(boost::all(sText2, string_pred2()) == false);
}

void test_classification()
{
    std::string sText = "123a";
    std::string sText2 = "123";
    std::string sText3 = "ass";
    assert(all(sText, boost::is_digit()) == false);
    assert(all(sText2, boost::is_digit()));
    assert(all(sText3, boost::is_lower()));
}

void test_trim()
{
    string str1 = "     hello world!     ";
    string str2 = boost::trim_left_copy(str1);
    assert(str2 == "hello world!     ");
    string str3 = boost::trim_right_copy(str1);
    assert(str3 == "     hello world!");
    boost::trim(str1);
    assert(str1 == "hello world!");

    string phone = "00423333444";
    boost::trim_if(phone, boost::is_any_of("04"));
    assert(phone == "23333");
}

void test_find()
{
    char text[] = "hello dolly!";
    boost::iterator_range<char*> result = boost::find_last(text, "ll");
    std::transform( result.begin(), result.end(), result.begin(), std::bind2nd(std::plus<char>(), 1));
    assert(boost::equal(text, "hello dommy!"));
    boost::to_upper(result);
    assert(boost::equal(text, "hello doMMy!"));
    boost::iterator_range<char*> result2 = boost::find_first(text, "doMMy");
    assert(result2.size() == 5);
}

void test_replace()
{

}

void test_find_iterator()
{

}

void test_split()
{
    string str1("hello abc-*-ABC-*-aBc goodbye");
    std::vector<string> SplitVec;
    boost::split(SplitVec, str1, boost::is_any_of("-*"), boost::token_compress_on);
    // SplitVec == { "hello abc","ABC","aBc goodbye" }
    for each (auto var in SplitVec)
    {
        cout<<var<<endl;
    }
}

void test_string_aogo()
{
    test_case_conv();
    test_predicates();
    test_classification();
    test_trim();
    test_find();
    test_replace();
    test_find_iterator();
    test_split();
}

