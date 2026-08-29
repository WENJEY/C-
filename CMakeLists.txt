cmake_minimum_required(VERSION 3.16)
project(C_)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
        CustomerInfo/*.cpp
        StaffInfo/*.cpp
        UI_Format/*.cpp
        main.cpp
)

add_executable(C_ ${APP_SOURCES})

# So a MinGW-built exe can run without compiler DLLs on PATH.
if(MINGW)
    target_link_options(C_ PRIVATE -static-libgcc -static-libstdc++)
endif()
