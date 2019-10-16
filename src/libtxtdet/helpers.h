#ifndef HELPERS_H
#define HELPERS_H

#endif // HELPERS_H

#include <cmath>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <functional>
#include <vector>

template<template<typename, typename> class Container, typename T>
double median(Container<T, std::allocator<T>> v)
{
    size_t nth = v.size()/2;
    std::nth_element(v.begin(), v.begin() + nth, v.end());
    double m = v[nth];
    if(v.size()%2 == 0)
        m = (m + *(std::max_element(v.begin(), v.begin() + nth)))/2;
    return m;
}

template<typename It>
void softmax(It begin, It end)
{
    using T = typename std::iterator_traits<It>::value_type;
    T sum{0};
    std::transform(begin, end, begin, [&](T val){ auto e = std::exp(val); sum += e; return e; });
    std::transform(begin, end, begin, std::bind2nd(std::divides<T>(), sum));
}
