cmake_minimum_required(VERSION 3.16)
project(C_)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

# Do NOT compile only main.cpp. All module folders must be linked together.
file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
        CustomerInfo/*.cpp
        StaffInfo/*.cpp
        UI_Format/*.cpp
        main.cpp
)

add_executable(C_ ${APP_SOURCES})

if (MINGW)
    target_compile_options(C_ PRIVATE -Wa,-mbig-obj)
endif ()
