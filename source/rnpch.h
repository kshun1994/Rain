#pragma once

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

#include <queue>
#include <array>
#include <valarray>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <magic_enum.hpp>

template<typename D, typename B>
std::unique_ptr<D> static_cast_ptr(std::unique_ptr<B>& base)
{
    return std::unique_ptr<D>(static_cast<D*>(base.release()));
}
 
template<typename D, typename B>
std::unique_ptr<D> static_cast_ptr(std::unique_ptr<B>&& base)
{
    return std::unique_ptr<D>(static_cast<D*>(base.release()));
}

#ifdef RN_PLATFORM_WINDOWS
#include <Windows.hpp>
#endif // RN_PLATFORM_WINDOWS
