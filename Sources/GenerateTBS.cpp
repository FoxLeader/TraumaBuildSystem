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

int main()
{
    StaticString buildDir               = "Build";
    StaticString sourcesDir             = "Sources";

    StaticString includeDirective       = "#include";
    StaticString pragmaOnceDirective    = "#pragma once";
    StaticString defineDirective        = "#define";

    StaticString TBSInjectFile          = "TBS_InjectFile";
    StaticString defineTBSInjectFileStr = defineDirective * TBSInjectFile;

    StaticString cppFlags               = "-s -std=c++20 -O3 -DNDEBUG -fno-rtti -fno-exceptions -ISources";

    if (Exists(buildDir))
        DeleteDirectory(buildDir);
    CreateDirectory(buildDir);

    Call("g++" * cppFlags * "-o" + buildDir / "build.exe Sources/TraumaBuildSystem.cpp -lShlwapi");

    auto [tbsFileBuffer, tbsFileBufferSize] = ReadFile("Sources/TraumaBuildSystem.hpp");
    char* p = tbsFileBuffer;
    char* injectDirective = strstr(p, defineTBSInjectFileStr);

    FILE* outFile = fopen(buildDir / "TraumaBuildSystem", "wb");
    if (injectDirective)
    {
        auto partSize = static_cast<size_t>(injectDirective - tbsFileBuffer);
        fwrite(tbsFileBuffer, 1, partSize, outFile);
        p = injectDirective + defineTBSInjectFileStr.max_size();
        char* pNext = strstr(p, TBSInjectFile);
        while (pNext)
        {
            fwrite(p, 1, static_cast<size_t>(pNext - p), outFile);
            p = strstr(p, includeDirective);
            char* filenameBegin = strchr(p, '\"');
            char* filenameEnd = strchr(filenameBegin + 1, '\"');
            p = filenameEnd + 1;

            String<4096> fileToInject = sourcesDir;
            fileToInject.append("/");
            fileToInject.append(filenameBegin + 1, static_cast<size_t>(filenameEnd - filenameBegin - 1));

            auto [buffer, bufferSize] = ReadFile(fileToInject);
            char* pBuffer = strstr(buffer, pragmaOnceDirective);
            if (pBuffer)
            {
                pBuffer += pragmaOnceDirective.max_size();
                bufferSize -= static_cast<size_t>(pBuffer - buffer);
            }
            else
                pBuffer = buffer;

            fwrite(pBuffer, 1, bufferSize , outFile);
            free(buffer);

            pNext = strstr(p, TBSInjectFile);
        }
        fputc('\n', outFile);
        fwrite(p, 1, strlen(p), outFile);
    }
    else
        fwrite(tbsFileBuffer, 1, tbsFileBufferSize, outFile);

    // NOTE: Shall we let the OS do the cleaning? We probably should.
    fclose(outFile);
    free(tbsFileBuffer);
}
