cmake_minimum_required(VERSION 4.2)
project(C___Assignment)

set(CMAKE_CXX_STANDARD 20)

# So every .cpp in subfolders can still #include "hotel.h"
include_directories(${CMAKE_SOURCE_DIR})

add_executable(C___Assignment
        main.cpp
        hotel.h
        # customerInfo — customer account & booking flow
        customerInfo/customer.cpp
        customerInfo/profile.cpp
        customerInfo/booking.cpp
        customerInfo/reservations.cpp
        customerInfo/modify.cpp
        customerInfo/cancel.cpp
        customerInfo/payment.cpp
        customerInfo/addons.cpp
        customerInfo/location.cpp
        customerInfo/room.cpp
        # staffInfo — staff tools
        staffInfo/staff.cpp
        # uiChecking — screen layout & input checks
        uiChecking/ui.cpp
        uiChecking/validation.cpp
        uiChecking/datetime.cpp
        # systemCore — shared data & file save/load
        systemCore/data.cpp
        systemCore/storage.cpp
)
