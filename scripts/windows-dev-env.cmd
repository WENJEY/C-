@echo off
REM Prepend CMake/compiler/debugger folders that exist on this PC.
REM Safe on machines that do not have CLion, MSYS2, or extra tools installed.
REM Uses a subroutine so PATH updates actually accumulate (for-blocks would not).

if exist "C:\Program Files\CMake\bin\cmake.exe" set "PATH=C:\Program Files\CMake\bin;%PATH%"
if exist "%LOCALAPPDATA%\Programs\CMake\bin\cmake.exe" set "PATH=%LOCALAPPDATA%\Programs\CMake\bin;%PATH%"
if exist "C:\ProgramData\chocolatey\bin\cmake.exe" set "PATH=C:\ProgramData\chocolatey\bin;%PATH%"

if exist "C:\msys64\mingw64\bin\g++.exe" set "PATH=C:\msys64\mingw64\bin;%PATH%"
if exist "C:\msys64\ucrt64\bin\g++.exe" set "PATH=C:\msys64\ucrt64\bin;%PATH%"
if exist "C:\mingw64\bin\g++.exe" set "PATH=C:\mingw64\bin;%PATH%"

for /d %%D in ("C:\Program Files\JetBrains\CLion *") do call :add_clion "%%D"

where ninja.exe >nul 2>&1
if not errorlevel 1 (
    if not defined CMAKE_GENERATOR set "CMAKE_GENERATOR=Ninja"
)
goto :eof

:add_clion
if exist "%~1\bin\mingw\bin\g++.exe" set "PATH=%~1\bin\mingw\bin;%PATH%"
if exist "%~1\bin\cmake\win\x64\bin\cmake.exe" set "PATH=%~1\bin\cmake\win\x64\bin;%PATH%"
if exist "%~1\bin\ninja\win\x64\ninja.exe" set "PATH=%~1\bin\ninja\win\x64;%PATH%"
if exist "%~1\bin\gdb\win\x64\bin\gdb.exe" set "PATH=%~1\bin\gdb\win\x64\bin;%PATH%"
goto :eof
