#pragma once

/**
 * @file vector_overloads.hpp
 * @author Barak Shoshany (baraksh@gmail.com) (http://baraksh.com)
 * @version Function template example: the vector operator overloads
 * @date 2020-11-30
 * @copyright Copyright (c) 2020
 * @brief A template with overloaded operators for common vector operations.
 * @details This library contains overloaded operators for a vector class. Overloaded operators for common vector operations such as addition and multiplication are defined.
 */

#include <iostream>
#include <vector>

using namespace std;

class err_size_must_match
{
};

template <typename T>
ostream &operator<<(ostream &out, const vector<T> &v)
{
    size_t s{v.size() - 1};
    out << "(";
    for (size_t i{0}; i < s; i++)
        out << v[i] << ", ";
    out << v[s] << ")\n";
    return out;
}

template <typename T>
vector<T> operator+(const vector<T> &v, const vector<T> &w)
{
    size_t s{v.size()};
    if (s != w.size())
        throw err_size_must_match{};
    vector<T> u(s);
    for (size_t i{0}; i < s; i++)
        u[i] = v[i] + w[i];
    return u;
}

template <typename T>
vector<T> operator+=(vector<T> &v, const vector<T> &w)
{
    v = v + w;
    return v;
}

template <typename T>
vector<T> operator-(const vector<T> &v)
{
    size_t s{v.size()};
    vector<T> u(s);
    for (size_t i{0}; i < s; i++)
        u[i] = -v[i];
    return u;
}

template <typename T>
vector<T> operator-(const vector<T> &v, const vector<T> &w)
{
    size_t s{v.size()};
    if (s != w.size())
        throw err_size_must_match{};
    vector<T> u(s);
    for (size_t i{0}; i < s; i++)
        u[i] = v[i] - w[i];
    return u;
}

template <typename T>
vector<T> operator-=(vector<T> &v, const vector<T> &w)
{
    v = v - w;
    return v;
}

template <typename T>
T operator*(const vector<T> &v, const vector<T> &w)
{
    size_t s{v.size()};
    if (s != w.size())
        throw err_size_must_match{};
    T p{0};
    for (size_t i{0}; i < s; i++)
        p += v[i] * w[i];
    return p;
}

template <typename T>
vector<T> operator*(const T &x, const vector<T> &v)
{
    size_t s{v.size()};
    vector<T> u(s);
    for (size_t i{0}; i < s; i++)
        u[i] = x * v[i];
    return u;
}

template <typename T>
vector<T> operator*(const vector<T> &v, const T &x)
{
    return x * v;
}