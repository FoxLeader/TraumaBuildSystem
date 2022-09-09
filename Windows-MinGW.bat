@echo off
setlocal

set WARNINGS_FLAGS=-Wall -Wextra -Wpedantic -Wsign-conversion -Wsuggest-override
REM set BUILD_FLAGS=-std=c++20 %WARNINGS_FLAGS% -O0 -g3 -fno-exceptions -fno-rtti
set BUILD_FLAGS=-s -std=c++20 %WARNINGS_FLAGS% -O3 -g0 -fno-exceptions -fno-rtti -DNDEBUG

set DEFINES=
set INCLUDES=-ISources
set LIBS_PATH=
set LIBS=-lShlwapi

rem - Build Steps

set BUILD_PATH=Build

if not exist %BUILD_PATH%\ mkdir %BUILD_PATH%

call g++ %BUILD_FLAGS% %DEFINES% %INCLUDES% %LIBS_PATH% -o %BUILD_PATH%\GenerateTBS.exe Sources\GenerateTBS.cpp %LIBS%
REM call g++ %BUILD_FLAGS% %DEFINES% %INCLUDES% %LIBS_PATH% -o tests.exe Sources\Tests.cpp %LIBS%

call %BUILD_PATH%\GenerateTBS.exe
del %BUILD_PATH%\GenerateTBS.exe

endlocal
