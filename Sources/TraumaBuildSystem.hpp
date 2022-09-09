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
#define TBS_InjectFile
#define BUILD_STEPS() extern "C" void BuildSteps()
#define TRAUMA_BUILD_SYSTEM(ver) \
    using namespace TraumaBuildSystem::ver; \
    using TraumaBuildSystem::String;

using size_t = unsigned long long;      static_assert(sizeof(size_t) == 8);
using uint16 = unsigned short;          static_assert(sizeof(uint16) == 2);

// You can skip this part -> //////////////////////////////
namespace TraumaBuildSystem
{
    struct FileData;
    template <size_t> class String;
}
// You can skip this part <- //////////////////////////////



/*  The current plan is to keep new functionality in the Experimental subspace,
    pulling them out once their function signature will be considered stable, this means that
    Build Scripts written against an Experimental API may break frequently.

    When writing Build Scripts against a stable API, it is reccomended to use the
    latest version available, but you will not be forced to update your old scripts
    to the newest version if you don't think it's necessary.
*/

namespace TraumaBuildSystem::v1::Experimental
{
    // - As* functions manipulate input according to the function called. All As* functions return a String.
    // Compiler Options Helpers.
    constexpr auto                  AsInclude(const auto& path);                                        // Prefix path so Use this path when resolving includes.
    constexpr auto                  AsSystemInclude(const auto& path);                                  // Use this path when resolving includes, but threat them like system headers. (Suppresses Warnings)
    constexpr auto                  AsLibrary(const auto& library);                                     // Use this library during Linking.
    constexpr auto                  AsLibraryPath(const auto& path);                                    // Use this path when resolving libraries.
    constexpr auto                  AsDefine(const auto& name);                                         // Define a macro, same as #define name in a source file.
    // Helpers for Path Arguments in Call().
    constexpr auto                  AsPath(const auto& path);                                           // Makes sure that "path" respects platform conventions. (Forward/Backward slashes, double quotes for paths containing empty spaces...)
    auto                            AsAbsolutePath(const auto& path);                                   // like AsPath() but also makes "path" absolute.
    auto                            AsRelativePath(const auto& path);                                   // like AsPath() but also makes "path" relative to the current working directory, if possible.

    // - Path Manipulation.
    constexpr auto                  StripExtension(const auto& path);                                   // Returns path with the extension elided. The returned value matches path if path has no extension suffix.
    constexpr auto                  StripPath(const auto& path);                                        // Returns the file name only. Returns an empty String if path is not valid or if ending with '/'.
    constexpr auto                  StripFileName(const auto& path);                                    // Returns the directory path only. Returns an empty is path is not valid or if there are no directories before the file name.
    constexpr auto                  ExtensionOf(const auto& path);

    // - Path Validation.
    bool                            Exists(const auto& path);                                           // Returns true if Path exists.
    bool                            NotExists(const auto& path);                                        // Return true if Path does NOT exist.
    constexpr bool                  IsValidPath(const auto& path);                                      // TODO: Currently always returns true.
    constexpr bool                  IsAbsolutePath(const auto& path);                                   // TODO: Currently always returns false.
    constexpr bool                  IsRelativePath(const auto& path);                                   // TODO: Currently always returns false.

    // - Directory Operations.
    bool                            CreateDirectory(const auto& path);                                  // Creates a directory, including the intermediates if needed. Returns true on success.
    bool                            DeleteDirectory(const auto& path);                                  // Deletes a directory and all its content recursively. Returns true on success.

    auto                            CurrentWorkingDirectory();                                          // Returns a String of the Current Working Directory.
    bool                            CurrentWorkingDirectory(const auto& path);                          // Sets the Current Working Directory to the new Path. Returns true on success.

    // - File Operations.
    bool                            DeleteFile(const auto& filename);                                   // Deletes a single file. Returns True on success.
    bool                            CopyFile(const auto& fromPath, const auto& toPath);                 // Copies a single file. Returns True on success.
    void                            ForEachFile(const auto& path, auto&& fn);                           // Executes function fn for each file in path. Path can contain Wildcards files will be filtered accordingly. (Ex: MyPath/*.txt)
    FileData                        ReadFile(const auto& filename);                                     // Reads an entire file into a buffer and returns a char* handle and its size in a FileData struct. On Error, the buffer is set to nullptr. IT IS THE USER'S RESPONSIBILITY TO FREE() THE BUFFER HANDLE.

    // - Launch External Programs. THIS CURRENTLY USES system() WHICH IS NOTORIOUSLY UNSAFE.
    void                            Call(const auto& cmd);                                              // Executes cmd.
    void                            Call(const auto& cmd, auto& output);                                // Executes cmd and captures the output.

    // - Console Functionality.
    void                            ClearConsole();                                                     // This is basicaly a shortcut for Call("cls");
    void                            Print(const auto& message, ...);                                    // Works like printf().
    void                            Println(const auto& message, ...);                                  // Works like printf(), but appends a newline.
    void                            Println();                                                          // Prints a newline. (AKA printf("\n"))

    // - Automations for Call(), not very useful for now.
    auto                            Compile(const auto &sourceFile, const auto& compilerFlags, const auto& includes);
    bool                            Build(const auto& artifact, const auto& source, const auto& compilerFlags, const auto& linkerFlags, const auto& includes, const auto& libsPath, const auto& libs);

    // TODO: Change this so that extensions can be specified when compiling TBS. Code for enabled extensions will be injected here using InjectFile.
    // - SVN Extension.
    namespace SVN
    {
        auto                        CurrentRevision();                                                  // Returns a String with the Working Copy's Revision Number of the Current Working Directory, if any.
        auto                        CurrentRevision(const auto& path);                                  // Returns a String with the Working Copy's Revision Number of Path, if any.
    }

    // - Itch.io Extension.
    namespace Itch
    {
        // TODO: Butler Stuff.
    }

    // - FL Studio Extension.
    namespace FLStudio
    {
        // TODO.
    }

    // - SoX Extension.
    namespace SoX
    {
        // TODO.
    }
}

TBS_InjectFile
#include "String.hpp"

// ====================================================================Current
// ------------------------------ IMPLEMENTATION ------------------------------
// ============================================================================
// Anything below this point is internal only.

// TODO: Remove Dependencies from CRT
#include <cstdio>
#include <cstring>
#include <cassert>

TBS_InjectFile
#include "TypeTraits.hpp"

#ifdef _WIN32
    namespace Windows
    {
        #include <windows.h>
        #include <shlwapi.h>
    }

    using DynamicLibrary = Windows::HMODULE;

    #undef CreateDirectory
    #undef GetCurrentDirectory
    #undef SetCurrentDirectory
    #undef LoadLibrary
    #undef CopyFile
    #undef DeleteFile
    #undef SHFileOperation
    #undef CreateProcess
#else
    #error Platform not supported
#endif



namespace TraumaBuildSystem
{
    struct FileData
    {
        char*       buffer;
        size_t      size;
    };
}



namespace TraumaBuildSystem::Helpers
{
    template <size_t aSize, size_t bSize>
    inline constexpr auto StrCat(const char (&a)[aSize], const String<bSize>& b) { return a + b; }

    template <size_t aSize, size_t bSize>
    inline constexpr auto StrCat(const String<aSize>& a, const char (&b)[bSize]) { return a + b; }

    template <size_t aSize, size_t bSize>
    inline constexpr auto StrCat(const char (&a)[aSize], const char (&b)[bSize])
    {
        String<aSize + bSize - 1> string;
        string.append(a);
        string.append(b);
        return string;
    }

    inline constexpr const char* ToCStr(const auto& string)                                     { return string.c_str(); }
    inline constexpr const char* ToCStr(const char* const string)                               { return string; }
    inline constexpr const char* ToCStr(char* const string)                                     { return string; }

    inline constexpr size_t StrLen(const auto& string)                                          { return Length(string); }
    inline constexpr size_t StrLen(const char* const string)                                    { return strlen(string); }
    inline constexpr size_t StrLen(char* const string)                                          { return strlen(string); }
    template <size_t Size>
    inline consteval size_t StrLen(const char (&string)[Size])                                  { return Size; }

    #ifdef _WIN32
        inline constexpr auto ToWinPath(const auto& path)
        {
            static_assert(TypeTraits::IsStringLiteral<decltype(path)> || TypeTraits::IsString<decltype(path)>);

            if constexpr (TypeTraits::IsString<decltype(path)>)
            {
                String newPath = path;
                for (size_t i = 0; i < SizeOf(newPath); i++)
                    if (newPath[i] == '/')
                        newPath[i] = '\\';

                return newPath;
            }
            else
            {
                const char* pPath = Helpers::ToCStr(path);
                size_t pathLength = Helpers::StrLen(path);

                String<4096> newPath;
                strncpy(newPath.data(), pPath, pathLength);
                size_t newPathLength = Length(newPath);
                for (size_t i = 0; i < newPathLength; i++)
                    if (newPath[i] == '/')
                        newPath[i] = '\\';

                return newPath;
            }
        };

        inline constexpr auto ToProperPath(const auto& path)
        {
            static_assert(TypeTraits::IsStringLiteral<decltype(path)> || TypeTraits::IsString<decltype(path)>);

            if constexpr (TypeTraits::IsString<decltype(path)>)
            {
                String newPath = path;
                for (size_t i = 0; i < SizeOf(newPath); i++)
                    if (newPath[i] == '\\')
                        newPath[i] = '/';

                return newPath;
            }
            else
            {
                const char* pPath = Helpers::ToCStr(path);
                size_t pathLength = Helpers::StrLen(path);

                String<4096> newPath;
                strncpy(newPath.data(), pPath, pathLength);
                size_t newPathLength = Length(newPath);
                for (size_t i = 0; i < newPathLength; i++)
                    if (newPath[i] == '\\')
                        newPath[i] = '/';

                return newPath;
            }
        }

        inline Windows::LPWSTR ToWStr(const auto& string)
        {
            size_t stringLength = Helpers::StrLen(string) + 1;
            int wStrBufferCharLength = Windows::MultiByteToWideChar(CP_UTF8, 0, string, stringLength, nullptr, 0);
            auto wStr = static_cast<Windows::LPWSTR>(malloc(static_cast<size_t>(wStrBufferCharLength) * sizeof(wchar_t)));
            Windows::MultiByteToWideChar(CP_UTF8, 0, string, stringLength, wStr, wStrBufferCharLength);
            return wStr;
        }

        inline String<4096> ToCStr(Windows::LPWSTR wStr)
        {
            String<4096> string;
            Windows::WideCharToMultiByte(CP_UTF8, 0, wStr, -1, string.data(), SizeOf(string), 0, 0);
            return string;
        }
    #endif
}



namespace TraumaBuildSystem::Platform
{
    using VoidFnPtr = void(*)();

    DynamicLibrary                  LoadLibrary(const char* const filename);
    void                            FreeLibrary(DynamicLibrary library);

    VoidFnPtr                       GetFunction(DynamicLibrary library, const char* const functionName);

    template <typename FunctionPointer>
    FunctionPointer                 GetFunction(DynamicLibrary library, const char* const functionName);
}



constexpr auto TraumaBuildSystem::v1::Experimental::AsInclude(const auto& path)             { return Helpers::StrCat("-I", path); }
constexpr auto TraumaBuildSystem::v1::Experimental::AsSystemInclude(const auto& path)       { return Helpers::StrCat("-isystem", path); }
constexpr auto TraumaBuildSystem::v1::Experimental::AsLibrary(const auto& library)          { return Helpers::StrCat("-l", library); }
constexpr auto TraumaBuildSystem::v1::Experimental::AsLibraryPath(const auto& path)         { return Helpers::StrCat("-L", path); }
constexpr auto TraumaBuildSystem::v1::Experimental::AsDefine(const auto& name)              { return Helpers::StrCat("-D", name); }



inline constexpr auto TraumaBuildSystem::v1::Experimental::AsPath(const auto& path)
{
    String<4096> ret;
    if (!IsValidPath(path))
        return ret;

    if (ContainsAnyOf(path, " "))
    {
        ret[0] = '\"';
        ret.append(path);
        ret.append("\"");
    }
    else
        ret = path;

    #ifdef _WIN32
        ret = Helpers::ToWinPath(ret);
    #endif

    return ret;
}



inline auto TraumaBuildSystem::v1::Experimental::AsAbsolutePath(const auto& path)
{
    String<4096> ret;
    if (!IsValidPath(path))
        return ret;

    if (IsAbsolutePath(path))
        return AsPath(path);

    String cwd = CurrentWorkingDirectory();

    if (path[0] != '.')
        return AsPath(cwd / path);

    const char* p = path;
    p++;
    uint16 levelsUp = 0;
    while(*p == '.' || *p == '/')
    {
        if (*p == '/')
            levelsUp++;
        p++;
    }

    uint16 levelsDone = levelsUp;
    size_t lastIndex = InvalidStringIndex;
    while (levelsDone > 0)
    {
        size_t index = FindLastOf(cwd, "/", lastIndex - 1);
        if (index == InvalidStringIndex || index == 0)
            break;
        lastIndex = index;
        levelsDone--;
    }

    ret.copy(cwd, 0, lastIndex);
    ret.append("/");
    ret.append(path, 3 * levelsUp);

    return AsPath(ret);
}



inline constexpr auto TraumaBuildSystem::v1::Experimental::StripExtension(const auto& path)
{
    String<SizeOf(path)> ret;
    if (!IsValidPath(path))
        return ret;

    size_t index = FindLastOf(path, ".");
    if (index != InvalidStringIndex)
        index--;

    ret.copy(path, 0, index);
    return ret;
}



inline constexpr auto TraumaBuildSystem::v1::Experimental::StripFileName(const auto& path)
{
    static_assert(TypeTraits::IsStringLiteral<decltype(path)> || TypeTraits::IsString<decltype(path)>);

    String<sizeof(path)> ret;
    if (!IsValidPath(path))
        return ret;

    size_t index = FindLastOf(path, "\\/");

    if (index != InvalidStringIndex)
        index--;

    ret.copy(path, 0, index);
    return ret;
}



inline constexpr auto TraumaBuildSystem::v1::Experimental::StripPath(const auto& path)
{
    static_assert(TypeTraits::IsStringLiteral<decltype(path)> || TypeTraits::IsString<decltype(path)>);

    String<sizeof(path)> ret;
    if (!IsValidPath(path))
        return ret;

    size_t index = FindLastOf(path, "\\/");

    if (index == InvalidStringIndex)
        index = 0;

    ret.copy(path, 0, InvalidStringIndex, index);
    return ret;
}



inline constexpr auto TraumaBuildSystem::v1::Experimental::ExtensionOf(const auto& path)
{
    static_assert(TypeTraits::IsStringLiteral<decltype(path)> || TypeTraits::IsString<decltype(path)>);

    String<sizeof(path)> ret;
    if (!IsValidPath(path))
        return ret;

    size_t index = FindLastOf(path, ".");
    if (index == InvalidStringIndex)
        return ret;

    ret.copy(path, 0, InvalidStringIndex, index + 1);
    return ret;
}



inline bool TraumaBuildSystem::v1::Experimental::Exists(const auto& path)
{
    static_assert(TypeTraits::IsStringLiteral<decltype(path)> || TypeTraits::IsString<decltype(path)>);

    #ifdef _WIN32
        auto winPath = Helpers::ToWinPath(path);
        Windows::LPWSTR wStr = Helpers::ToWStr(winPath);
        Windows::BOOL result = Windows::PathFileExistsW(wStr);
        free(wStr);
        return result;
    #endif
}



inline bool TraumaBuildSystem::v1::Experimental::NotExists(const auto& path)
{
    return !Exists(path);
}



inline constexpr bool TraumaBuildSystem::v1::Experimental::IsValidPath([[maybe_unused]] const auto& path)
{
    // TODO
    return true;
}



inline constexpr bool TraumaBuildSystem::v1::Experimental::IsAbsolutePath([[maybe_unused]] const auto& path)
{
    // TODO
    return false;
}



inline constexpr bool TraumaBuildSystem::v1::Experimental::IsRelativePath([[maybe_unused]] const auto& path)
{
    // TODO
    return false;
}



inline bool TraumaBuildSystem::v1::Experimental::CreateDirectory(const auto& path)
{
    static_assert(TypeTraits::IsStringLiteral<decltype(path)> || TypeTraits::IsString<decltype(path)>);
    if (!IsValidPath(path)) return false;

    #ifdef _WIN32
        auto RecursiveDirectoryCreation = [] (auto winPath, auto RecursiveDirectoryCreation) -> bool
        {
            if (size_t index = FindLastOf(winPath, "\\");
                index != InvalidStringIndex)
            {
                index++;
                auto rootDirectory = static_cast<char*>(malloc(index));
                memcpy(rootDirectory, winPath, index);
                rootDirectory[index - 1] = '\0';
                RecursiveDirectoryCreation(rootDirectory, RecursiveDirectoryCreation);
                free(rootDirectory);
            }

            auto wStr = Helpers::ToWStr(winPath);
            Windows::BOOL success = Windows::CreateDirectoryW(wStr, nullptr);
            free(wStr);

            // TODO: Detailed error reporting.
            return success;
        };

        auto winPath = Helpers::ToWinPath(path);
        return RecursiveDirectoryCreation(winPath, RecursiveDirectoryCreation);
    #endif
}



inline bool TraumaBuildSystem::v1::Experimental::DeleteDirectory(const auto& path)
{
    static_assert(TypeTraits::IsStringLiteral<decltype(path)> || TypeTraits::IsString<decltype(path)>);

    #ifdef _WIN32
        auto winPath = Helpers::ToWinPath(path + "\0");
        auto wStr = Helpers::ToWStr(winPath);
        Windows::SHFILEOPSTRUCTW op = {};
        op.wFunc = FO_DELETE;
        op.pFrom = wStr;
        op.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;
        Windows::SHFileOperationW(&op);
        free(wStr);
        return !op.fAnyOperationsAborted;
    #endif
}



inline auto TraumaBuildSystem::v1::Experimental::CurrentWorkingDirectory()
{
    #ifdef _WIN32
        Windows::DWORD wStrBufferCharLength = Windows::GetCurrentDirectoryW(0, nullptr);
        auto wStr = static_cast<Windows::LPWSTR>(malloc(static_cast<size_t>(wStrBufferCharLength) * sizeof(wchar_t)));
        Windows::GetCurrentDirectoryW(wStrBufferCharLength, wStr);
        String<4096> currentDir = Helpers::ToCStr(wStr);
        free(wStr);
        return Helpers::ToProperPath(currentDir);
    #endif
}



inline bool TraumaBuildSystem::v1::Experimental::CurrentWorkingDirectory(const auto& path)
{
    static_assert(TypeTraits::IsStringLiteral<decltype(path)> || TypeTraits::IsString<decltype(path)>);

    #ifdef _WIN32
        Windows::LPWSTR wStr = Helpers::ToWStr(Helpers::ToWinPath(path));
        bool success = Windows::SetCurrentDirectoryW(wStr);
        free(wStr);
        return success;
    #endif
}



inline void TraumaBuildSystem::v1::Experimental::ForEachFile(const auto& path, auto&& fn)
{
    static_assert(TypeTraits::IsStringLiteral<decltype(path)> || TypeTraits::IsString<decltype(path)>);
    // TODO: Strengthen fn static checks.

    #ifdef _WIN32
        auto winPath = Helpers::ToWinPath(path);
        auto wStr = Helpers::ToWStr(winPath);
        Windows::WIN32_FIND_DATAW fileData = {};
        Windows::HANDLE handle = Windows::FindFirstFileW(wStr, &fileData);
        free(wStr);
        do { fn(Helpers::ToCStr(fileData.cFileName)); } while (Windows::FindNextFileW(handle, &fileData));
        Windows::FindClose(handle);
    #endif
}



inline bool TraumaBuildSystem::v1::Experimental::DeleteFile(const auto& filename)
{
    static_assert(TypeTraits::IsStringLiteral<decltype(filename)> || TypeTraits::IsString<decltype(filename)>);

    if (!IsValidPath(filename) || NotExists(filename)) return false;

    #ifdef _WIN32
        auto winFilename = Helpers::ToWinPath(filename);
        auto wStr = Helpers::ToWStr(winFilename);
        bool success = Windows::DeleteFileW(wStr);
        free(wStr);
        return success;
    #endif
}



inline bool TraumaBuildSystem::v1::Experimental::CopyFile(const auto& fromPath, const auto& toPath)
{
    static_assert(TypeTraits::IsStringLiteral<decltype(fromPath)> || TypeTraits::IsString<decltype(fromPath)>);
    static_assert(TypeTraits::IsStringLiteral<decltype(toPath)> || TypeTraits::IsString<decltype(toPath)>);

    if (!IsValidPath(fromPath) || !IsValidPath(toPath)) return false;

    // TODO: If toPath doesn't exist, create.

    #ifdef _WIN32
        auto winFrom = Helpers::ToWinPath(fromPath);
        auto wStrFrom = Helpers::ToWStr(winFrom);

        auto winTo = Helpers::ToWinPath(toPath);
        auto wStrTo = Helpers::ToWStr(winTo);

        bool success = Windows::CopyFileW(wStrFrom, wStrTo, FALSE);
        free(wStrFrom);
        free(wStrTo);
        return success;
    #endif
}



inline TraumaBuildSystem::FileData TraumaBuildSystem::v1::Experimental::ReadFile(const auto& filename)
{
    FILE* f = fopen(filename, "rb");
    if (!f)
        return { nullptr, 0 };

    fseek(f, 0, SEEK_END);
    size_t fileSize = static_cast<size_t>(ftell(f));
    fseek(f, 0, SEEK_SET);

    auto buffer = static_cast<char*>(malloc(fileSize + 1));
    if (!buffer)
        return { nullptr, fileSize };

    fread(buffer, 1, fileSize, f);
    fclose(f);
    buffer[fileSize] = '\0';

    return { buffer, fileSize };
}



inline auto TraumaBuildSystem::v1::Experimental::Compile(const auto& sourceFile, const auto& compilerFlags, const auto& includes)
{
    static_assert(TypeTraits::IsStringLiteral<decltype(sourceFile)> || TypeTraits::IsString<decltype(sourceFile)>);

    printf("Building %s...\n", Helpers::ToCStr(sourceFile));
    String sourceOutput = sourceFile + ".o";
    Call("g++ -c" * compilerFlags * includes * "-o" * sourceOutput * sourceFile);
    return sourceOutput;
}



inline bool TraumaBuildSystem::v1::Experimental::Build(const auto& artifact, const auto& source, const auto& compilerFlags, const auto& linkerFlags, const auto& includes, const auto& libsPath, const auto& libs)
{
    static_assert(TypeTraits::IsStringLiteral<decltype(artifact)> || TypeTraits::IsString<decltype(artifact)>);

    printf("Building %s...\n", Helpers::ToCStr(artifact));
    Call("g++" * linkerFlags * compilerFlags * includes * libsPath * "-o" * artifact * source * libs);

    return true;
}



template <size_t Size>
inline void TraumaBuildSystem::v1::Experimental::Call(const auto& cmd, String<Size>& output) // TODO: Rewrite.
{
    static_assert(TypeTraits::IsStringLiteral<decltype(cmd)> || TypeTraits::IsString<decltype(cmd)>);
    static_assert(Size > 0);

    static constexpr String redirection = "2>&1"; // Redirects stderr
    // https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Message-Formatting-Options.html#Diagnostic-Message-Formatting-Options

    FILE* pipe = popen(Helpers::ToCStr(cmd * redirection), "r"); assert(pipe);
    if (!pipe) // TODO: Manage error.
        return;

    size_t c = fread(output.data(), 1, Size - 1, pipe);
    output[c - 1] = '\0';
    pclose(pipe);
}



inline void TraumaBuildSystem::v1::Experimental::Call(const auto& cmd)
{
    static_assert(TypeTraits::IsStringLiteral<decltype(cmd)> || TypeTraits::IsString<decltype(cmd)>);

    // TODO: system() is not safe, use something else.
    if constexpr (TypeTraits::IsString<decltype(cmd)>)
        system(cmd.c_str());
    else
        system(cmd);
}



inline void TraumaBuildSystem::v1::Experimental::ClearConsole()
{
    #ifdef _WIN32
        Call("cls");
    #endif
}



inline void TraumaBuildSystem::v1::Experimental::Print(const auto& message, ...)
{
    static_assert(TypeTraits::IsStringLiteral<decltype(message)> || TypeTraits::IsString<decltype(message)>);

    va_list args;
    va_start(args, message);
    if constexpr (TypeTraits::IsString<decltype(message)>)
        vprintf(message.c_str(), args);
    else
        vprintf(message, args);
    va_end(args);
}



inline void TraumaBuildSystem::v1::Experimental::Println(const auto& message, ...)
{
    static_assert(TypeTraits::IsStringLiteral<decltype(message)> || TypeTraits::IsString<decltype(message)>);

    va_list args;
    va_start(args, message);
    if constexpr (TypeTraits::IsString<decltype(message)>)
        vprintf((message + "\n").c_str(), args);
    else
    {
        vprintf(message, args);
        printf("\n");
    }
    va_end(args);
}



inline void TraumaBuildSystem::v1::Experimental::Println()
{
    printf("\n");
}



inline auto TraumaBuildSystem::v1::Experimental::SVN::CurrentRevision()
{
    // FIXME: This currently returns the HEAD revision, not the the Working Copy's.
    String<16> rev;
    TraumaBuildSystem::v1::Experimental::Call("svn info -rHEAD --show-item revision", rev);
    return rev;
}



inline auto TraumaBuildSystem::v1::Experimental::SVN::CurrentRevision(const auto& path)
{
    // FIXME: This currently returns the HEAD revision, not the the Working Copy's.
    String<16> rev;
    if (!IsValidPath(path))
        return rev;

    auto cwd = CurrentWorkingDirectory();
    CurrentWorkingDirectory(path);
    rev = CurrentRevision();
    CurrentWorkingDirectory(cwd);
    return rev;
}



inline DynamicLibrary TraumaBuildSystem::Platform::LoadLibrary(const auto& filename)
{
    static_assert(TypeTraits::IsStringLiteral<decltype(filename)> || TypeTraits::IsString<decltype(filename)>);

    #ifdef _WIN32
        auto winFilename = Helpers::ToWinPath(filename);
        Windows::LPWSTR wStr = Helpers::ToWStr(winFilename);
        Windows::HMODULE handle = Windows::LoadLibraryW(wStr);
        free(wStr);
        return handle;
    #endif
}



inline void TraumaBuildSystem::Platform::FreeLibrary(DynamicLibrary library)
{
    #ifdef _WIN32
        Windows::FreeLibrary(library);
    #endif
}



inline TraumaBuildSystem::Platform::VoidFnPtr TraumaBuildSystem::Platform::GetFunction(DynamicLibrary library, const char* const functionName)
{
    assert(functionName);

    #ifdef _WIN32
        return reinterpret_cast<VoidFnPtr>(GetProcAddress(library, functionName));
    #endif
}



template <typename FunctionPointer>
inline FunctionPointer TraumaBuildSystem::Platform::GetFunction(DynamicLibrary library, const char* const functionName)
{
    static_assert(TypeTraits::IsPointer<FunctionPointer>);
    // static_assert(TypeTraits::IsFunction<std::remove_pointer_t<FunctionPointer>>); // FIXME
    assert(functionName);
    return reinterpret_cast<FunctionPointer>(GetFunction(library, functionName));
}
