#include "stdafx.h"
#include "test_rxcpp.h"
#include "rxcpp/Ix/CPP/src/cpplinq/linq.hpp"
#include "rxcpp/Rx/v2/src/rxcpp/rx.hpp"
#include <cctype>
#include <clocale>

void test_ix()
{
    using namespace cpplinq;

    std::vector<int> ints = { 3, 1, 4, 1, 5, 9, 2, 6, 5, 4 };
    auto v = from(ints)
        .where([](int i) {return i % 2 == 0; })
        .to_vector();
    for each (auto var in v)
    {
        cout << var << endl;
    }
}

void test_rx()
{
    namespace rx = rxcpp;
    namespace rxsub = rxcpp::subjects;
    namespace rxu = rxcpp::util;

    auto keys = rx::observable<>::create<int>([](rx::subscriber<int> dest)
    {
        dest.on_next('1');
        dest.on_next('a');
        dest.on_next('x');
        dest.on_next('g');
        dest.on_next('3');
    }).publish();

    auto a = keys.filter([](int key){return std::tolower(key) == 'a'; });

    auto g = keys.filter([](int key){return std::tolower(key) == 'g'; });

    a.merge(g).subscribe([](int key)
    {
        std::cout << key << std::endl;
    });

    keys.connect();
}

void test_rxcpp()
{
    test_ix();
    test_rx();
}
