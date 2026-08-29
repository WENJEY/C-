# C_

Console hotel app. Build with CMake from VS Code, Cursor, or a terminal.

## What to install

You need **CMake 3.21+** and a **C++14 compiler**. A debugger is optional but needed for F5.

| OS | Typical install |
|---|---|
| Windows | [CMake](https://cmake.org/download/) plus one of: [Visual Studio Build Tools](https://visualstudio.microsoft.com/visual-cpp-build-tools/), [MSYS2 MinGW](https://www.msys2.org/), or CLion (its bundled MinGW is detected automatically) |
| macOS | `xcode-select --install`, then `brew install cmake ninja` |
| Linux | `sudo apt install build-essential cmake gdb ninja-build` (or the equivalent on Fedora/Arch) |

On Windows, [Ninja](https://ninja-build.org/) is used automatically when it is on PATH (CLion includes it). Otherwise CMake picks the default generator (Visual Studio or MinGW Makefiles).

## VS Code / Cursor

1. Install the recommended extensions when prompted (C/C++ and CMake Tools).
2. Open this folder.
3. **Terminal → Run Build Task** (or Ctrl+Shift+B) to configure and build.
4. **Run → Start Debugging** (F5) and choose:
   - **Run C_ (gdb/lldb)** on MinGW, Linux, or macOS
   - **Run C_ (MSVC)** if you compiled with Visual Studio

No CLion-specific paths are required. Windows tasks look for CMake, Ninja, MinGW, MSYS2, and any `C:\Program Files\JetBrains\CLion *` install, then use whatever is on PATH.

## Terminal

```bash
cmake --preset vscode
cmake --build --preset vscode
```

The executable is `build/C_` (or `build/C_.exe` on Windows). With the Visual Studio generator it may be `build/Debug/C_.exe`.
