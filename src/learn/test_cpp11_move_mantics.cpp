#include "stdafx.h"
#include "test_cpp11_move_mantics.h"
#include <algorithm>

class vector_no_move
{
public:
    vector_no_move()
    {
        int i = 10;
    }

    vector_no_move(const vector_no_move& vec)
    {
        m_v.resize(vec.m_v.size());
        std::copy(vec.m_v.begin(), vec.m_v.end(), m_v.begin());
    }
    vector_no_move &operator = (const vector_no_move& vec)
    {
        m_v.resize(vec.m_v.size());
        std::copy(vec.m_v.begin(), vec.m_v.end(), m_v.begin());
        return *this;
    }
    vector_no_move(vector_no_move&& vec) = delete;
    vector_no_move &operator = (vector_no_move&& vec) = delete;

    void push_back(int i)
    {
        m_v.push_back(i);
    }

private:
    std::vector<int> m_v;
};

class vector_has_move
{
public:
    vector_has_move()
    {
        int i = 10;
    }

    vector_has_move(const vector_has_move& vec)
    {
        m_v.resize(vec.m_v.size());
        std::copy(vec.m_v.begin(), vec.m_v.end(), m_v.begin());
    }
    vector_has_move &operator = (const vector_has_move& vec)
    {
        m_v.resize(vec.m_v.size());
        std::copy(vec.m_v.begin(), vec.m_v.end(), m_v.begin());
        return *this;
    }

    vector_has_move(vector_has_move&& vec)
    {
        std::swap(m_v, vec.m_v);
    }
    vector_has_move &operator = (vector_has_move&& vec)
    {
        std::swap(m_v, vec.m_v);
        return *this;
    }

    void push_back(int i)
    {
        m_v.push_back(i);
    }

private:
    std::vector<int> m_v;
};

// vector_no_move getVec1()
// {
//     vector_no_move v;
//     for (int i = 0; i < 1000; i++)
//     {
//         v.push_back(1);
//     }
//     return v;
// }
// vector_no_move getVec2()
// {
//     vector_no_move v;
//     for (int i = 0; i < 1000; i++)
//     {
//         v.push_back(1);
//     }
//     return std::move(v);
// }
vector_has_move getVec3()
{
    vector_has_move v;
    for (int i = 0; i < 1000; i++)
    {
        v.push_back(1);
    }
    return v;
}
vector_has_move getVec4()
{
    vector_has_move v;
    for (int i = 0; i < 1000; i++)
    {
        v.push_back(1);
    }
    return std::move(v);
}

void test_cpp11_move_mantics()
{
    zl::timer zTimer;
//     zTimer.restart();
//     for (int i = 0; i < 999; i++)
//     {
//         auto v1 = getVec1();
//     }
//     std::cout << zTimer.elapsed() << std::endl;
//     zTimer.restart();
//     for (int i = 0; i < 999; i++)
//     {
//         auto v1 = getVec2();
//     }
//     std::cout << zTimer.elapsed() << std::endl;
    zTimer.restart();
    for (int i = 0; i < 999; i++)
    {
        auto v1 = getVec3();
    }
    std::cout << zTimer.elapsed() << std::endl;
    zTimer.restart();
    for (int i = 0; i < 999; i++)
    {
        auto v1 = getVec4();
    }
    std::cout << zTimer.elapsed() << std::endl;
}
