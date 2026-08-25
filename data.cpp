#include "hotel.h"

vector<Customer> customers;
string currentLoggedInCustomer = "";

// This list is the start of every run. Bookings stay in memory only.
vector<Room> roomList = {
	{"101", "Single",        1, 120.00, "Available"},
	{"102", "Single",        1, 120.00, "Available"},
	{"103", "Single",        1, 120.00, "Available"},
	{"104", "Single",        1, 120.00, "Available"},
	{"105", "Single",        1, 120.00, "Cleaning"},
	{"201", "Twin",          2, 150.00, "Available"},
	{"202", "Twin",          2, 150.00, "Available"},
	{"203", "Twin",          2, 150.00, "Available"},
	{"204", "Twin",          2, 150.00, "Available"},
	{"301", "Deluxe",        2, 180.00, "Available"},
	{"302", "Deluxe",        2, 180.00, "Available"},
	{"303", "Deluxe",        2, 180.00, "Available"},
	{"304", "Deluxe",        2, 180.00, "Available"},
	{"305", "Deluxe",        2, 180.00, "Cleaning"},
	{"401", "Family",        4, 250.00, "Available"},
	{"402", "Family",        4, 250.00, "Available"},
	{"403", "Family",        4, 250.00, "Available"},
	{"501", "Suite",         4, 320.00, "Available"},
	{"502", "Suite",         4, 320.00, "Available"},
	{"503", "Suite",         4, 320.00, "Available"},
	{"504", "Suite",         4, 320.00, "Maintenance"},
	{"601", "Presidential",  6, 580.00, "Available"},
	{"602", "Presidential",  6, 580.00, "Available"}
};

vector<BookingRecord> reservations;
int nextReservationID = 1;
int nextInvoiceNo = 1;

vector<string> currentSessionIDs;
string sessionPromoCode = "";
double sessionPromoPercent = 0.0;
double sessionPromoFlat = 0.0;
int sessionRedeemedPoints = 0;
bool sessionSurpriseGiven = false;

vector<HotelBranch> hotelBranches = {
	{"Grand Horizon Petaling Jaya", 0, 0, "2A, Jalan Universiti, 46200 Petaling Jaya", 1.6},
	{"Grand Horizon Shah Alam", 0, 1, "8, Persiaran Tasik, 40000 Shah Alam", 2.1},
	{"Grand Horizon Subang Jaya", 0, 2, "1, Jalan SS16/1, 47500 Subang Jaya", 1.2},
	{"Grand Horizon Kajang", 0, 3, "21, Jalan Semenyih, 43000 Kajang", 1.8},
	{"Grand Horizon Kepong", 1, 0, "88, Jalan Kepong, 52100 Kuala Lumpur", 1.4},
	{"Grand Horizon KLCC", 1, 1, "18, Jalan Ampang, 50450 Kuala Lumpur", 0.9},
	{"Grand Horizon Bukit Bintang", 1, 2, "5, Jalan Bukit Bintang, 55100 Kuala Lumpur", 1.1},
	{"Grand Horizon Cheras", 1, 3, "3, Jalan Cheras, 56000 Kuala Lumpur", 2.0},
	{"Grand Horizon Georgetown", 2, 0, "25, Lebuh Farquhar, 10200 George Town", 1.3},
	{"Grand Horizon Batu Ferringhi", 2, 1, "10, Jalan Batu Ferringhi, 11100 Penang", 1.7},
	{"Grand Horizon Bayan Lepas", 2, 2, "2, Jalan Bayan Lepas, 11900 Bayan Lepas", 2.4},
	{"Grand Horizon Johor Bahru", 3, 0, "11, Jalan Wong Ah Fook, 80000 Johor Bahru", 1.5},
	{"Grand Horizon Iskandar", 3, 1, "7, Persiaran Laksamana, 79200 Puteri", 2.2},
	{"Grand Horizon Skudai", 3, 2, "9, Jalan Pendidikan, 81300 Skudai", 1.9}
};

string currentHotelName = "";
string currentHotelAddress = "";
string currentHotelArea = "";
string currentHotelState = "";

vector<AddOnItem> addOnList = {
	{"Breakfast Buffet", 35.00, "per person / night"},
	{"Airport Pickup", 80.00, "per stay"},
	{"Extra Bed", 50.00, "per night"},
	{"Spa Relaxation", 120.00, "per person"},
	{"Late Checkout (2pm)", 40.00, "per stay"},
	{"Mini Bar Package", 60.00, "per stay"},
	{"Romantic Room Setup", 150.00, "per stay"},
	{"Birthday Cake", 80.00, "per stay"},
	{"KL City Tour Ticket", 90.00, "per person"}
};

bool customerExists(const string& username) {
	for (size_t i = 0; i < customers.size(); i++) {
		if (customers[i].username == username) {
			return true;
		}
	}
	return false;
}

void loadUnpaidIntoSession() {
	currentSessionIDs.clear();
	for (size_t i = 0; i < reservations.size(); i++) {
		if (reservations[i].customerUsername == currentLoggedInCustomer
			&& reservations[i].paymentStatus == "Unpaid"
			&& reservations[i].status != "Cancelled") {
			currentSessionIDs.push_back(reservations[i].reservationID);
		}
	}
}

void resetSessionExtras() {
	sessionPromoCode = "";
	sessionPromoPercent = 0.0;
	sessionPromoFlat = 0.0;
	sessionRedeemedPoints = 0;
	sessionSurpriseGiven = false;
}

void clearCurrentHotel() {
	currentHotelName = "";
	currentHotelAddress = "";
	currentHotelArea = "";
	currentHotelState = "";
}

void resetOccupiedRooms() {
	for (size_t i = 0; i < roomList.size(); i++) {
		if (roomList[i].status == "Occupied") {
			roomList[i].status = "Available";
		}
	}
}

void setRoomStatus(const string& roomNumber, const string& status) {
	int idx = findRoomIndex(roomNumber);
	if (idx != -1) {
		roomList[idx].status = status;
	}
}

void removeFromCurrentSession(const string& reservationID) {
	vector<string> kept;
	for (size_t i = 0; i < currentSessionIDs.size(); i++) {
		if (currentSessionIDs[i] != reservationID) {
			kept.push_back(currentSessionIDs[i]);
		}
	}
	currentSessionIDs = kept;
	if (currentSessionIDs.empty()) {
		resetSessionExtras();
	}
}

void syncRoomOccupancy() {
	for (size_t i = 0; i < roomList.size(); i++) {
		if (roomList[i].status == "Cleaning" || roomList[i].status == "Maintenance") {
			continue;
		}

		bool used = false;
		for (size_t r = 0; r < reservations.size(); r++) {
			if (reservations[r].status == "Cancelled") {
				continue;
			}
			if (reservations[r].roomNumber == roomList[i].roomNumber) {
				used = true;
				break;
			}
		}
		roomList[i].status = used ? "Occupied" : "Available";
	}
}

string generateReservationID() {
	return padNumber(nextReservationID, 4);
}

int findRoomIndex(const string& roomNumber) {
	for (size_t i = 0; i < roomList.size(); i++) {
		if (roomList[i].roomNumber == roomNumber) {
			return static_cast<int>(i);
		}
	}
	return -1;
}

int findCustomerIndex(const string& username) {
	for (size_t i = 0; i < customers.size(); i++) {
		if (customers[i].username == username) {
			return static_cast<int>(i);
		}
	}
	return -1;
}

int findReservationIndex(const string& reservationID) {
	for (size_t i = 0; i < reservations.size(); i++) {
		if (reservations[i].reservationID == reservationID) {
			return static_cast<int>(i);
		}
	}
	return -1;
}

string membershipOfCurrentUser() {
	int idx = findCustomerIndex(currentLoggedInCustomer);
	if (idx == -1) {
		return "Regular";
	}
	return customers[idx].membershipStatus;
}

int loyaltyPointsOfCurrentUser() {
	int idx = findCustomerIndex(currentLoggedInCustomer);
	if (idx == -1) {
		return 0;
	}
	return customers[idx].loyaltyPoints;
}

void addLoyaltyPoints(int points) {
	int idx = findCustomerIndex(currentLoggedInCustomer);
	if (idx == -1) {
		return;
	}
	customers[idx].loyaltyPoints += points;
	refreshMembershipTier(idx);
}

void refreshMembershipTier(int customerIndex) {
	if (customers[customerIndex].loyaltyPoints >= GOLD_POINTS) {
		customers[customerIndex].membershipStatus = "Gold";
	}
	else if (customers[customerIndex].loyaltyPoints >= SILVER_POINTS) {
		customers[customerIndex].membershipStatus = "Silver";
	}
	else {
		customers[customerIndex].membershipStatus = "Regular";
	}
}

double membershipRate(const string& status) {
	if (status == "Gold") {
		return 0.10;
	}
	if (status == "Silver") {
		return 0.05;
	}
	return 0.0;
}

string currentCustomerName() {
	int idx = findCustomerIndex(currentLoggedInCustomer);
	if (idx == -1) {
		return currentLoggedInCustomer;
	}
	return customers[idx].fullName;
}
