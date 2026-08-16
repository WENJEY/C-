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
#define green "\033[92m"
#define yellow "\033[93m"
#define cyan "\033[96m"
#define original "\033[0m"
#define window_width 112

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

vector<Customer> customers;
string currentLoggedInCustomer = "";

vector<Room> roomList = {
	{"101", "Single",  1, 120.00, "Available"},
	{"102", "Single",  1, 120.00, "Occupied"},
	{"201", "Deluxe",  2, 180.00, "Available"},
	{"202", "Deluxe",  2, 180.00, "Cleaning"},
	{"301", "Suite",   4, 320.00, "Available"},
	{"302", "Suite",   4, 320.00, "Maintenance"}
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

const vector<AddOnItem> ADDON_CATALOG = {
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

void enableColors();
void logo();
void displayUserTypeMenu();
void customerAuthMenu();
void customerRegister();
bool customerLogin();
void customerMenu();
void staffLogin();
void staffMenu();
void displayAvailableRoom();
void displayRoomsByType(const string& typeFilter);
void saveRoomsToFile();
void loadRoomsFromFile();
void saveCustomersToFile();
void loadCustomersFromFile();
void saveReservationsToFile();
void loadReservationsFromFile();
void bookRoom();
bool createOneBooking();
void afterBookingMenu();
void offerAddOns();
void addOnsForReservation(int resIndex);
void specialRequestMenu();
void applyPromoCode();
void redeemLoyaltyPoints();
void showSessionBill(bool showPayHint);
void processPayment();
void printAndSaveInvoice(const BillBreakdown& bill, const string& method);
void viewMyReservations();
void viewMyProfile();
void offerRoomUpgrade(int& roomIndex);
void maybeGiveWelcomeGift(int resIndex);
void loadUnpaidIntoSession();
void resetSessionExtras();
void pauseEnter();
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
string getSecurePassword(bool showPrompt = true);

int main() {
	enableColors();
	srand(static_cast<unsigned int>(time(0)));
	loadCustomersFromFile();
	loadRoomsFromFile();
	loadReservationsFromFile();

	while (true) {
		logo();
		displayUserTypeMenu();
		int choice = getValidatedInput(1, 3);

		switch (choice) {
		case 1:
			customerAuthMenu();
			break;
		case 2:
			staffLogin();
			break;
		case 3:
			cout << endl;
			cout << " Thank you for using Grand Horizon Hotel!" << endl;
			return 0;
		}
	}
}

void enableColors() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD mode;
	if (GetConsoleMode(hConsole, &mode)) {
		SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	}
}

void logo() {
	string logoArt = R"(
  /$$$$$$                                 /$$      /$$   /$$                  /$$
 /$$__  $$                               | $$     | $$  | $$                 |__/
| $$  \__/ /$$$$$$ /$$$$$$ /$$$$$$$  /$$$$$$$     | $$  | $$ /$$$$$$  /$$$$$$ /$$/$$$$$$$$ /$$$$$$ /$$$$$$$
| $$ /$$$$/$$__  $|____  $| $$__  $$/$$__  $$     | $$$$$$$$/$$__  $$/$$__  $| $|____ /$$//$$__  $| $$__  $$
| $$|_  $| $$  \__//$$$$$$| $$  \ $| $$  | $$     | $$__  $| $$  \ $| $$  \__| $$   /$$/ | $$  \ $| $$  \ $$
| $$  \ $| $$     /$$__  $| $$  | $| $$  | $$     | $$  | $| $$  | $| $$     | $$ /$$/   | $$  | $$ $$  | $$
|  $$$$$$| $$    |  $$$$$$| $$  | $|  $$$$$$$     | $$  | $|  $$$$$$| $$     | $$/$$$$$$$|  $$$$$$| $$  | $$
 \______/|__/     \_______|__/  |__/\_______/     |__/  |__/\______/|__/     |__|________/\______/|__/  |__/
)";
	string systemArt = R"(
      _ ___ _       _   _  __  _  _          ___ ___  _          __     __ ___ _
 |_| / \ | |_ |    |_) |_ (_  |_ |_) \  / /\  |   |  / \ |\ |   (_ \_/ (_   | |_ |\/|
 | | \_/ | |_ |_   | \ |_ __) |_ | \  \/ /--\ |  _|_ \_/ | \|   __) |  __)  | |_ |  |
                                                                                      )";
	stringstream ss(logoArt), s(systemArt);
	string line;

	cout << string(22, '=') << setw(window_width) << setfill('=') << "=" << string(22, '=') << endl;
	cout << red;
	while (getline(ss, line)) {
		cout << string(22, ' ') << setw(2) << setfill(' ') << " " << line << '\n';
	}
	cout << original;
	while (getline(s, line)) {
		cout << string(25, ' ') << setw(10) << setfill(' ') << " " << line << '\n';
	}
	cout << string(22, '=') << setw(window_width) << setfill('=') << "=" << string(22, '=') << endl;
}

void displayUserTypeMenu() {
	cout << setfill(' ') << endl;
	cout << " Welcome to Grand Horizon Hotel" << endl;
	cout << " " << setw(30) << setfill('=') << '=' << endl;
	cout << "\n +---------------- Hotel Reservation System ----------------+" << endl;
	cout << " | 1. Customer                                              |" << endl;
	cout << " | 2. Staff                                                 |" << endl;
	cout << " | 3. Exit System                                           |" << endl;
	cout << " +----------------------------------------------------------+" << endl;
	cout << " Please select user type (1-3): ";
}

void customerAuthMenu() {
	int choice;

	do {
		cout << "\n +---------- Customer Access -------------+" << endl;
		cout << " | 1. Register New Account                |" << endl;
		cout << " | 2. Login to Existing Account           |" << endl;
		cout << " | 0. Back to Main Menu                   |" << endl;
		cout << " +----------------------------------------+" << endl;
		cout << "  Please choose (0-2): ";
		choice = getValidatedInput(0, 2);

		switch (choice) {
		case 1:
			customerRegister();
			break;
		case 2:
			if (customerLogin()) {
				customerMenu();
			}
			break;
		case 0:
			return;
		}
	} while (true);
}

void customerMenu() {
	int choice;

	do {
		cout << "\n +-------------------------- Menu Page --------------------------+" << endl;
		cout << " | 1. View Available Rooms                                       |" << endl;
		cout << " | 2. Booking Room                                               |" << endl;
		cout << " | 3. View My Reservations                                       |" << endl;
		cout << " | 4. Modify Reservations                                        |" << endl;
		cout << " | 5. Cancel Reservations                                        |" << endl;
		cout << " | 6. View My Profile                                            |" << endl;
		cout << " | 0. Back to Main Menu                                          |" << endl;
		cout << " +---------------------------------------------------------------+" << endl;
		cout << " Please choose (0-6): ";
		choice = getValidatedInput(0, 6);

		switch (choice) {
		case 1:
			displayAvailableRoom();
			break;
		case 2:
			bookRoom();
			break;
		case 3:
			viewMyReservations();
			break;
		case 4:
			cout << "  4. Module. To be continued..." << endl;
			break;
		case 5:
			cout << "  5. Module. To be continued..." << endl;
			break;
		case 6:
			viewMyProfile();
			break;
		case 0:
			currentLoggedInCustomer = "";
			cout << "\n Logged out successfully." << endl;
			return;
		}
	} while (choice != 0);
}

void displayAvailableRoom() {
	displayRoomsByType("ALL");
}

void displayRoomsByType(const string& typeFilter) {
	int shown = 0;
	cout << setfill(' ') << left;
	cout << "\n +---------------------------------------------------------------+" << endl;
	cout << " | " << setw(10) << "Room No" << setw(12) << "Type" << setw(10) << "Capacity"
		 << setw(14) << "Price/Night" << setw(16) << "Status" << "|" << endl;
	cout << " +---------------------------------------------------------------+" << endl;

	for (size_t i = 0; i < roomList.size(); i++) {
		if (typeFilter != "ALL" && roomList[i].roomType != typeFilter) {
			continue;
		}
		if (shown > 0) {
			cout << endl;
		}
		cout << " | " << setw(10) << roomList[i].roomNumber
			 << setw(12) << roomList[i].roomType
			 << setw(10) << roomList[i].capacity
			 << "RM" << setw(12) << fixed << setprecision(2) << roomList[i].price
			 << setw(16) << roomList[i].status << "|";
		shown++;
	}

	if (shown == 0) {
		cout << " | No rooms found for this type.                                 |";
	}
	cout << "\n +---------------------------------------------------------------+" << endl;
}

void bookRoom() {
	loadUnpaidIntoSession();

	if (!currentSessionIDs.empty()) {
		cout << yellow << "\n  You still have unpaid booking(s) from this stay:" << original << endl;
		for (size_t i = 0; i < currentSessionIDs.size(); i++) {
			int idx = findReservationIndex(currentSessionIDs[i]);
			if (idx == -1) {
				continue;
			}
			cout << "   - #" << reservations[idx].reservationID
				 << "  Room " << reservations[idx].roomNumber
				 << "  (" << reservations[idx].roomType << ", "
				 << reservations[idx].nights << " night(s))" << endl;
		}

		cout << "\n +---------- Continue Your Stay ----------+" << endl;
		cout << " | 1. Continue to Add-ons / Payment       |" << endl;
		cout << " | 2. Book another room first             |" << endl;
		cout << " | 0. Back to Menu Page                   |" << endl;
		cout << " +----------------------------------------+" << endl;
		cout << "  Please choose (0-2): ";
		int pendingChoice = getValidatedInput(0, 2);

		if (pendingChoice == 0) {
			return;
		}
		if (pendingChoice == 1) {
			afterBookingMenu();
			return;
		}
	}

	bool created = createOneBooking();
	if (created || !currentSessionIDs.empty()) {
		afterBookingMenu();
	}
}

bool createOneBooking() {
	cout << "\n +---------- Choose Room Type ----------+" << endl;
	cout << " | 1. All rooms                         |" << endl;
	cout << " | 2. Single                            |" << endl;
	cout << " | 3. Deluxe                            |" << endl;
	cout << " | 4. Suite                             |" << endl;
	cout << " | 0. Cancel booking                    |" << endl;
	cout << " +--------------------------------------+" << endl;
	cout << "  Please choose (0-4): ";
	int typeChoice = getValidatedInput(0, 4);

	if (typeChoice == 0) {
		cout << "  Booking cancelled." << endl;
		return false;
	}

	string typeFilter = "ALL";
	if (typeChoice == 2) {
		typeFilter = "Single";
	}
	if (typeChoice == 3) {
		typeFilter = "Deluxe";
	}
	if (typeChoice == 4) {
		typeFilter = "Suite";
	}

	displayRoomsByType(typeFilter);

	string roomNumber;
	int roomIndex = -1;

	while (true) {
		cout << "\n  Enter room number to book (or 0 to cancel): ";
		getline(cin, roomNumber);

		if (roomNumber == "0") {
			cout << "  Booking cancelled." << endl;
			return false;
		}

		roomIndex = findRoomIndex(roomNumber);
		if (roomIndex == -1) {
			cout << red << "  Room not found! Please try again." << original << endl;
			continue;
		}
		if (typeFilter != "ALL" && roomList[roomIndex].roomType != typeFilter) {
			cout << "  That room is not in the " << typeFilter << " list. Please choose again." << endl;
			continue;
		}
		if (roomList[roomIndex].status != "Available") {
			cout << red << "  This room is not available (status: "
				 << roomList[roomIndex].status << "). Please choose another." << original << endl;
			continue;
		}
		break;
	}

	offerRoomUpgrade(roomIndex);

	int guests = 0;
	while (true) {
		cout << "\n  Enter number of guests (1-" << roomList[roomIndex].capacity << ", or 0 to cancel): ";
		guests = getIntInRange(0, 20);
		if (guests == 0) {
			cout << "  Booking cancelled." << endl;
			return false;
		}
		if (guests > roomList[roomIndex].capacity) {
			cout << yellow << "  This " << roomList[roomIndex].roomType << " only fits "
				 << roomList[roomIndex].capacity << " guest(s)." << original << endl;
			cout << "  You can continue and add an Extra Bed after confirming." << endl;
			if (!confirmYesNo("  Continue with this room anyway? (y/n): ")) {
				continue;
			}
		}
		break;
	}

	cout << "\n  Enter number of nights (or 0 to cancel): ";
	int nights = getIntInRange(0, 30);
	if (nights == 0) {
		cout << "  Booking cancelled." << endl;
		return false;
	}

	double estimated = roundMoney(roomList[roomIndex].price * nights);

	cout << fixed << setprecision(2);
	cout << "\n +------------------ Booking Summary ------------------+" << endl;
	cout << "  Guest        : " << currentCustomerName() << endl;
	cout << "  Room Number  : " << roomList[roomIndex].roomNumber << endl;
	cout << "  Room Type    : " << roomList[roomIndex].roomType << endl;
	cout << "  Guests       : " << guests << endl;
	cout << "  Nights       : " << nights << endl;
	cout << "  Price/Night  : RM " << roomList[roomIndex].price << endl;
	cout << "  Room Charge  : RM " << estimated << endl;
	cout << " +-----------------------------------------------------+" << endl;
	cout << "  Add-ons, promo codes and payment will appear after you confirm." << endl;

	if (!confirmYesNo("  Confirm booking? (y/n): ")) {
		cout << "  Booking cancelled." << endl;
		return false;
	}

	BookingRecord newBooking;
	newBooking.reservationID = generateReservationID();
	nextReservationID++;
	newBooking.customerUsername = currentLoggedInCustomer;
	newBooking.roomNumber = roomList[roomIndex].roomNumber;
	newBooking.roomType = roomList[roomIndex].roomType;
	newBooking.pricePerNight = roomList[roomIndex].price;
	newBooking.nights = nights;
	newBooking.guests = guests;
	newBooking.status = "Confirmed";
	newBooking.paymentStatus = "Unpaid";
	newBooking.paymentMethod = "-";
	newBooking.promoCode = "";
	newBooking.specialRequest = "-";
	newBooking.discount = 0;
	newBooking.loyaltyDiscount = 0;
	newBooking.serviceCharge = 0;
	newBooking.sst = 0;
	newBooking.totalAmount = 0;

	reservations.push_back(newBooking);
	currentSessionIDs.push_back(newBooking.reservationID);
	roomList[roomIndex].status = "Occupied";
	saveRoomsToFile();
	saveReservationsToFile();

	cout << green << "\n  Reservation confirmed! Your reservation ID is "
		 << newBooking.reservationID << "." << original << endl;
	maybeGiveWelcomeGift(static_cast<int>(reservations.size()) - 1);
	cout << cyan << "  Tip: first-time promo code WELCOME10 gives 10% off." << original << endl;
	return true;
}

void afterBookingMenu() {
	int choice;

	do {
		cout << setfill(' ') << left;
		cout << cyan << "\n +---------------- After Booking Menu ----------------+" << original << endl;
		cout << " | Your confirmed stay is ready. Choose what to do next.|" << endl;
		cout << " +-----------------------------------------------------+" << endl;

		for (size_t i = 0; i < currentSessionIDs.size(); i++) {
			int idx = findReservationIndex(currentSessionIDs[i]);
			if (idx == -1) {
				continue;
			}
			cout << " |  #" << reservations[idx].reservationID
				 << "  Room " << reservations[idx].roomNumber
				 << "  " << reservations[idx].roomType
				 << "  x" << reservations[idx].nights << " night(s)"
				 << "  [" << reservations[idx].paymentStatus << "]" << endl;
		}

		BillBreakdown preview = calculateSessionBill();
		cout << " +-----------------------------------------------------+" << endl;
		cout << " | Estimated total now : RM " << fixed << setprecision(2) << preview.total << endl;
		cout << " +-----------------------------------------------------+" << endl;
		cout << " | 1. Hotel Add-ons (breakfast, spa, pickup...)        |" << endl;
		cout << " | 2. View Bill / Make Payment                         |" << endl;
		cout << " | 3. Book Another Room                                |" << endl;
		cout << " | 4. Special Requests (high floor, birthday...)       |" << endl;
		cout << " | 5. Apply Promo Code                                 |" << endl;
		cout << " | 6. Redeem Loyalty Points                            |" << endl;
		cout << " | 0. Back to Customer Menu                            |" << endl;
		cout << " +-----------------------------------------------------+" << endl;
		cout << "  Please choose (0-6): ";
		choice = getValidatedInput(0, 6);

		switch (choice) {
		case 1:
			offerAddOns();
			break;
		case 2:
			showSessionBill(true);
			if (confirmYesNo("  Proceed to payment now? (y/n): ")) {
				processPayment();
				if (currentSessionIDs.empty()) {
					return;
				}
			}
			break;
		case 3:
			createOneBooking();
			break;
		case 4:
			specialRequestMenu();
			break;
		case 5:
			applyPromoCode();
			break;
		case 6:
			redeemLoyaltyPoints();
			break;
		case 0: {
			bool unpaid = false;
			for (size_t i = 0; i < currentSessionIDs.size(); i++) {
				int idx = findReservationIndex(currentSessionIDs[i]);
				if (idx != -1 && reservations[idx].paymentStatus == "Unpaid") {
					unpaid = true;
					break;
				}
			}
			if (unpaid) {
				cout << yellow << "\n  Your booking is saved, but payment is still unpaid." << original << endl;
				cout << "  You can continue add-ons / payment anytime from Booking Room." << endl;
			}
			return;
		}
		}
	} while (true);
}

void offerAddOns() {
	if (currentSessionIDs.empty()) {
		cout << "  No booking in this session." << endl;
		return;
	}

	int targetIndex = findReservationIndex(currentSessionIDs[0]);
	if (currentSessionIDs.size() > 1) {
		cout << "\n  Which room should receive the add-ons?" << endl;
		for (size_t i = 0; i < currentSessionIDs.size(); i++) {
			int idx = findReservationIndex(currentSessionIDs[i]);
			if (idx == -1) {
				continue;
			}
			cout << "  " << (i + 1) << ". Room " << reservations[idx].roomNumber
				 << " (" << reservations[idx].roomType << ")" << endl;
		}
		cout << "  0. Cancel" << endl;
		cout << "  Choose: ";
		int pick = getIntInRange(0, static_cast<int>(currentSessionIDs.size()));
		if (pick == 0) {
			return;
		}
		targetIndex = findReservationIndex(currentSessionIDs[pick - 1]);
	}

	if (targetIndex != -1) {
		addOnsForReservation(targetIndex);
	}
}

void addOnsForReservation(int resIndex) {
	int choice;

	do {
		cout << left << setfill(' ');
		cout << "\n +---------------- Hotel Add-ons ----------------+" << endl;
		cout << " | Room " << reservations[resIndex].roomNumber
			 << "  |  Guests: " << reservations[resIndex].guests
			 << "  |  Nights: " << reservations[resIndex].nights << endl;
		cout << " +-----------------------------------------------+" << endl;
		for (size_t i = 0; i < ADDON_CATALOG.size(); i++) {
			cout << " | " << (i + 1) << ". " << setw(24) << ADDON_CATALOG[i].name
				 << " RM " << setw(7) << fixed << setprecision(2) << ADDON_CATALOG[i].price
				 << ADDON_CATALOG[i].unit << endl;
		}
		cout << " | 0. Done with add-ons" << endl;
		cout << " +-----------------------------------------------+" << endl;

		if (!reservations[resIndex].addOns.empty()) {
			cout << "  Already added:" << endl;
			for (size_t i = 0; i < reservations[resIndex].addOns.size(); i++) {
				cout << "   - " << reservations[resIndex].addOns[i].name
					 << " x" << reservations[resIndex].addOns[i].quantity
					 << "  RM " << fixed << setprecision(2) << reservations[resIndex].addOns[i].lineTotal << endl;
			}
		}

		cout << "  Choose add-on (0-9): ";
		choice = getIntInRange(0, static_cast<int>(ADDON_CATALOG.size()));
		if (choice == 0) {
			saveReservationsToFile();
			return;
		}

		const AddOnItem& item = ADDON_CATALOG[choice - 1];
		cout << "  Quantity for " << item.name << " (1-20, 0 to cancel): ";
		int qty = getIntInRange(0, 20);
		if (qty == 0) {
			continue;
		}

		double line = roundMoney(item.price * qty);
		bool merged = false;
		for (size_t i = 0; i < reservations[resIndex].addOns.size(); i++) {
			if (reservations[resIndex].addOns[i].name == item.name) {
				reservations[resIndex].addOns[i].quantity += qty;
				reservations[resIndex].addOns[i].lineTotal = roundMoney(
					reservations[resIndex].addOns[i].unitPrice * reservations[resIndex].addOns[i].quantity);
				merged = true;
				break;
			}
		}

		if (!merged) {
			SelectedAddOn selected;
			selected.name = item.name;
			selected.unitPrice = item.price;
			selected.quantity = qty;
			selected.lineTotal = line;
			reservations[resIndex].addOns.push_back(selected);
		}

		cout << green << "  Added " << item.name << " x" << qty
			 << " (RM " << fixed << setprecision(2) << line << ")." << original << endl;
		saveReservationsToFile();
	} while (true);
}

void specialRequestMenu() {
	if (currentSessionIDs.empty()) {
		cout << "  No booking in this session." << endl;
		return;
	}

	int resIndex = findReservationIndex(currentSessionIDs.back());
	if (currentSessionIDs.size() > 1) {
		cout << "\n  Which room is this request for?" << endl;
		for (size_t i = 0; i < currentSessionIDs.size(); i++) {
			int idx = findReservationIndex(currentSessionIDs[i]);
			if (idx == -1) {
				continue;
			}
			cout << "  " << (i + 1) << ". Room " << reservations[idx].roomNumber << endl;
		}
		cout << "  0. Cancel" << endl;
		cout << "  Choose: ";
		int pick = getIntInRange(0, static_cast<int>(currentSessionIDs.size()));
		if (pick == 0) {
			return;
		}
		resIndex = findReservationIndex(currentSessionIDs[pick - 1]);
	}

	if (resIndex == -1) {
		return;
	}

	cout << "\n +---------- Special Requests ----------+" << endl;
	cout << " | 1. High floor                        |" << endl;
	cout << " | 2. Quiet room / away from lift       |" << endl;
	cout << " | 3. Extra pillows                     |" << endl;
	cout << " | 4. Baby crib                         |" << endl;
	cout << " | 5. Connecting rooms                  |" << endl;
	cout << " | 6. Type my own request               |" << endl;
	cout << " | 0. Cancel                            |" << endl;
	cout << " +--------------------------------------+" << endl;
	cout << "  Please choose (0-6): ";
	int choice = getValidatedInput(0, 6);

	string request = reservations[resIndex].specialRequest;
	if (request == "-") {
		request = "";
	}

	string extra;
	switch (choice) {
	case 1:
		extra = "High floor";
		break;
	case 2:
		extra = "Quiet room";
		break;
	case 3:
		extra = "Extra pillows";
		break;
	case 4:
		extra = "Baby crib";
		break;
	case 5:
		extra = "Connecting rooms";
		break;
	case 6:
		cout << "  Type your request: ";
		getline(cin, extra);
		break;
	case 0:
		return;
	}

	if (extra.empty()) {
		cout << "  No request added." << endl;
		return;
	}

	if (request.empty()) {
		request = extra;
	}
	else {
		request = request + ", " + extra;
	}

	reservations[resIndex].specialRequest = request;
	saveReservationsToFile();
	cout << green << "  Noted! We will try our best: " << request << original << endl;
}

void applyPromoCode() {
	cout << "\n +---------- Promo Codes ----------+" << endl;
	cout << " | WELCOME10  : 10% off            |" << endl;
	cout << " | HORIZON15  : 15% off (3+ nights)|" << endl;
	cout << " | WEEKEND20  : 20% off stay       |" << endl;
	cout << " | STUDENT5   : RM 5 off           |" << endl;
	cout << " | GOLDEN50   : RM 50 off (RM400+) |" << endl;
	cout << " +---------------------------------+" << endl;
	cout << "  Enter promo code (or 0 to cancel): ";

	string code;
	getline(cin, code);
	if (code == "0" || code.empty()) {
		return;
	}

	for (size_t i = 0; i < code.length(); i++) {
		code[i] = static_cast<char>(toupper(static_cast<unsigned char>(code[i])));
	}

	int totalNights = 0;
	double roomCharge = 0;
	for (size_t i = 0; i < currentSessionIDs.size(); i++) {
		int idx = findReservationIndex(currentSessionIDs[i]);
		if (idx == -1) {
			continue;
		}
		totalNights += reservations[idx].nights;
		roomCharge += reservations[idx].pricePerNight * reservations[idx].nights;
		roomCharge += addOnTotal(reservations[idx]);
	}

	sessionPromoPercent = 0;
	sessionPromoFlat = 0;
	sessionPromoCode = "";

	if (code == "WELCOME10") {
		sessionPromoPercent = 0.10;
	}
	else if (code == "HORIZON15") {
		if (totalNights < 3) {
			cout << red << "  HORIZON15 needs at least 3 nights in this stay." << original << endl;
			return;
		}
		sessionPromoPercent = 0.15;
	}
	else if (code == "WEEKEND20") {
		sessionPromoPercent = 0.20;
	}
	else if (code == "STUDENT5") {
		sessionPromoFlat = 5.00;
	}
	else if (code == "GOLDEN50") {
		if (roomCharge < 400.00) {
			cout << red << "  GOLDEN50 needs a subtotal of RM 400.00 or more." << original << endl;
			return;
		}
		sessionPromoFlat = 50.00;
	}
	else {
		cout << red << "  Invalid promo code." << original << endl;
		return;
	}

	sessionPromoCode = code;
	cout << green << "  Promo " << code << " applied!" << original << endl;
	showSessionBill(false);
}

void redeemLoyaltyPoints() {
	int points = loyaltyPointsOfCurrentUser();
	cout << "\n  Your loyalty points: " << points << endl;
	cout << "  Redeem " << REDEEM_BLOCK << " points = RM " << fixed << setprecision(2) << REDEEM_VALUE << " off." << endl;

	if (points < REDEEM_BLOCK) {
		cout << "  Not enough points yet. Keep booking to earn more!" << endl;
		return;
	}

	int maxBlocks = points / REDEEM_BLOCK;
	BillBreakdown bill = calculateSessionBill();
	int affordable = static_cast<int>(bill.afterDiscount / REDEEM_VALUE);
	if (affordable < 1) {
		cout << "  Current bill is too small to redeem points." << endl;
		return;
	}
	if (maxBlocks > affordable) {
		maxBlocks = affordable;
	}

	cout << "  You can redeem up to " << (maxBlocks * REDEEM_BLOCK) << " points." << endl;
	cout << "  Enter points to redeem (multiples of " << REDEEM_BLOCK << ", 0 to cancel): ";
	int redeem = getIntInRange(0, maxBlocks * REDEEM_BLOCK);
	if (redeem == 0) {
		return;
	}
	if (redeem % REDEEM_BLOCK != 0) {
		cout << "  Points must be in blocks of " << REDEEM_BLOCK << "." << endl;
		return;
	}

	sessionRedeemedPoints = redeem;
	cout << green << "  " << redeem << " points reserved for this payment." << original << endl;
	showSessionBill(false);
}

void showSessionBill(bool showPayHint) {
	BillBreakdown bill = calculateSessionBill();
	cout << setfill(' ') << left << fixed << setprecision(2);
	cout << "\n +---------------------- Your Bill ----------------------+" << endl;
	cout << "  Guest           : " << currentCustomerName() << endl;
	cout << "  Member          : " << membershipOfCurrentUser() << endl;
	cout << " +-------------------------------------------------------+" << endl;

	for (size_t i = 0; i < currentSessionIDs.size(); i++) {
		int idx = findReservationIndex(currentSessionIDs[i]);
		if (idx == -1) {
			continue;
		}
		double roomCharge = roundMoney(reservations[idx].pricePerNight * reservations[idx].nights);
		cout << "  #" << reservations[idx].reservationID
			 << " Room " << reservations[idx].roomNumber
			 << " " << reservations[idx].roomType
			 << "  " << reservations[idx].nights << " night(s)" << endl;
		cout << "    Room charge   : RM " << roomCharge << endl;
		for (size_t j = 0; j < reservations[idx].addOns.size(); j++) {
			cout << "    + " << reservations[idx].addOns[j].name
				 << " x" << reservations[idx].addOns[j].quantity
				 << " : RM " << reservations[idx].addOns[j].lineTotal << endl;
		}
		if (reservations[idx].specialRequest != "-") {
			cout << "    Request       : " << reservations[idx].specialRequest << endl;
		}
	}

	cout << " +-------------------------------------------------------+" << endl;
	cout << "  Subtotal        : RM " << bill.subtotal << endl;
	if (bill.promoDiscount > 0) {
		cout << "  Promo " << sessionPromoCode << "      : -RM " << bill.promoDiscount << endl;
	}
	if (bill.memberDiscount > 0) {
		cout << "  Member discount : -RM " << bill.memberDiscount << endl;
	}
	if (bill.loyaltyDiscount > 0) {
		cout << "  Loyalty points  : -RM " << bill.loyaltyDiscount << endl;
	}
	cout << "  Service 10%     : RM " << bill.serviceCharge << endl;
	cout << "  SST 8%          : RM " << bill.sst << endl;
	cout << " +-------------------------------------------------------+" << endl;
	cout << green << "  TOTAL PAYABLE   : RM " << bill.total << original << endl;
	cout << " +-------------------------------------------------------+" << endl;

	if (showPayHint) {
		cout << "  Payment methods: Cash / Card / Touch n Go / GrabPay / Boost" << endl;
	}
}

void processPayment() {
	bool alreadyPaid = true;
	for (size_t i = 0; i < currentSessionIDs.size(); i++) {
		int idx = findReservationIndex(currentSessionIDs[i]);
		if (idx != -1 && reservations[idx].paymentStatus == "Unpaid") {
			alreadyPaid = false;
			break;
		}
	}
	if (alreadyPaid) {
		cout << "  This stay is already paid." << endl;
		return;
	}

	BillBreakdown bill = calculateSessionBill();
	showSessionBill(false);

	cout << "\n +---------- Payment Method ----------+" << endl;
	cout << " | 1. Cash                            |" << endl;
	cout << " | 2. Debit / Credit Card             |" << endl;
	cout << " | 3. Touch n Go eWallet              |" << endl;
	cout << " | 4. GrabPay                         |" << endl;
	cout << " | 5. Boost                           |" << endl;
	cout << " | 0. Cancel payment                  |" << endl;
	cout << " +------------------------------------+" << endl;
	cout << "  Please choose (0-5): ";
	int method = getValidatedInput(0, 5);
	if (method == 0) {
		cout << "  Payment cancelled." << endl;
		return;
	}

	string methodName;
	if (method == 1) {
		methodName = "Cash";
	}
	else if (method == 2) {
		methodName = "Card";
	}
	else if (method == 3) {
		methodName = "Touch n Go";
	}
	else if (method == 4) {
		methodName = "GrabPay";
	}
	else {
		methodName = "Boost";
	}

	if (method == 1) {
		while (true) {
			cout << "  Cash received (RM, 0 to cancel): ";
			string cashStr;
			getline(cin, cashStr);
			if (cashStr == "0") {
				cout << "  Payment cancelled." << endl;
				return;
			}

			bool valid = !cashStr.empty();
			int dotCount = 0;
			for (size_t i = 0; i < cashStr.length(); i++) {
				if (cashStr[i] == '.') {
					dotCount++;
				}
				else if (!isdigit(cashStr[i])) {
					valid = false;
				}
			}
			if (!valid || dotCount > 1) {
				cout << "  Invalid amount." << endl;
				continue;
			}

			double cash = stod(cashStr);
			if (cash + 0.001 < bill.total) {
				cout << red << "  Insufficient cash. Total is RM "
					 << fixed << setprecision(2) << bill.total << original << endl;
				continue;
			}

			double change = roundMoney(cash - bill.total);
			cout << green << "  Payment accepted. Change: RM "
				 << fixed << setprecision(2) << change << original << endl;
			break;
		}
	}
	else if (method == 2) {
		while (true) {
			cout << "  Enter 16-digit card number (or 0 to cancel): ";
			string card;
			getline(cin, card);
			if (card == "0") {
				cout << "  Payment cancelled." << endl;
				return;
			}

			string digits = "";
			for (size_t i = 0; i < card.length(); i++) {
				if (isdigit(card[i])) {
					digits += card[i];
				}
			}
			if (digits.length() != 16) {
				cout << "  Card number must contain 16 digits." << endl;
				continue;
			}
			cout << "  Charging card ending " << digits.substr(12, 4) << " ..." << endl;
			break;
		}
	}
	else {
		cout << "  Pay RM " << fixed << setprecision(2) << bill.total
			 << " with " << methodName << " using the phone number on your profile." << endl;
		if (!confirmYesNo("  Confirm e-wallet payment? (y/n): ")) {
			cout << "  Payment cancelled." << endl;
			return;
		}
	}

	int earned = static_cast<int>(bill.total / 10.0) * POINTS_PER_RM10;
	int custIndex = findCustomerIndex(currentLoggedInCustomer);
	if (custIndex != -1 && sessionRedeemedPoints > 0) {
		customers[custIndex].loyaltyPoints -= sessionRedeemedPoints;
		if (customers[custIndex].loyaltyPoints < 0) {
			customers[custIndex].loyaltyPoints = 0;
		}
	}

	for (size_t i = 0; i < currentSessionIDs.size(); i++) {
		int idx = findReservationIndex(currentSessionIDs[i]);
		if (idx == -1) {
			continue;
		}
		double ownSubtotal = reservations[idx].pricePerNight * reservations[idx].nights + addOnTotal(reservations[idx]);
		double share = 0.0;
		if (bill.subtotal > 0.0) {
			share = ownSubtotal / bill.subtotal;
		}
		reservations[idx].paymentStatus = "Paid";
		reservations[idx].status = "Paid";
		reservations[idx].paymentMethod = methodName;
		reservations[idx].promoCode = sessionPromoCode;
		reservations[idx].discount = roundMoney((bill.promoDiscount + bill.memberDiscount) * share);
		reservations[idx].loyaltyDiscount = roundMoney(bill.loyaltyDiscount * share);
		reservations[idx].serviceCharge = roundMoney(bill.serviceCharge * share);
		reservations[idx].sst = roundMoney(bill.sst * share);
		reservations[idx].totalAmount = roundMoney(bill.total * share);
	}

	addLoyaltyPoints(earned);
	saveCustomersToFile();
	saveReservationsToFile();
	printAndSaveInvoice(bill, methodName);

	cout << green << "\n  Payment successful! Thank you, " << currentCustomerName() << "." << original << endl;
	cout << "  You earned " << earned << " loyalty point(s)." << endl;
	if (custIndex != -1) {
		cout << "  Membership: " << customers[custIndex].membershipStatus
			 << "  |  Points: " << customers[custIndex].loyaltyPoints << endl;
	}
	if (rand() % 5 == 0) {
		cout << yellow << "  Lucky draw: complimentary late checkout voucher for your next stay!" << original << endl;
	}

	currentSessionIDs.clear();
	resetSessionExtras();
	pauseEnter();
}

void printAndSaveInvoice(const BillBreakdown& bill, const string& method) {
	string invoiceId = padNumber(nextInvoiceNo, 4);
	string invoiceNo = "INV-" + invoiceId;
	nextInvoiceNo++;

	time_t now = time(0);
	char timeBuf[80];
	strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", localtime(&now));

	ostringstream receipt;
	receipt << fixed << setprecision(2);
	receipt << "=======================================================\n";
	receipt << "              GRAND HORIZON HOTEL RECEIPT\n";
	receipt << "=======================================================\n";
	receipt << " Invoice No : " << invoiceNo << "\n";
	receipt << " Date/Time  : " << timeBuf << "\n";
	receipt << " Guest      : " << currentCustomerName() << "\n";
	receipt << " Member     : " << membershipOfCurrentUser() << "\n";
	receipt << "------------------------------------------------------\n";

	for (size_t i = 0; i < currentSessionIDs.size(); i++) {
		int idx = findReservationIndex(currentSessionIDs[i]);
		if (idx == -1) {
			continue;
		}
		receipt << " #" << reservations[idx].reservationID
				<< " Room " << reservations[idx].roomNumber
				<< " " << reservations[idx].roomType
				<< " x" << reservations[idx].nights << " night(s)\n";
		receipt << "   Room     RM " << (reservations[idx].pricePerNight * reservations[idx].nights) << "\n";
		for (size_t j = 0; j < reservations[idx].addOns.size(); j++) {
			receipt << "   " << reservations[idx].addOns[j].name
					<< " x" << reservations[idx].addOns[j].quantity
					<< "  RM " << reservations[idx].addOns[j].lineTotal << "\n";
		}
	}

	receipt << "------------------------------------------------------\n";
	receipt << " Subtotal              RM " << bill.subtotal << "\n";
	receipt << " Promo / Member        -RM " << (bill.promoDiscount + bill.memberDiscount) << "\n";
	receipt << " Loyalty               -RM " << bill.loyaltyDiscount << "\n";
	receipt << " Service charge 10%     RM " << bill.serviceCharge << "\n";
	receipt << " SST 8%                 RM " << bill.sst << "\n";
	receipt << " TOTAL PAID             RM " << bill.total << "\n";
	receipt << " Method                 " << method << "\n";
	receipt << "=======================================================\n";
	receipt << "     Thank you for staying with Grand Horizon!\n";

	cout << "\n" << receipt.str();

	string fileName = "invoice_" + invoiceId + ".txt";
	ofstream file(fileName);
	if (file.is_open()) {
		file << receipt.str();
		file.close();
		cout << "  Receipt saved as " << fileName << endl;
	}
}

void viewMyReservations() {
	cout << "\n +---------------- My Reservations ----------------+" << endl;
	bool found = false;

	for (size_t i = 0; i < reservations.size(); i++) {
		if (reservations[i].customerUsername != currentLoggedInCustomer) {
			continue;
		}
		found = true;
		cout << " | #" << reservations[i].reservationID
			 << "  Room " << reservations[i].roomNumber
			 << "  " << reservations[i].roomType
			 << "  " << reservations[i].nights << " night(s)" << endl;
		cout << " |    Status: " << reservations[i].status
			 << "  |  Payment: " << reservations[i].paymentStatus << endl;
		if (reservations[i].paymentStatus == "Paid") {
			cout << " |    Paid RM " << fixed << setprecision(2) << reservations[i].totalAmount
				 << " via " << reservations[i].paymentMethod << endl;
		}
		if (!reservations[i].addOns.empty()) {
			cout << " |    Add-ons: ";
			for (size_t j = 0; j < reservations[i].addOns.size(); j++) {
				if (j > 0) {
					cout << ", ";
				}
				cout << reservations[i].addOns[j].name << " x" << reservations[i].addOns[j].quantity;
			}
			cout << endl;
		}
		cout << " +--------------------------------------------------+" << endl;
	}

	if (!found) {
		cout << " | You have no reservations yet.                    |" << endl;
		cout << " +--------------------------------------------------+" << endl;
	}
}

void viewMyProfile() {
	int idx = findCustomerIndex(currentLoggedInCustomer);
	if (idx == -1) {
		cout << "  Profile not found." << endl;
		return;
	}

	Customer c = customers[idx];
	int nextNeed = 0;
	string nextTier = "Silver";
	if (c.membershipStatus == "Regular") {
		nextNeed = SILVER_POINTS - c.loyaltyPoints;
		nextTier = "Silver";
	}
	else if (c.membershipStatus == "Silver") {
		nextNeed = GOLD_POINTS - c.loyaltyPoints;
		nextTier = "Gold";
	}

	cout << "\n +---------------- My Profile ----------------+" << endl;
	cout << " | Username   : " << c.username << endl;
	cout << " | Full name  : " << c.fullName << endl;
	cout << " | Age        : " << c.age << endl;
	cout << " | Email      : " << c.email << endl;
	cout << " | Phone      : " << c.phoneNumber << endl;
	cout << " | IC/Passport: " << c.icPassport << endl;
	cout << " | Member     : " << c.membershipStatus << endl;
	cout << " | Points     : " << c.loyaltyPoints << endl;
	if (c.membershipStatus != "Gold") {
		cout << " | Next tier  : " << nextNeed << " more point(s) to " << nextTier << endl;
	}
	else {
		cout << " | Next tier  : You are at the highest tier!" << endl;
	}
	cout << " | Perks      : Silver 5% off, Gold 10% off" << endl;
	cout << " +--------------------------------------------+" << endl;
}

void offerRoomUpgrade(int& roomIndex) {
	if (roomList[roomIndex].roomType == "Suite") {
		return;
	}

	string nextType = "Suite";
	if (roomList[roomIndex].roomType == "Single") {
		nextType = "Deluxe";
	}

	int upgradeIndex = -1;
	for (size_t i = 0; i < roomList.size(); i++) {
		if (roomList[i].roomType == nextType && roomList[i].status == "Available") {
			upgradeIndex = static_cast<int>(i);
			break;
		}
	}
	if (upgradeIndex == -1) {
		return;
	}

	double extra = roomList[upgradeIndex].price - roomList[roomIndex].price;
	cout << yellow << "\n  Upgrade offer: " << nextType << " Room "
		 << roomList[upgradeIndex].roomNumber << " is available." << original << endl;
	cout << "  Extra RM " << fixed << setprecision(2) << extra << " per night. Bigger room, nicer stay!" << endl;
	if (confirmYesNo("  Upgrade now? (y/n): ")) {
		roomIndex = upgradeIndex;
		cout << green << "  Upgraded to Room " << roomList[roomIndex].roomNumber
			 << " (" << roomList[roomIndex].roomType << ")." << original << endl;
	}
}

void maybeGiveWelcomeGift(int resIndex) {
	if (sessionSurpriseGiven) {
		return;
	}
	sessionSurpriseGiven = true;
	if (rand() % 4 != 0) {
		return;
	}

	SelectedAddOn gift;
	gift.name = "Welcome Fruit Platter (Complimentary)";
	gift.unitPrice = 0;
	gift.quantity = 1;
	gift.lineTotal = 0;
	reservations[resIndex].addOns.push_back(gift);
	saveReservationsToFile();
	cout << yellow << "  Surprise gift: complimentary welcome fruit platter added to your room!" << original << endl;
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

void pauseEnter() {
	cout << "\n  Press Enter to continue...";
	string dummy;
	getline(cin, dummy);
}

string padNumber(int value, int width) {
	string text = to_string(value);
	while (static_cast<int>(text.length()) < width) {
		text = "0" + text;
	}
	return text;
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

int getIntInRange(int minVal, int maxVal) {
	string inputStr;

	while (true) {
		getline(cin, inputStr);
		if (inputStr.empty()) {
			cout << "  Please enter a number between " << minVal << "-" << maxVal << ": ";
			continue;
		}

		bool isNumeric = true;
		for (size_t i = 0; i < inputStr.length(); i++) {
			if (!isdigit(inputStr[i])) {
				isNumeric = false;
				break;
			}
		}
		if (!isNumeric) {
			cout << "  Invalid input! Please enter a number between " << minVal << "-" << maxVal << ": ";
			continue;
		}

		int input = stoi(inputStr);
		if (input < minVal || input > maxVal) {
			cout << "  Number out of range! Please enter a number between " << minVal << "-" << maxVal << ": ";
			continue;
		}
		return input;
	}
}

bool confirmYesNo(const string& prompt) {
	while (true) {
		cout << prompt;
		string answer;
		getline(cin, answer);
		if (answer.empty()) {
			cout << "  Please enter y or n." << endl;
			continue;
		}
		char c = static_cast<char>(tolower(static_cast<unsigned char>(answer[0])));
		if (c == 'y') {
			return true;
		}
		if (c == 'n') {
			return false;
		}
		cout << "  Please enter y or n." << endl;
	}
}

double roundMoney(double amount) {
	return round(amount * 100.0) / 100.0;
}

double addOnTotal(const BookingRecord& booking) {
	double total = 0;
	for (size_t i = 0; i < booking.addOns.size(); i++) {
		total += booking.addOns[i].lineTotal;
	}
	return roundMoney(total);
}

BillBreakdown calculateSessionBill() {
	BillBreakdown bill;
	bill.roomCharge = 0;
	bill.addOnCharge = 0;

	for (size_t i = 0; i < currentSessionIDs.size(); i++) {
		int idx = findReservationIndex(currentSessionIDs[i]);
		if (idx == -1) {
			continue;
		}
		bill.roomCharge += reservations[idx].pricePerNight * reservations[idx].nights;
		bill.addOnCharge += addOnTotal(reservations[idx]);
	}

	bill.roomCharge = roundMoney(bill.roomCharge);
	bill.addOnCharge = roundMoney(bill.addOnCharge);
	bill.subtotal = roundMoney(bill.roomCharge + bill.addOnCharge);
	bill.promoDiscount = roundMoney(bill.subtotal * sessionPromoPercent + sessionPromoFlat);
	if (bill.promoDiscount > bill.subtotal) {
		bill.promoDiscount = bill.subtotal;
	}

	double afterPromo = roundMoney(bill.subtotal - bill.promoDiscount);
	bill.memberDiscount = roundMoney(afterPromo * membershipRate(membershipOfCurrentUser()));
	bill.afterDiscount = roundMoney(afterPromo - bill.memberDiscount);
	bill.loyaltyDiscount = roundMoney((sessionRedeemedPoints / REDEEM_BLOCK) * REDEEM_VALUE);
	if (bill.loyaltyDiscount > bill.afterDiscount) {
		bill.loyaltyDiscount = bill.afterDiscount;
	}
	bill.afterDiscount = roundMoney(bill.afterDiscount - bill.loyaltyDiscount);
	bill.serviceCharge = roundMoney(bill.afterDiscount * SERVICE_CHARGE_RATE);
	bill.sst = roundMoney((bill.afterDiscount + bill.serviceCharge) * SST_RATE);
	bill.total = roundMoney(bill.afterDiscount + bill.serviceCharge + bill.sst);
	return bill;
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

void staffMenu() {
	cout << "\n  Staff module. To be continued..." << endl;
	pauseEnter();
}

void customerRegister() {
	Customer newCustomer;
	string confirmPassword;

	cout << "\n +-----------------------------------------------+" << endl;
	cout << " |              Customer Registration            |" << endl;
	cout << " +-----------------------------------------------+" << endl;
	cout << " |  Please fill in the following information     |" << endl;
	cout << " |  Enter '0' at any field to cancel             |" << endl;
	cout << " +-----------------------------------------------+" << endl;

	while (true) {
		cout << "\n  Enter username (3-20 characters, letters/numbers only): ";
		getline(cin, newCustomer.username);

		if (newCustomer.username == "0") {
			cout << "  Registration cancelled." << endl;
			return;
		}
		if (newCustomer.username.empty()) {
			cout << "  Username cannot be empty!" << endl;
			continue;
		}
		if (newCustomer.username.length() < 3 || newCustomer.username.length() > 20) {
			cout << "  Username must be 3-20 characters long!" << endl;
			continue;
		}

		bool validUsername = true;
		for (size_t i = 0; i < newCustomer.username.length(); i++) {
			if (!isalnum(newCustomer.username[i])) {
				validUsername = false;
				break;
			}
		}
		if (!validUsername) {
			cout << "  Username can only contain letters and numbers!" << endl;
			continue;
		}
		if (customerExists(newCustomer.username)) {
			cout << "  Username already exists! Please choose another." << endl;
			continue;
		}
		break;
	}

	while (true) {
		cout << "\n  Enter your age (or 0 to cancel): ";
		getline(cin, newCustomer.age);
		if (newCustomer.age == "0") {
			cout << "  Registration cancelled." << endl;
			return;
		}
		if (!isValidAge(newCustomer.age)) {
			cout << "  You must be 18 years old and above to book our hotel." << endl;
			continue;
		}
		break;
	}

	while (true) {
		cout << "\n  Enter password (minimum 6 characters, or 0 to cancel): ";
		newCustomer.password = getSecurePassword(false);
		if (newCustomer.password == "0") {
			cout << "  Registration cancelled." << endl;
			return;
		}
		if (!isValidPassword(newCustomer.password)) {
			cout << "  Password must be at least 6 characters long!" << endl;
			continue;
		}

		cout << "\n  Confirm password (or 0 to cancel): ";
		confirmPassword = getSecurePassword(false);
		if (confirmPassword == "0") {
			cout << "  Registration cancelled." << endl;
			return;
		}
		if (newCustomer.password != confirmPassword) {
			cout << "  Passwords do not match! Please try again." << endl;
			continue;
		}
		cout << "  Password confirmed successfully!" << endl;
		break;
	}

	while (true) {
		cout << "\n  Enter full name (or 0 to cancel): ";
		getline(cin, newCustomer.fullName);
		if (newCustomer.fullName == "0") {
			cout << "  Registration cancelled." << endl;
			return;
		}
		if (newCustomer.fullName.empty()) {
			cout << "  Full name cannot be empty!" << endl;
			continue;
		}
		if (!isValidName(newCustomer.fullName)) {
			cout << "  Invalid name format! Names should contain only letters and spaces (2-50 characters)." << endl;
			continue;
		}
		formatName(newCustomer.fullName);
		break;
	}

	while (true) {
		cout << "\n  Enter Gmail address (example: yourname@gmail.com, or 0 to cancel): ";
		getline(cin, newCustomer.email);
		if (newCustomer.email == "0") {
			cout << "  Registration cancelled." << endl;
			return;
		}
		if (newCustomer.email.empty()) {
			cout << "  Email cannot be empty!" << endl;
			continue;
		}
		if (!isValidEmail(newCustomer.email)) {
			cout << "  Invalid email! Please use Gmail format: yourname@gmail.com" << endl;
			continue;
		}
		cout << "  Valid Gmail address!" << endl;
		break;
	}

	while (true) {
		cout << "\n  Enter Malaysian phone number (+60xxxxxxxxx or 01xxxxxxxx, or 0 to cancel): ";
		getline(cin, newCustomer.phoneNumber);
		if (newCustomer.phoneNumber == "0") {
			cout << "  Registration cancelled." << endl;
			return;
		}
		if (newCustomer.phoneNumber.empty()) {
			cout << "  Phone number cannot be empty!" << endl;
			continue;
		}
		if (!isValidPhoneNumber(newCustomer.phoneNumber)) {
			cout << "  Invalid format! Use: +60123456789 or 0123456789" << endl;
			continue;
		}
		cout << "  Valid Malaysian phone number!" << endl;
		break;
	}

	while (true) {
		cout << "\n  Enter IC number (12 digits) or Passport number (or 0 to cancel): ";
		getline(cin, newCustomer.icPassport);
		if (newCustomer.icPassport == "0") {
			cout << "  Registration cancelled." << endl;
			return;
		}
		if (newCustomer.icPassport.empty()) {
			cout << "  IC/Passport cannot be empty!" << endl;
			continue;
		}
		if (isValidMalaysianIC(newCustomer.icPassport) || isValidPassport(newCustomer.icPassport)) {
			cout << "  Valid IC/Passport!" << endl;
			break;
		}
		cout << "  Invalid IC/Passport format!" << endl;
	}

	newCustomer.membershipStatus = "Regular";
	newCustomer.loyaltyPoints = 0;
	customers.push_back(newCustomer);
	saveCustomersToFile();
	cout << "\n  Registration successful! Welcome, " << newCustomer.fullName << "!" << endl;
	cout << "  You can now login with username: " << newCustomer.username << endl;
	cout << "  Loyalty membership started at Regular (0 points)." << endl;
	cout << "  Earn 1 point for every RM 10 spent. Silver = 5% off, Gold = 10% off." << endl;
}

bool customerLogin() {
	string username;
	string password;
	int attempts = 0;
	const int MAX_ATTEMPTS = 3;

	while (attempts < MAX_ATTEMPTS) {
		cout << "\n  ===== Customer Login =====" << endl;
		if (attempts > 0) {
			cout << "  Attempt " << attempts + 1 << " of " << MAX_ATTEMPTS << endl;
		}

		cout << "  Username (or 0 to cancel): ";
		getline(cin, username);
		if (username == "0") {
			cout << "  Login cancelled." << endl;
			return false;
		}

		cout << "  Password (or 0 to cancel): ";
		password = getSecurePassword(false);
		if (password == "0") {
			cout << "  Login cancelled." << endl;
			return false;
		}

		for (size_t i = 0; i < customers.size(); i++) {
			if (customers[i].username == username && customers[i].password == password) {
				currentLoggedInCustomer = customers[i].username;
				cout << "\n  Login successful! Welcome back, " << customers[i].fullName << "!" << endl;
				cout << "  Membership: " << customers[i].membershipStatus
					 << "  |  Points: " << customers[i].loyaltyPoints << endl;
				return true;
			}
		}

		attempts++;
		if (attempts < MAX_ATTEMPTS) {
			cout << "  Login failed! Invalid username or password." << endl;
			cout << "  You have " << (MAX_ATTEMPTS - attempts) << " attempt(s) remaining." << endl;
			if (!confirmYesNo("  Would you like to try again? (y/n): ")) {
				cout << "  Login cancelled." << endl;
				return false;
			}
		}
		else {
			cout << "  Login failed! Maximum attempts exceeded." << endl;
			cout << "  Access denied. Returning to main menu." << endl;
			return false;
		}
	}
	return false;
}

void staffLogin() {
	string id;
	string password;
	int attempts = 0;
	const int MAX_ATTEMPTS = 3;

	while (attempts < MAX_ATTEMPTS) {
		cout << "\n +---------- Staff Login ----------+" << endl;
		if (attempts > 0) {
			cout << " Attempt " << attempts + 1 << " of " << MAX_ATTEMPTS << endl;
		}

		cout << " Username (or 0 to cancel): ";
		getline(cin, id);
		if (id == "0") {
			cout << "  Login cancelled." << endl;
			return;
		}

		cout << " Password (or 0 to cancel): ";
		password = getSecurePassword(false);
		if (password == "0") {
			cout << " Login cancelled." << endl;
			return;
		}

		if (id == STAFF_ID && password == STAFF_PASSWORD) {
			cout << " Login successful! Welcome, " << id << "!" << endl;
			staffMenu();
			return;
		}

		attempts++;
		if (attempts < MAX_ATTEMPTS) {
			cout << " Login failed! Invalid username or password." << endl;
			cout << " You have " << (MAX_ATTEMPTS - attempts) << " attempt(s) remaining." << endl;
			if (!confirmYesNo(" Would you like to try again? (y/n): ")) {
				cout << "  Login cancelled." << endl;
				return;
			}
		}
		else {
			cout << "  Login failed! Maximum attempts exceeded." << endl;
			cout << "  Access denied. Returning to main menu." << endl;
			return;
		}
	}
}

int getValidatedInput(int min, int max) {
	string inputStr;

	while (true) {
		getline(cin, inputStr);
		if (inputStr.empty()) {
			cout << "  Please enter a number between " << min << "-" << max << " (or 0 to cancel): ";
			continue;
		}

		bool isNumeric = true;
		for (size_t i = 0; i < inputStr.length(); i++) {
			if (!isdigit(inputStr[i])) {
				isNumeric = false;
				break;
			}
		}
		if (!isNumeric) {
			cout << "  Invalid input! Please enter a number between " << min << "-" << max << " (or 0 to cancel): ";
			continue;
		}

		int input = stoi(inputStr);
		if (input != 0 && (input < min || input > max)) {
			cout << "  Number out of range! Please enter a number between " << min << "-" << max << " (or 0 to cancel): ";
			continue;
		}
		return input;
	}
}

bool isValidName(const string& name) {
	if (name.length() < 2 || name.length() > 50) {
		return false;
	}
	for (size_t i = 0; i < name.length(); i++) {
		if (!isalpha(name[i]) && name[i] != ' ' && name[i] != '\'' && name[i] != '-') {
			return false;
		}
	}
	return true;
}

bool isValidAge(const string& age) {
	if (age.empty()) {
		return false;
	}
	for (size_t i = 0; i < age.length(); i++) {
		if (!isdigit(age[i])) {
			return false;
		}
	}
	int ageValue = stoi(age);
	if (ageValue < 18 || ageValue > 120) {
		return false;
	}
	return true;
}

bool isValidMalaysianIC(const string& ic) {
	int digitCount = 0;
	for (size_t i = 0; i < ic.length(); i++) {
		if (isdigit(ic[i])) {
			digitCount++;
		}
		else if (ic[i] != '-') {
			return false;
		}
	}
	return (digitCount == 12);
}

bool isValidPassport(const string& passport) {
	if (passport.length() < 6 || passport.length() > 12) {
		return false;
	}
	if (!isalpha(passport[0])) {
		return false;
	}
	for (size_t i = 0; i < passport.length(); i++) {
		if (!isalnum(passport[i])) {
			return false;
		}
	}
	return true;
}

void formatName(string& name) {
	if (name.empty()) {
		return;
	}
	name[0] = toupper(name[0]);
	for (size_t i = 1; i < name.length(); i++) {
		if (name[i - 1] == ' ') {
			name[i] = toupper(name[i]);
		}
		else {
			name[i] = tolower(name[i]);
		}
	}
}

bool isValidEmail(const string& email) {
	if (email.length() < 10 || email.length() > 50) {
		return false;
	}

	string gmailSuffix = "@gmail.com";
	if (email.length() < gmailSuffix.length()) {
		return false;
	}

	string emailSuffix = email.substr(email.length() - gmailSuffix.length());
	if (emailSuffix != gmailSuffix) {
		return false;
	}

	string username = email.substr(0, email.length() - gmailSuffix.length());
	if (username.empty()) {
		return false;
	}
	for (size_t i = 0; i < username.length(); i++) {
		if (!isalnum(username[i]) && username[i] != '.' && username[i] != '_') {
			return false;
		}
	}
	if (username[0] == '.' || username[username.length() - 1] == '.') {
		return false;
	}
	return true;
}

bool isValidPhoneNumber(const string& phone) {
	string cleanPhone = "";
	for (size_t i = 0; i < phone.length(); i++) {
		if (isdigit(phone[i])) {
			cleanPhone += phone[i];
		}
	}

	if (cleanPhone.length() >= 11 && cleanPhone.length() <= 12) {
		if (cleanPhone.substr(0, 2) == "60") {
			return true;
		}
	}
	if (cleanPhone.length() >= 10 && cleanPhone.length() <= 11) {
		if (cleanPhone.substr(0, 2) == "01") {
			return true;
		}
	}
	return false;
}

bool isValidPassword(const string& password) {
	return password.length() >= 6;
}

bool customerExists(const string& username) {
	for (size_t i = 0; i < customers.size(); i++) {
		if (customers[i].username == username) {
			return true;
		}
	}
	return false;
}

string getSecurePassword(bool showPrompt) {
	string password;
	if (showPrompt) {
		cout << "  Enter password (min 6 characters): ";
	}
	getline(cin, password);
	if (!password.empty() && password != "0") {
		cout << "\n  Password entered: " << string(password.length(), '*') << endl;
	}
	return password;
}

void saveCustomersToFile() {
	ofstream file("customers.txt");
	if (file.is_open()) {
		file << customers.size() << endl;
		for (size_t i = 0; i < customers.size(); i++) {
			file << customers[i].username << endl;
			file << customers[i].age << endl;
			file << customers[i].password << endl;
			file << customers[i].fullName << endl;
			file << customers[i].email << endl;
			file << customers[i].phoneNumber << endl;
			file << customers[i].icPassport << endl;
			file << customers[i].membershipStatus << endl;
			file << customers[i].loyaltyPoints << endl;
			file << "---" << endl;
		}
		file.close();
	}
}

void loadCustomersFromFile() {
	ifstream file("customers.txt");
	if (!file.is_open()) {
		return;
	}

	int numCustomers = 0;
	file >> numCustomers;
	file.ignore();
	customers.clear();

	for (int i = 0; i < numCustomers; i++) {
		Customer customer;
		string separator;
		string pointsStr;
		getline(file, customer.username);
		getline(file, customer.age);
		getline(file, customer.password);
		getline(file, customer.fullName);
		getline(file, customer.email);
		getline(file, customer.phoneNumber);
		getline(file, customer.icPassport);
		getline(file, customer.membershipStatus);
		getline(file, pointsStr);
		getline(file, separator);

		if (customer.membershipStatus.empty() || customer.membershipStatus == "---") {
			customer.membershipStatus = "Regular";
			customer.loyaltyPoints = 0;
		}
		else if (pointsStr.empty()) {
			customer.loyaltyPoints = 0;
		}
		else {
			customer.loyaltyPoints = stoi(pointsStr);
		}
		customers.push_back(customer);
	}
	file.close();
}

void saveRoomsToFile() {
	ofstream file("rooms.txt");
	if (file.is_open()) {
		file << roomList.size() << endl;
		for (size_t i = 0; i < roomList.size(); i++) {
			file << roomList[i].roomNumber << endl;
			file << roomList[i].roomType << endl;
			file << roomList[i].capacity << endl;
			file << roomList[i].price << endl;
			file << roomList[i].status << endl;
			file << "---" << endl;
		}
		file.close();
	}
}

void loadRoomsFromFile() {
	ifstream file("rooms.txt");
	if (!file.is_open()) {
		return;
	}

	int numRooms = 0;
	file >> numRooms;
	file.ignore();
	if (numRooms <= 0) {
		file.close();
		return;
	}

	roomList.clear();
	for (int i = 0; i < numRooms; i++) {
		Room room;
		string capacityStr;
		string priceStr;
		string separator;
		getline(file, room.roomNumber);
		getline(file, room.roomType);
		getline(file, capacityStr);
		getline(file, priceStr);
		getline(file, room.status);
		getline(file, separator);
		room.capacity = stoi(capacityStr);
		room.price = stod(priceStr);
		roomList.push_back(room);
	}
	file.close();
}

void saveReservationsToFile() {
	ofstream file("reservations.txt");
	if (!file.is_open()) {
		return;
	}

	file << reservations.size() << endl;
	file << nextReservationID << endl;
	file << nextInvoiceNo << endl;

	for (size_t i = 0; i < reservations.size(); i++) {
		file << reservations[i].reservationID << endl;
		file << reservations[i].customerUsername << endl;
		file << reservations[i].roomNumber << endl;
		file << reservations[i].roomType << endl;
		file << reservations[i].pricePerNight << endl;
		file << reservations[i].nights << endl;
		file << reservations[i].guests << endl;
		file << reservations[i].status << endl;
		file << reservations[i].paymentStatus << endl;
		file << reservations[i].paymentMethod << endl;
		file << reservations[i].promoCode << endl;
		file << reservations[i].specialRequest << endl;
		file << reservations[i].discount << endl;
		file << reservations[i].loyaltyDiscount << endl;
		file << reservations[i].serviceCharge << endl;
		file << reservations[i].sst << endl;
		file << reservations[i].totalAmount << endl;
		file << reservations[i].addOns.size() << endl;
		for (size_t j = 0; j < reservations[i].addOns.size(); j++) {
			file << reservations[i].addOns[j].name << "|"
				 << reservations[i].addOns[j].unitPrice << "|"
				 << reservations[i].addOns[j].quantity << "|"
				 << reservations[i].addOns[j].lineTotal << endl;
		}
		file << "---" << endl;
	}
	file.close();
}

void loadReservationsFromFile() {
	ifstream file("reservations.txt");
	if (!file.is_open()) {
		return;
	}

	int numReservations = 0;
	file >> numReservations;
	file >> nextReservationID;
	file >> nextInvoiceNo;
	file.ignore();
	reservations.clear();

	for (int i = 0; i < numReservations; i++) {
		BookingRecord booking;
		string priceStr;
		string nightsStr;
		string guestsStr;
		string discountStr;
		string loyaltyStr;
		string serviceStr;
		string sstStr;
		string totalStr;
		string addonCountStr;
		string separator;

		getline(file, booking.reservationID);
		getline(file, booking.customerUsername);
		getline(file, booking.roomNumber);
		getline(file, booking.roomType);
		getline(file, priceStr);
		getline(file, nightsStr);
		getline(file, guestsStr);
		getline(file, booking.status);
		getline(file, booking.paymentStatus);
		getline(file, booking.paymentMethod);
		getline(file, booking.promoCode);
		getline(file, booking.specialRequest);
		getline(file, discountStr);
		getline(file, loyaltyStr);
		getline(file, serviceStr);
		getline(file, sstStr);
		getline(file, totalStr);
		getline(file, addonCountStr);

		booking.pricePerNight = stod(priceStr);
		booking.nights = stoi(nightsStr);
		booking.guests = stoi(guestsStr);
		booking.discount = stod(discountStr);
		booking.loyaltyDiscount = stod(loyaltyStr);
		booking.serviceCharge = stod(serviceStr);
		booking.sst = stod(sstStr);
		booking.totalAmount = stod(totalStr);

		int addonCount = stoi(addonCountStr);
		for (int a = 0; a < addonCount; a++) {
			string line;
			getline(file, line);
			SelectedAddOn item;
			stringstream ss(line);
			string pricePart;
			string qtyPart;
			string totalPart;
			getline(ss, item.name, '|');
			getline(ss, pricePart, '|');
			getline(ss, qtyPart, '|');
			getline(ss, totalPart, '|');
			item.unitPrice = stod(pricePart);
			item.quantity = stoi(qtyPart);
			item.lineTotal = stod(totalPart);
			booking.addOns.push_back(item);
		}
		getline(file, separator);
		reservations.push_back(booking);
	}
	file.close();
}
