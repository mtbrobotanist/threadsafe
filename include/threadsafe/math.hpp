//
// Created by n8 on 3/10/18.
//

#ifndef THREADSAFE_CPP_MATH_H
#define THREADSAFE_CPP_MATH_H

#include <algorithm>

namespace ts
{
    template<typename T>
    T clamp(const T& v, const T& min, const T& max)
    {
        return std::min(std::max(v, min), max);
    }
}

#endif //THREADSAFE_CPP_MATH_H
