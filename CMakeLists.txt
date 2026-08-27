cmake_minimum_required(VERSION 4.2)
project(C_)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(C_
        CustomerInfo/addons.cpp
        CustomerInfo/booking.cpp
        CustomerInfo/cancel.cpp
        CustomerInfo/customer.cpp
        CustomerInfo/modify.cpp
        CustomerInfo/payment.cpp
        CustomerInfo/profile.cpp
        CustomerInfo/reservations.cpp
        CustomerInfo/room.cpp
        StaffInfo/staff.cpp
        StaffInfo/staffBookings.cpp
        StaffInfo/staffCheckIns.cpp
        StaffInfo/staffHelpers.cpp
        StaffInfo/staffLocations.cpp
        StaffInfo/staffReports.cpp
        StaffInfo/staffRooms.cpp
        StaffInfo/staffSearch.cpp
        UI_Format/datetime.cpp
        UI_Format/location.cpp
        UI_Format/data.cpp
        UI_Format/storage.cpp
        UI_Format/ui.cpp
        UI_Format/validation.cpp
        hotel.h
        main.cpp)
