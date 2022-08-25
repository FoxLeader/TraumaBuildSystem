#include "TraumaBuildSystem"

using namespace TraumaBuildSystem::v1::Experimental;



// Edit Me --->

StaticString cacheDir               = "Builds/.cache";
StaticString buildScriptsDir        = "BuildScripts";

StaticString additionalFlags        = "-Wall -Wextra -Wpedantic -Wsign-conversion ";

// <--- Edit Me



int main(int argc, char** argv)
{
    if (argc == 2 && IsValidPath(argv[1]))
        SetWorkingDirectory(argv[1]);

    if (Exists(cacheDir)) DeleteDirectory(cacheDir);
    CreateDirectory(cacheDir / buildScriptsDir);

    ClearConsole();
    Println("=== Building Scripts ===");
    ForEachFile(buildScriptsDir / "*.build", [&] (auto&& script)
    {
        Println("Compiling %s...", script.c_str());
        Call("g++ -s -std=c++20 -x c++ -shared" * additionalFlags * "-fdiagnostics-color=always -fno-rtti -fno-exceptions -o" * cacheDir / buildScriptsDir / script * buildScriptsDir / script * "-lShlwapi");
    });
    Println("=== Terminated ===\n");

    ForEachFile(cacheDir / buildScriptsDir / "*.build", [&] (auto&& script)
    {
        DynamicLibrary library = Platform::LoadLibrary(cacheDir / buildScriptsDir / script);
        Println("=== Build Process Started: %s ===", script.c_str());
        Platform::GetFunction(library, "BuildSteps")();
        Platform::FreeLibrary(library);
        Println("=== Build Process Terminated: %s ===\n", script.c_str());
    });
}
