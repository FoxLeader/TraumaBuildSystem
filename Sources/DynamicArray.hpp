#pragma once

#include <vector>



template <typename T>
class DynamicArray
{
    // ============================================================ Types

    // ============================================================ Constructors / Destructors / Operators

    public:

    DynamicArray() = default;
    ~DynamicArray() = default;

    // ============================================================ Functions

    public:

    auto                            begin() { return mArray.begin(); }
    auto                            end() { return mArray.end(); }

    template <typename ...Args>
    void                            push(Args&&... args) { mArray.emplace_back(std::forward<Args>(args)...); }

    // ============================================================ Data

    private:

    std::vector<T>                  mArray;
};
