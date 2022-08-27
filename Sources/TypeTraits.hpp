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

using size_t = unsigned long long; static_assert(sizeof(size_t) == 8);

namespace TraumaBuildSystem
{
    template <size_t> class String;

    namespace TypeTraits
    {
        namespace Implementation
        {
            namespace Helpers
            {
                // - RemoveConstAndVolatile
                template <typename T>
                struct RemoveConstAndVolatile                       { using type = T; };
                template <typename T>
                struct RemoveConstAndVolatile<const T>              { using type = T; };
                template <typename T>
                struct RemoveConstAndVolatile<volatile T>           { using type = T; };
                template <typename T>
                struct RemoveConstAndVolatile<const volatile T>     { using type = T; };

                // - RemovePointer
                template <typename T>
                struct RemovePointer                                { using type = T; };
                template <typename T>
                struct RemovePointer<T*>                            { using type = T; };
            }

            template <typename T>
            using RemoveConstAndVolatile                            = typename Helpers::RemoveConstAndVolatile<T>::type;
            template <typename T>
            using RemovePointer                                     = typename Helpers::RemovePointer<T>::type;

            // ============================================================ Traits

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

            // - IsPointer
            template <typename T>
            struct IsPointer                                        { static constexpr bool Value = false; };
            template <typename T>
            struct IsPointer<T*>                                    { static constexpr bool Value = true; };

            // - IsFunction
            template <typename T>
            struct IsFunction                                       { static constexpr bool Value = false; };

            // - IsFunctionPointer
            template <typename T>
            struct IsFunctionPointer                                { static constexpr bool Value = false; };
        }

        using Implementation::RemoveConstAndVolatile;
        using Implementation::RemovePointer;

        template <typename T>
        inline constexpr bool IsStringLiteral                       = Implementation::IsStringLiteral<T>::Value;
        template <typename T>
        inline constexpr bool IsSizedStringLiteral                  = Implementation::IsSizedStringLiteral<T>::Value;
        template <typename T>
        inline constexpr bool IsString                              = Implementation::IsString<T>::Value;
        template <typename T>
        inline constexpr bool IsPointer                             = Implementation::IsPointer<T>::Value;
        template <typename T>
        inline constexpr bool IsFunction                            = Implementation::IsFunction<T>::Value;
        template <typename T>
        inline constexpr bool IsFunctionPointer                     = Implementation::IsFunctionPointer<T>::Value;
    }
}
