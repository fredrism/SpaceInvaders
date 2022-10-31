#pragma once
template <typename T>
concept Arithmetic = std::integral<T> || std::floating_point<T>;