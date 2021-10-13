#pragma once

#define _USE_MATH_DEFINES

#include "Log.h"
#include "Constants.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <iostream>
#include <cassert>
#include <memory>
#include <utility>
#include <algorithm>
#include <numeric>
#include <functional>
#include <sstream>
#include <chrono>
#include <math.h>

#include <queue>
#include <array>
#include <valarray>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <boost/range/irange.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>

#include <magic_enum.hpp>

template <typename D, typename B>
std::unique_ptr<D> static_cast_ptr(std::unique_ptr<B>& base)
{
    return std::unique_ptr<D>(static_cast<D*>(base.release()));
}
 
template <typename D, typename B>
std::unique_ptr<D> static_cast_ptr(std::unique_ptr<B>&& base)
{
    return std::unique_ptr<D>(static_cast<D*>(base.release()));
}

// For summing all of a vector's elements
template <typename T>
T sum_vector(const std::vector<T>& vector)
{
    return std::accumulate(vector.begin(), vector.end(), 0);
}

// For summing a vector's elements until a certain index
template <typename T>
T sum_vector(const std::vector<T>& vector, const size_t& endIndexExclusive)
{
    return std::accumulate(vector.begin(), vector.begin() + endIndexExclusive, 0);
}

// For summing a vector's elements from a certain index until a certain index
template <typename T>
T sum_vector(const std::vector<T>& vector, const size_t& beginIndexInclusive, const size_t& endIndexExclusive)
{
    return std::accumulate(vector.begin() + beginIndexInclusive, vector.begin() + endIndexExclusive, 0);
}

#ifdef RN_PLATFORM_WINDOWS
#include <Windows.hpp>
#endif // RN_PLATFORM_WINDOWS
