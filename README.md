# Trauma Build System

A simple Build System written in C++, which lets you write C++ build scripts to build C++ projects.

---

**Right now this builds on Windows only.** Linux and Mac support is coming.

## A Quick Example

```cpp
#include "TraumaBuildSystem"

using namespace TraumaBuildSystem::v1::Experimental;

StaticString buildsDir      = "Builds";
StaticString debugDir       = buildsDir / "Debug";
StaticString releaseDir     = buildsDir / "Release";

StaticString cppStandard    = "-std=c++20";
StaticString globalFlags    = "-Wall -Wextra -Wpedantic";

StaticString debugFlags     = cppStandard * globalFlags * AsDefine("DEBUG_BUILD");

StaticString includes       = AsInclude("Sources");

BUILD_STEPS()
{
    if (NotExists(debugDir)) CreateDirectory(debugDir);
    if (NotExists(releaseDir)) CreateDirectory(releaseDir);

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

- Place build.exe wherever you like and run it, the build process will start: the system will look for all .build files, will attempt to compile them, and will run the successful ones.

    - `OPTIONAL` Provide the root of your project as a parameter to build.exe, it will be used as the current working directory.

- Check the Examples directory to see how build scripts look like, and remember: a script **is just a regular C++ file**, whatever you can do in your regular day to day programming, you can do it to automate your build process.

## API and Documentation

You can find the documentation in the TraumaBuildSystem file itself.
