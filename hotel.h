#ifndef HOTEL_H
#define HOTEL_H

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <string>
#include <vector>
#include <cctype>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cmath>
#include <cstdlib>
using namespace std;

#define red "\033[91m"
#define yellow "\033[93m"
#define blue "\033[94m"
#define cyan "\033[96m"
#define original "\033[0m"
#define window_width 112
#define BOX_W 64
#define COL_L 22
#define COL_R 41

const char* const STAFF_ID = "1";
const char* const STAFF_PASSWORD = "1";

const double SST_RATE = 0.08;
const double SERVICE_CHARGE_RATE = 0.10;
const int POINTS_PER_RM10 = 1;
const int REDEEM_BLOCK = 100;
const double REDEEM_VALUE = 10.00;
const int SILVER_POINTS = 500;
const int GOLD_POINTS = 1500;

struct Customer {
	string username;
	string age;
	string password;
	string fullName;
	string email;
	string phoneNumber;
	string icPassport;
	string membershipStatus;
	int loyaltyPoints;
};

struct Room {
	string roomNumber;
	string roomType;
	int capacity;
	double price;
	string status;
};

struct SelectedAddOn {
	string name;
	double unitPrice;
	int quantity;
	double lineTotal;
};

struct HotelBranch {
	string state;
	string area;
	string name;
	string address;
};

struct BookingRecord {
	string reservationID;
	string customerUsername;
	string hotelName;
	string hotelAddress;
	string hotelArea;
	string hotelState;
	string roomNumber;
	string roomType;
	double pricePerNight;
	int nights;
	int guests;
	string bookingDate;
	string bookingTime;
	string checkInDate;
	string checkInTime;
	string checkOutDate;
	string status;
	string paymentStatus;
	string paymentMethod;
	string promoCode;
	string specialRequest;
	double discount;
	double loyaltyDiscount;
	double serviceCharge;
	double sst;
	double totalAmount;
	vector<SelectedAddOn> addOns;
};

struct AddOnItem {
	string name;
	double price;
	string unit;
};

struct BillBreakdown {
	double roomCharge;
	double addOnCharge;
	double subtotal;
	double promoDiscount;
	double memberDiscount;
	double loyaltyDiscount;
	double afterDiscount;
	double serviceCharge;
	double sst;
	double total;
};

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
extern vector<HotelBranch> hotelBranches;
extern string currentHotelName;
extern string currentHotelAddress;
extern string currentHotelArea;
extern string currentHotelState;

void enableColors();
void clearScreen();
void showBanner();
void showPage(const string& title);
void logo();
void displayUserTypeMenu();
void customerAuthMenu();
void customerRegister();
bool customerLogin();
void customerMenu();
bool findHotelByDestination();
bool requireHotelSelected();
bool restoreHotelFromUnpaid();
void changeDestination();
void clearCurrentHotel();
void resetOccupiedRooms();
void staffLogin();
void staffMenu();
void displayAvailableRoom();
void displayRoomsByType(const string& typeFilter);
int displayBookableRooms(int guests);
int displayAvailabilityForDate(int guests, int day, int month, int year);
int displayBookableRoomsForDates(int guests, int inD, int inM, int inY, int outD, int outM, int outY);
int displayRoomsForModify(int guests, const string& currentRoomNumber, int inD, int inM, int inY, int outD, int outM, int outY, const string& skipReservationID);
void showRoomBookingCalendar(const string& roomNumber);
void showRoomAvailabilityForBooking(const string& roomNumber);
void printRoomColumns();
void printRoomDataRow(const string& number, const string& type, int capacity, double price, const string& status);
vector<string> destinationStates();
vector<string> areasOf(const string& state);
vector<int> hotelsOf(const string& state, const string& area);
void saveRoomsToFile();
void loadRoomsFromFile();
void saveCustomersToFile();
void loadCustomersFromFile();
void saveReservationsToFile();
void loadReservationsFromFile();
void saveHotelsToFile();
void loadHotelsFromFile();
void ensureDataFolder();
string dataFile(const string& name);
void bookRoom();
bool createOneBooking();
void afterBookingMenu();
void offerAddOns();
void addOnsForReservation(int resIndex);
void manageAddOnsForReservation(int resIndex);
void specialRequestMenu();
void applyPromoCode();
void redeemLoyaltyPoints();
void showSessionBill(bool showPayHint);
void processPayment();
void printAndSaveInvoice(const BillBreakdown& bill, const string& method);
void viewMyReservations();
void modifyReservations();
void modifyBookingMenu(int resIndex);
void saveStayChanges();
void remindPaidChange(int resIndex);
void cancelReservations();
bool confirmAndCancelReservation(int resIndex);
void viewMyProfile();
void offerRoomUpgrade(int& roomIndex, int guests);
void maybeGiveWelcomeGift(int resIndex);
void loadUnpaidIntoSession();
void showUnpaidReminder();
bool currentUserHasUnpaid();
void resetSessionExtras();
void setRoomStatus(const string& roomNumber, const string& status);
void removeFromCurrentSession(const string& reservationID);
void syncRoomOccupancy();
int daysFromToday(int day, int month, int year);
void pauseEnter();
void loadingPause();
void boxLine();
void boxTitle(const string& title);
void boxRow(const string& text);
void boxWrap(const string& text);
void boxWrapHang(const string& text, int hang);
void boxField(const string& label, const string& value);
string optionText(int number);
void boxLineEq();
void boxCenter(const string& text);
void boxInner(const string& leftText, const string& rightText);
void boxSplitLine();
void boxSplitRow(const string& leftText, const string& rightText);
void malaysiaNow(int& year, int& month, int& day, int& hour, int& minute);
string twoDigits(int value);
string makeDate(int day, int month, int year);
string makeClockTime(int hour, int minute);
string weekdayName(int day, int month, int year);
bool askAvailabilityBrowseDate(int& day, int& month, int& year);
bool askCheckInDate(int& day, int& month, int& year);
bool parseDate(const string& text, int& day, int& month, int& year);
bool isValidDate(int day, int month, int year);
int dateCompare(int d1, int m1, int y1, int d2, int m2, int y2);
int daysInMonth(int month, int year);
void addDays(int& day, int& month, int& year, int extraDays);
bool datesOverlap(int inD1, int inM1, int inY1, int outD1, int outM1, int outY1,
	int inD2, int inM2, int inY2, int outD2, int outM2, int outY2);
bool isRoomBookedForRange(const string& roomNumber,
	int inD, int inM, int inY, int outD, int outM, int outY,
	const string& skipReservationID = "");
bool isRoomAvailableForDates(const string& roomNumber,
	int inD, int inM, int inY, int outD, int outM, int outY,
	const string& skipReservationID = "");
bool isRoomAvailableOnDate(const string& roomNumber, int day, int month, int year);
string generateReservationID();
string padNumber(int value, int width);
int findRoomIndex(const string& roomNumber);
int findCustomerIndex(const string& username);
int findReservationIndex(const string& reservationID);
int getIntInRange(int minVal, int maxVal);
bool confirmYesNo(const string& prompt);
double roundMoney(double amount);
double addOnTotal(const BookingRecord& booking);
BillBreakdown calculateSessionBill();
string membershipOfCurrentUser();
int loyaltyPointsOfCurrentUser();
void addLoyaltyPoints(int points);
void refreshMembershipTier(int customerIndex);
double membershipRate(const string& status);
string currentCustomerName();
int getValidatedInput(int min, int max);
bool isValidName(const string& name);
bool isValidAge(const string& age);
bool isValidMalaysianIC(const string& ic);
bool isValidPassport(const string& passport);
void formatName(string& name);
bool isValidEmail(const string& email);
bool isValidPhoneNumber(const string& phone);
bool isValidPassword(const string& password);
bool customerExists(const string& username);
bool usernameTakenExcept(const string& username, int excludeIndex);
bool isValidUsername(const string& username);
string getSecurePassword(bool showPrompt = true);

#endif
