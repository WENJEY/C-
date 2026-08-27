cmake_minimum_required(VERSION 4.2)
project(C___Assignment)

set(CMAKE_CXX_STANDARD 20)

add_executable(C___Assignment main.cpp
        hotel.h
        "UI & Format/data.cpp"
        "UI & Format/ui.cpp"
        "UI & Format/datetime.cpp"
        "UI & Format/validation.cpp"
        "UI & Format/storage.cpp"
        CustomerInfo/customer.cpp
        StaffInfo/staff.cpp
        CustomerInfo/room.cpp
        CustomerInfo/booking.cpp
        CustomerInfo/reservations.cpp
        CustomerInfo/profile.cpp
        CustomerInfo/addons.cpp
        CustomerInfo/payment.cpp
        CustomerInfo/modify.cpp
        CustomerInfo/cancel.cpp
        "UI & Format/location.cpp"
        StaffInfo/staffRooms.cpp
        StaffInfo/staffLocations.cpp
        StaffInfo/staffReports.cpp
        StaffInfo/staffBookings.cpp
        StaffInfo/staffSearch.cpp
        StaffInfo/staffCheckIns.cpp
        StaffInfo/staffHelpers.cpp
        staff_internal.h
)
