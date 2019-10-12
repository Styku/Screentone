#ifndef HELPERS_H
#define HELPERS_H

#endif // HELPERS_H

#include <algorithm>
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
