#pragma once
#include <vector>

template <typename T>
std::vector<T> LinearSearch(std::vector<T>& arr, T& key)
{
    std::vector<T> result;
    for (auto& element : arr) {
        if (element == key) {
            result.push_back(element);
        }
    }
    return result;
}