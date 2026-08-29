@echo off
setlocal EnableExtensions
call "%~dp0windows-dev-env.cmd"
where gdb.exe >nul 2>&1
if errorlevel 1 (
    echo gdb was not found. Install MinGW gdb, MSYS2, or CLion, or add gdb to PATH.
    exit /b 1
)
gdb.exe %*
