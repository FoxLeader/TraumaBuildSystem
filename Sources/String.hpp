#pragma once

#include <cstddef>

#define StaticString constexpr String



inline constexpr size_t InvalidStringIndex = static_cast<size_t>(-1);



template <size_t MaxSize>
class String
{
    // ============================================================ Constructors / Destructors / Operators

    public:

    constexpr String() = default;
    constexpr String(const char (&string)[MaxSize]) { copy(string); }

    template <size_t StringSize>
    constexpr String(const char (&string)[StringSize]) { copy(string); }

    template <size_t StringSize>
    constexpr String(const String<StringSize>& string) { copy(string); }

    template <size_t StringSize>
    constexpr String&               operator=(const String<StringSize>& string) { copy(string); return *this; }
    constexpr String&               operator=(const char* const string) { copy(string); return *this; }

    constexpr char&                 operator[](size_t index) { return mString[index]; }
    constexpr const char&           operator[](size_t index) const { return mString[index]; }

    constexpr operator              const char*() const { return mString; }

    // ============================================================ Functions

    public:

    template <size_t StringSize>
    constexpr void                  append(const String<StringSize>& string, size_t stringLength = InvalidStringIndex, size_t stringOffset = 0) { copy(string.c_str(), length(), stringLength, stringOffset); }
    constexpr void                  append(const char* string, size_t stringLength = InvalidStringIndex, size_t stringOffset = 0) { copy(string, length(), stringLength, stringOffset); }

    constexpr char*                 data() { return mString; }
    constexpr const char*           c_str() const { return mString; }
    constexpr size_t                max_size() const { return MaxSize; }
    constexpr bool                  is_empty() const { return mString[0] == '\0'; }

    constexpr size_t                find_first_of_chars(const auto& string) const
    {
        size_t stringLength = Length(string);
        size_t index = InvalidStringIndex;
        const char* p = string;

        for (size_t i = 0; i < MaxSize && mString[i] != '\0' && index == InvalidStringIndex; i++)
            for (size_t k = 0; k < stringLength; k++)
                if (mString[i] == p[k])
                {
                    index = i;
                    break;
                }

        return index;
    }

    constexpr size_t                find_last_of_chars(const auto& string) const
    {
        size_t stringLength = Length(string);
        size_t myLength = length();
        const char* p = string;
        size_t index = InvalidStringIndex;
        for (size_t i = myLength - 1; i > 0 && index == InvalidStringIndex; i--)
            for (size_t k = 0; k < stringLength; k++)
                if (mString[i] == p[k])
                {
                    index = i;
                    break;
                }
        return index;
    }

    constexpr void                  copy(const char* const string, size_t offset = 0, size_t stringLength = InvalidStringIndex, size_t stringOffset = 0)
    {
        const char* p = string + stringOffset;
        if (stringLength == InvalidStringIndex)
            stringLength = Length(string);
        size_t c = 0;
        while (p[c] != '\0' && c + offset < MaxSize - 1 && c + stringOffset < stringLength && c < stringLength)
        {
            mString[c + offset] = p[c];
            c++;
        }
        mString[c + offset] = '\0';
    }

    constexpr size_t                length() const { return Length(mString); }

    private:

    static constexpr size_t         Length(const char* const string)
    {
        const char* p = string;
        size_t c = 0;
        while (p[c] != '\0') c++;
        return c;
    }

    // ============================================================ Data

    private:

    char                            mString[MaxSize]                = {};
};

template <size_t aSize, size_t bSize>
constexpr auto operator +(const String<aSize>& a, const String<bSize>& b)
{
    String<aSize + bSize - 1> newString;
    newString.append(a);
    newString.append(b);
    return newString;
}

template <size_t aSize, size_t bSize>
constexpr auto operator +(const String<aSize>& a, const char (&b)[bSize])
{
    String<aSize + bSize - 1> newString;
    newString.append(a);
    newString.append(b);
    return newString;
}

template <size_t aSize, size_t bSize>
constexpr auto operator +(const char (&a)[aSize], const String<bSize>& b)
{
    String<aSize + bSize - 1> newString;
    newString.append(a);
    newString.append(b);
    return newString;
}

template <size_t aSize, size_t bSize>
constexpr auto operator *(const String<aSize>& a, const String<bSize>& b)
{
    String<aSize + bSize> newString;
    newString.append(a);
    newString.append(" ");
    newString.append(b);
    return newString;
}

template <size_t aSize, size_t bSize>
constexpr auto operator *(const String<aSize>& a, const char (&b)[bSize])
{
    String<aSize + bSize> newString;
    newString.append(a);
    newString.append(" ");
    newString.append(b);
    return newString;
}

template <size_t aSize, size_t bSize>
constexpr auto operator *(const char (&a)[aSize], const String<bSize>& b)
{
    String<aSize + bSize> newString;
    newString.append(a);
    newString.append(" ");
    newString.append(b);
    return newString;
}

template <size_t aSize, size_t bSize>
constexpr auto operator /(const String<aSize>& a, const String<bSize>& b)
{
    String<aSize + bSize> newString;
    newString.append(a);
    newString.append("/");
    newString.append(b);
    return newString;
}

template <size_t aSize, size_t bSize>
constexpr auto operator /(const String<aSize>& a, const char (&b)[bSize])
{
    String<aSize + bSize> newString;
    newString.append(a);
    newString.append("/");
    newString.append(b);
    return newString;
}
