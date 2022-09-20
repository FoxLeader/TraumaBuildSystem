// ======================================================================================================= //
//      This file is part of Trauma Build System (https://github.com/FoxLeader/TraumaBuildSystem)          //
//      Copyright: PolyTrauma Studios Srls, All Rights Reserved.                                           //
//                                                                                                         //
//      Author: Fabiano Raffaelli                                                                          //
//                                                                                                         //
// ======================================================================================================= //
//      This software is licensed under Creative Commons (CC BY NC 4.0): See LICENSE.md for details.       //
// ======================================================================================================= //

#include "TraumaBuildSystem.hpp"

TRAUMA_BUILD_SYSTEM(v1::Experimental)

// Edit  Me --->

StaticString buildsDir              = "../Builds";
StaticString cacheDir               = buildsDir / ".cache";
StaticString buildScriptsDir        = "BuildScripts";
StaticString additionalFlags        = "-Wall -Wextra -Wpedantic -Wsign-conversion ";

// <--- Edit Me

int main(int argc, char** argv)
{
    if (argc == 2 && IsValidPath(argv[1]))
        CurrentWorkingDirectory(argv[1]);

    if (Exists(cacheDir))
        DeleteDirectory(cacheDir);
    CreateDirectory(cacheDir / buildScriptsDir);

    ClearConsole();
    Println("=== Checking Scripts ===");
    ForEachFile(buildScriptsDir / "*.build", [&] (auto&& script)
    {
        Println("%s...", script.c_str());
        Call("g++ -s -std=c++20 -x c++ -shared" * additionalFlags * "-fdiagnostics-color=always -fno-rtti -fno-exceptions -o" * cacheDir / buildScriptsDir / script * buildScriptsDir / script * "-lShlwapi");
    });
    Println("=== Checks Terminated ===\n");

    ForEachFile(cacheDir / buildScriptsDir / "*.build", [&] (auto&& script)
    {
        DynamicLibrary library = TraumaBuildSystem::Platform::LoadLibrary(cacheDir / buildScriptsDir / script);
        Println("=== Build Process Started: %s ===", script.c_str());
        TraumaBuildSystem::Platform::GetFunction(library, "BuildSteps")();
        TraumaBuildSystem::Platform::FreeLibrary(library);
        Println("=== Build Process Terminated: %s ===\n", script.c_str());
    });
}
