#include <iostream>
#include <iomanip>
#include <windows.h>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <fstream>
#include <sstream>
using namespace std;

#define red "\033[91m"
#define original "\033[0m"
#define window_width 112

const string STAFF_ID = "bruh";
const string STAFF_PASSWORD = "1234567890";

struct Customer {
	string username;
	string age;
	string password;
	string fullName;
	string email;
	string phoneNumber;
	string icPassport;
};

vector<Customer> customers;
string currentLoggedInCustomer = "";

void enableColors();
void logo();
void displayUserTypeMenu();
void customerAuthMenu();
void customerRegister();
bool customerLogin();
void customerMenu();
void staffLogin();
void staffMenu();

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
void saveCustomersToFile();
void loadCustomersFromFile();

int main() {
	enableColors();
	loadCustomersFromFile();

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
	cout << " " <<  setw(30) << setfill('=') << '=' << endl;
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
	cout << "\n  Logged in as: " << currentLoggedInCustomer << endl;
	cout << "  [ Put your customer module menu here ]" << endl;
	cout << "\n  Press Enter to logout and return to main menu...";
	cin.get();
	currentLoggedInCustomer = "";
}

void staffMenu() {
	cout << "\n  [ Put your staff module menu here ]" << endl;
	cout << "\n  Press Enter to return to main menu...";
	cin.get();
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
		for (char c : newCustomer.username) {
			if (!isalnum(c)) {
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
		else {
			cout << "  Invalid IC/Passport format!" << endl;
			continue;
		}
	}

	customers.push_back(newCustomer);
	saveCustomersToFile();
	cout << "\n  Registration successful! Welcome, " << newCustomer.fullName << "!" << endl;
	cout << "  You can now login with username: " << newCustomer.username << endl;
}

bool customerLogin() {
	string username, password;
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

		for (const auto& customer : customers) {
			if (customer.username == username && customer.password == password) {
				currentLoggedInCustomer = customer.username;
				cout << "\n  Login successful! Welcome back, " << customer.fullName << "!" << endl;
				return true;
			}
		}

		attempts++;
		if (attempts < MAX_ATTEMPTS) {
			cout << "  Login failed! Invalid username or password." << endl;
			cout << "  You have " << (MAX_ATTEMPTS - attempts) << " attempt(s) remaining." << endl;
			cout << "\n  Would you like to try again? (y/n): ";
			char choice;
			cin >> choice;
			cin.ignore();

			if (choice != 'y' && choice != 'Y') {
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
	string id, password;
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
		else {
			attempts++;
			if (attempts < MAX_ATTEMPTS) {
				cout << " Login failed! Invalid username or password." << endl;
				cout << " You have " << (MAX_ATTEMPTS - attempts) << " attempt(s) remaining." << endl;
				cout << " Would you like to try again? (y/n): ";
				char choice;
				cin >> choice;
				cin.ignore();

				if (choice != 'y' && choice != 'Y') {
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
}

int getValidatedInput(int min, int max) {
	string inputStr;

	while (true) {
		getline(cin, inputStr);

		if (inputStr.empty()) {
			cout << "  Please enter a number between " << min << "-" << max
				<< " (or 0 to cancel): ";
			continue;
		}

		bool isNumeric = true;
		for (char c : inputStr) {
			if (!isdigit(c)) {
				isNumeric = false;
				break;
			}
		}

		if (!isNumeric) {
			cout << "  Invalid input! Please enter a number between " << min << "-" << max
				<< " (or 0 to cancel): ";
			continue;
		}

		int input = stoi(inputStr);
		if (input != 0 && (input < min || input > max)) {
			cout << "  Number out of range! Please enter a number between " << min << "-" << max
				<< " (or 0 to cancel): ";
			continue;
		}
		else {
			return input;
		}
	}
}

bool isValidName(const string& name) {
	if (name.length() < 2 || name.length() > 50) {
		return false;
	}

	for (char c : name) {
		if (!isalpha(c) && c != ' ' && c != '\'' && c != '-') {
			return false;
		}
	}
	return true;
}

bool isValidAge(const string& age) {
	if (age.empty()) {
		return false;
	}

	for (char c : age) {
		if (!isdigit(c)) {
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

	for (char c : ic) {
		if (isdigit(c)) {
			digitCount++;
		}
		else if (c != '-') {
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

	for (char c : passport) {
		if (!isalnum(c)) {
			return false;
		}
	}

	return true;
}

void formatName(string& name) {
	if (name.empty()) return;

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

	for (char c : username) {
		if (!isalnum(c) && c != '.' && c != '_') {
			return false;
		}
	}

	if (username[0] == '.' || username[username.length() - 1] == '.') {
		return false;
	}

	return true;
}

bool isValidPhoneNumber(const string& phone) {
	string cleanPhone;
	for (char c : phone) {
		if (isdigit(c)) {
			cleanPhone += c;
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
	for (const auto& customer : customers) {
		if (customer.username == username) {
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

		for (const auto& customer : customers) {
			file << customer.username << endl;
			file << customer.age << endl;
			file << customer.password << endl;
			file << customer.fullName << endl;
			file << customer.email << endl;
			file << customer.phoneNumber << endl;
			file << customer.icPassport << endl;
			file << "---" << endl;
		}

		file.close();
	}
}

void loadCustomersFromFile() {
	ifstream file("customers.txt");
	if (file.is_open()) {
		int numCustomers;
		file >> numCustomers;
		file.ignore();

		customers.clear();

		for (int i = 0; i < numCustomers; i++) {
			Customer customer;
			string separator;

			getline(file, customer.username);
			getline(file, customer.age);
			getline(file, customer.password);
			getline(file, customer.fullName);
			getline(file, customer.email);
			getline(file, customer.phoneNumber);
			getline(file, customer.icPassport);
			getline(file, separator);

			customers.push_back(customer);
		}

		file.close();
	}
}