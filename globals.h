#ifndef GLOBALS_H
#define GLOBALS_H

#include "common.h"

extern vector<Customer> customers;
extern string currentLoggedInCustomer;

extern vector<Room> roomList;
extern vector<BookingRecord> reservations;
extern int nextReservationID;
extern int nextInvoiceNo;

extern vector<string> currentSessionIDs;
extern string sessionPromoCode;
extern double sessionPromoPercent;
extern double sessionPromoFlat;
extern int sessionRedeemedPoints;
extern bool sessionSurpriseGiven;

extern vector<AddOnItem> addOnList;

int findRoomIndex(const string& roomNumber);
int findCustomerIndex(const string& username);
int findReservationIndex(const string& reservationID);
bool customerExists(const string& username);
string generateReservationID();

string membershipOfCurrentUser();
int loyaltyPointsOfCurrentUser();
void addLoyaltyPoints(int points);
void refreshMembershipTier(int customerIndex);
double membershipRate(const string& status);
string currentCustomerName();

void loadUnpaidIntoSession();
void resetSessionExtras();

#endif
