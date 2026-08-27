#ifndef STAFF_INTERNAL_H
#define STAFF_INTERNAL_H

#include "hotel.h"

// Staff menu modules (one file each)
void staffRoomMenu();
void staffLocationMenu();
void staffReports();
void staffViewAllBookings();
void staffSearchCustomer();
void staffTodayCheckIns();

// Shared helpers used by more than one staff module
bool roomHasActiveBooking(const string& roomNumber);
bool askLine(const string& prompt, string& out);
bool askMoney(const string& prompt, double& amount);
string pickStateForStaff(bool allowNew);
string pickAreaForStaff(const string& state, bool allowNew);
int pickHotelIndexForStaff(const string& state, const string& area);
string pickRoomStatus();
string pickRoomType();
void showStaffHotel(int idx);
void staffPrintRooms();
void staffListRooms();
void staffAddRoom();
void staffEditRoom();
void staffDeleteRoom();
void staffListHotels();
void staffAddHotel();
void staffEditHotel();
void staffDeleteHotel();

#endif
