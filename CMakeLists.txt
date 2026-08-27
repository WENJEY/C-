cmake_minimum_required(VERSION 3.16)
project(C___Assignment)

set(CMAKE_CXX_STANDARD 20)

# IMPORTANT: every .cpp must be listed here. Linking only main.cpp causes
# "Undefined symbols" for logo/enableColors/staffLogin/... on macOS and Windows.
add_executable(C___Assignment
        main.cpp
        hotel.h
        data.cpp
        ui.cpp
        datetime.cpp
        validation.cpp
        storage.cpp
        customer.cpp
        staff.cpp
        room.cpp
        booking.cpp
        reservations.cpp
        profile.cpp
        addons.cpp
        payment.cpp
        modify.cpp
        cancel.cpp
        location.cpp
)

# MinGW COFF objects are limited to ~32k sections. Large string tables in
# data.cpp (and debug builds in general) can exceed that without big-obj.
if (MINGW)
    target_compile_options(C___Assignment PRIVATE -Wa,-mbig-obj)
endif ()
