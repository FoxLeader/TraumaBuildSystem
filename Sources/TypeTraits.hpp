#pragma once

#include <cstddef>



template <size_t> class String;
template <typename> class DynamicArray;



namespace TypeTraits
{
    namespace Implementation
    {
        // - IsStringLiteral
        template <typename T>
        struct IsStringLiteral                                  { static constexpr bool Value = false; };
        template <size_t StringSize>
        struct IsStringLiteral<const char (&)[StringSize]>      { static constexpr bool Value = true; };
        template <>
        struct IsStringLiteral<const char* const>               { static constexpr bool Value = true; };
        template <>
        struct IsStringLiteral<char* const&>                    { static constexpr bool Value = true; };

        // - IsSizedStringLiteral
        template <typename T>
        struct IsSizedStringLiteral                             { static constexpr bool Value = false; };
        template <size_t StringSize>
        struct IsSizedStringLiteral<const char (&)[StringSize]> { static constexpr bool Value = true; };

        // - IsString
        template <typename T>
        struct IsString                                         { static constexpr bool Value = false; };
        template <size_t StringSize>
        struct IsString<const String<StringSize>&>              { static constexpr bool Value = true; };
        template <size_t StringSize>
        struct IsString<String<StringSize>&>                    { static constexpr bool Value = true; };

        // - IsArray
        template <typename T>
        struct IsArray                                          { static constexpr bool Value = false; };
        template <typename T>
        struct IsArray<const DynamicArray<T>&>                  { static constexpr bool Value = false; };
    }

    template <typename T>
    inline constexpr bool IsStringLiteral                       = Implementation::IsStringLiteral<T>::Value;
    template <typename T>
    inline constexpr bool IsSizedStringLiteral                  = Implementation::IsSizedStringLiteral<T>::Value;
    template <typename T>
    inline constexpr bool IsString                              = Implementation::IsString<T>::Value;
    template <typename T>
    inline constexpr bool IsArray                               = Implementation::IsArray<T>::Value;
}
