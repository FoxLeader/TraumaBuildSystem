// ======================================================================================================= //
//      This file is part of Trauma Build System (https://github.com/FoxLeader/TraumaBuildSystem)          //
//      Copyright: PolyTrauma Studios Srls, All Rights Reserved.                                           //
//                                                                                                         //
//      Author: Fabiano Raffaelli                                                                          //
//                                                                                                         //
// ======================================================================================================= //
//      This software is licensed under Creative Commons (CC BY NC 4.0): See LICENSE.md for details.       //
// ======================================================================================================= //

#pragma once

#define StaticString constexpr TraumaBuildSystem::String

using size_t = unsigned long long; static_assert(sizeof(size_t) == 8);



namespace TraumaBuildSystem
{
    inline constexpr size_t InvalidStringIndex = static_cast<size_t>(-1);

    constexpr size_t FindFirstOfSet(const char* const string, const char* const set);
    constexpr size_t FindLastOfSet(const char* const string, const char* const set);
    constexpr size_t Length(const char* const string);

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
        constexpr operator              char*() { return mString; }

        // ============================================================ Functions

        public:

        template <size_t StringSize>
        constexpr void                  append(const String<StringSize>& string, size_t stringLength = InvalidStringIndex, size_t stringOffset = 0) { copy(string.c_str(), Length(mString), stringLength, stringOffset); }
        constexpr void                  append(const char* string, size_t stringLength = InvalidStringIndex, size_t stringOffset = 0) { copy(string, Length(mString), stringLength, stringOffset); }

        constexpr char*                 data() { return mString; }
        constexpr const char*           c_str() const { return mString; }

        constexpr bool                  is_empty() const { return mString[0] == '\0'; }

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

    template <size_t aSize, size_t bSize>
    constexpr auto operator /(const char (&a)[aSize], const String<bSize>& b)
    {
        String<aSize + bSize> newString;
        newString.append(a);
        newString.append("/");
        newString.append(b);
        return newString;
    }



    // inline constexpr size_t FindFirstOfSet(const char* const string, const char* const set)
    // {
    //     size_t stringLength = Length(string);
    //     size_t index = InvalidStringIndex;
    //     const char* p = string;
    //
    //     for (size_t i = 0; i < MaxSize && mString[i] != '\0' && index == InvalidStringIndex; i++)
    //         for (size_t k = 0; k < stringLength; k++)
    //             if (mString[i] == p[k])
    //             {
    //                 index = i;
    //                 break;
    //             }
    //
    //     return index;
    // }



    inline constexpr size_t FindLastOfSet(const char* const string, const char* const set)
    {
        const char* pStr = string + Length(string);
        const char* pSet = set;
        size_t index = InvalidStringIndex;

        while (string <= pStr && index == InvalidStringIndex)
        {
            while (*pSet != '\0')
            {
                if (*pStr == *pSet)
                {
                    index = static_cast<size_t>(pStr - string);
                    break;
                }
                pSet++;
            }
            pSet = set;
            pStr--;
        }

        return index;
    }



    template <size_t Size> consteval size_t SizeOf(const String<Size>&)     { return Size; }
    template <size_t Size> consteval size_t SizeOf(const char (&)[Size])    { return Size; }



    inline constexpr size_t Length(const char* const string)
    {
        size_t c = 0;
        while (string[c] != '\0')
            c++;
        return c;
    }
}
