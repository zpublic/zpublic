#include "stdafx.h"
#include "test_regex.h"

//     typedef basic_regex<char> regex;
//     typedef basic_regex<wchar_t> wregex;
//     typedef match_results<const char *> cmatch;
//     typedef match_results<const wchar_t *> wcmatch;
//     typedef match_results<string::const_iterator> smatch;
//     typedef match_results<wstring::const_iterator> wsmatch;
//     typedef sub_match<const char *> csub_match;
//     typedef sub_match<const wchar_t *> wcsub_match;
//     typedef sub_match<string::const_iterator> ssub_match;
//     typedef sub_match<wstring::const_iterator> wssub_match;
//     typedef regex_token_iterator<const char *> cregex_token_iterator;
//     typedef regex_token_iterator<const wchar_t *> wcregex_token_iterator;
//     typedef regex_token_iterator<string::const_iterator> sregex_token_iterator;
//     typedef regex_token_iterator<wstring::const_iterator> wsregex_token_iterator;

bool is_email_valid(const std::string& email)
{
    const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    return std::regex_match(email, pattern);
}

void show_ip_parts(const std::string& ip)
{
    ///> 通过()来分字串
    const std::regex pattern("(\\d{1,2}|1\\d{2}|20\\d|21\\d|22\\d|23\\d|24\\d|250|251|252|253|254|255)\\.(\\d{1,2}|1\\d{2}|20\\d|21\\d|22\\d|23\\d|24\\d|250|251|252|253|254|255)\\.(\\d{1,2}|1\\d{2}|20\\d|21\\d|22\\d|23\\d|24\\d|250|251|252|253|254|255)\\.(\\d{1,2}|1\\d{2}|20\\d|21\\d|22\\d|23\\d|24\\d|250|251|252|253|254|255)");
    std:: match_results<std::string::const_iterator> result;
    bool valid = std:: regex_match(ip, result, pattern);
    if(valid)
    {
        std::cout << "b1: " << result[1] << std::endl;
        std::cout << "b2: " << result[2] << std::endl;
        std::cout << "b3: " << result[3] << std::endl;
        std::cout << "b4: " << result[4] << std::endl;
    }
}

void test_regex_match()
{
    std::string email1 = "marius.bancila@domain.com";
    std::string email2 = "mariusbancila@domain.com";
    std::string email3 = "marius_b@domain.co.uk";
    std::string email4 = "marius@domain";
    std::string email5 = "12345@qq.com";
    assert(is_email_valid(email1));
    assert(is_email_valid(email2));
    assert(is_email_valid(email3));
    assert(is_email_valid(email4) == false);
    assert(is_email_valid(email5));

    show_ip_parts("1.22.33.255");
    show_ip_parts("1.22.33.256");
    show_ip_parts("100.200");
}

void test_regex_search()
{
    ///> 只能得到一个
    const std::regex pattern("(\\w+day)");
    std::string weekend = "Saturday and Sunday";
    std::smatch result;
    bool match = std::regex_search(weekend, result, pattern);
    if(match)
    {
        for(size_t i = 1; i < result.size(); ++i)
        {
            std::cout << result[i] << std::endl;
        }
    }
    std::cout<<std::endl;
}

void test_regex_iterator()
{
    const std::regex pattern("\\w+day");
    std::string weekend = "Saturday and Sunday, but some Fridays also.";
    const std::sregex_token_iterator end;
    for (std::sregex_token_iterator i(weekend.begin(),weekend.end(), pattern); i != end; ++i)
    {
        std::cout << i->str() << std::endl;
    }
    std::cout<<std::endl;
}

std::string format_date(const std::string& date)
{
    const std::regex pattern("(\\d{1,2})(\\.|-|/)(\\d{1,2})(\\.|-|/)(\\d{4})");
    std::string replacer = "$5$4$3$2$1";
    return std::regex_replace(date, pattern, replacer);
}

void test_regex_replace()
{
    std::string text = "This is a element and this a unique ID.";
    const std::regex pattern("(\\ba (a|e|i|u|o))+");
    std::string replace = "an $2";
    std::string newtext = std::regex_replace(text, pattern, replace);
    std::cout << newtext << std::endl;
    std::cout << std::endl;

    std::string date1 = "1/2/2008";
    std::string date2 = "12.08.2008";
    std::cout << date1 << " -> " << format_date(date1) << std::endl;
    std::cout << date2 << " -> " << format_date(date2) << std::endl;
    std::cout << std::endl;
}

void test_regex()
{
    test_regex_match();
    test_regex_search();
    test_regex_iterator();
    test_regex_replace();
}
