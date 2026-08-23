#ifndef COMMON_H
#define COMMON_H

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
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
#define original "\033[0m"
#define window_width 112
#define BOX_W 64
#define COL_L 22
#define COL_R 41

const string STAFF_ID = "bruh";
const string STAFF_PASSWORD = "1234567890";

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

struct BookingRecord {
	string reservationID;
	string customerUsername;
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

#endif
