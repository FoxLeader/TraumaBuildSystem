# Trauma Build System

A simple Build System written in C++, which lets you write C++ build scripts to build C++ projects.

---

Current Limitations:
- **Requires** a C++20 compliant compiler. I'll be lowering this requirement in future releases.
- **Works only on Windows using MinGW.** Full support for Windows and Linux support is planned, and I'll gladly accept Mac support pull requests.

## A Quick Example

```cpp
#include "TraumaBuildSystem"

using namespace TraumaBuildSystem::v1::Experimental;

// The String class acts as a wrapper on char[] arrays, which allows for easier strings operations.
// A StaticString is just a define for a constexpr String, which allows to optimize out most of the String processing operations.

StaticString buildsDir      = "Builds";
StaticString debugDir       = buildsDir / "Debug";      // <- I'm concatenating Builds and Debug with a /, resulting in Builds/Debug!
StaticString releaseDir     = buildsDir / "Release";

StaticString cppStandard    = "-std=c++20";
StaticString globalFlags    = "-Wall -Wextra -Wpedantic";

StaticString debugFlags     = cppStandard * globalFlags * AsDefine("DEBUG_BUILD"); // <- the * indicates a concatenation with a white space in-between.

StaticString includes       = AsInclude("Sources");

BUILD_STEPS()
{
    if (NotExists(debugDir)) CreateDirectory(debugDir);
    if (NotExists(releaseDir)) CreateDirectory(releaseDir);

    ClearConsole();

    Build
    (
        debugDir / "Debug.exe",
        "Sources/Program.cpp",
        "-Og -g3" * debugFlags,
        "-static",
        includes,
        "",
        ""
    );

    Build
    (
        releaseDir / "Release.exe",
        "Sources/Program.cpp",
        "-O3" * cppStandard,
        "-static",
        includes,
        "",
        ""
    );
}
```

## How To Build

- `OPTIONAL` Inside TraumaBuildSystem.cpp you can customize:
    - The Cache directory, by modifying `cacheDir`.
    - The Build Script directory, by modifying `buildScriptsDir`
    - The Compiler flags when building scripts, by modifying `additionalFlags`.

- Run the Batch Script of your platform of choice, or:

    - build GenerateTBS.cpp like this:

        #### Windows

        > **MinGW**
        >
        >       g++ -s -std=c++20 -O3 -ISources -o GenerateTBS.exe Sources/GenerateTBS.cpp -lShlwapi
        >
        > **Clang**
        >
        >       TODO
        >
        > **MSVC**
        >
        >       TODO

    - Run `GenerateTBS.exe`. It can be removed when the process is complete.

- Inside the Build directory you'll find 2 files: build.exe and TraumaBuildSystem.

- Place TraumaBuildSystem inside your scripts directory and include it in your scripts.

- Place build.exe at the root of your project and run it, the build process will start: the system will look for all .build files in the specified path, will attempt to compile them, and will run the successful ones.

    - `OPTIONAL` Place build.exe wherever you like and provide the root of your project as a parameter to build.exe, it will be set as the current working directory.

- **TODO** Check the Examples directory to see what Build Scripts can do.

- Remember: a script **is just a regular C++ file**, whatever you can do in your regular day to day programming, you can use it to automate your build process now.

## API

You can find the API documentation in the TraumaBuildSystem file itself.
