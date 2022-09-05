# Trauma Build System

A simple Build System written in C++, which lets you write C++ build scripts to build C++ projects.

---

Current Limitations:

- **Requires** a C++20 compliant compiler. I'll be lowering this requirement in future releases.

- **Works only on Windows using MinGW.** Full support for Windows and Linux is planned, and I'll gladly accept Mac support pull requests.

## A Quick Example

```cpp
#include "TraumaBuildSystem"

TRAUMA_BUILD_SYSTEM(v1::Experimental) // <- I'm specifying the version I want to target, so that future releases won't break scripts written against an older spec.

// The String class acts as a wrapper on char[] arrays, which allows for easy strings operations.
// A StaticString is just a define for a constexpr String, which allows the compiler to perform most (often all) of the String operations at compile-time.

StaticString buildsDir      = "Builds";
StaticString debugDir       = buildsDir / "Debug";      // <- I'm concatenating the content of buildsDir and the literal string "Debug" with a /, resulting in Builds/Debug
StaticString releaseDir     = buildsDir / "Release";    // NOTE: Windows uses \ for paths, but you don't need to worry, / will be converted to \ internally.

StaticString cppStandard    = "-std=c++20";
StaticString globalFlags    = "-Wall -Wextra -Wpedantic";

StaticString debugFlags     = cppStandard * globalFlags * AsDefine("DEBUG_BUILD"); // <- the * indicates a concatenation, just like above, but with a white space instead of a /.
StaticString linkerFlags    = "-static";

StaticString includes       = AsInclude("Sources");

StaticString sourceFile     = "Sources/Program.cpp";

BUILD_STEPS()
{
    if (NotExists(debugDir)) CreateDirectory(debugDir);
    if (NotExists(releaseDir)) CreateDirectory(releaseDir);

    ClearConsole();

    Println("Building %s...", StripPath(sourceFile).c_str());
    Call("g++" * linkerFlags * "-Og -g3" * debugFlags * includes * "-o" * debugDir / "Debug.exe" * sourceFile);

    // A more structured way to do the above operation.
    Build
    (
        releaseDir / "Release.exe",
        sourceFile,
        "-O3" * cppStandard,
        linkerFlags,
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

    - build GenerateTBS.cpp from the command line like this:

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

- Place build.exe at the root of your project and run it, the build process will start: the system will look for all .build files in the specified path (`buildScriptsDir`), will attempt to compile them, and will run the successful ones.

    - `OPTIONAL` Place build.exe wherever you like and provide the root of your project as a parameter to build.exe, it will be set as the current working directory.

## How To Use

TODO

> Remember: a ".build" script **is just a regular C++ file**, whatever you can do in your regular day to day programming, you can use it to automate your build process now.

## Examples and API

Check the "Examples" directory for hints on how you can write your own .build scripts.  
The API documentation is in the generated TraumaBuildSystem file itself.
