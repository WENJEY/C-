#include "hotel.h"

int getValidatedInput(int min, int max) {
	string inputStr;

	while (true) {
		getline(cin, inputStr);
		if (inputStr.empty()) {
			cout << " Please enter a number between " << min << "-" << max << " or 0 to cancel: ";
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
			cout << " Invalid input! Please enter a number between " << min << "-" << max << " or 0 to cancel: ";
			continue;
		}

		int input = stoi(inputStr);
		if (input != 0 && (input < min || input > max)) {
			cout << " Number out of range! Please enter a number between " << min << "-" << max << " or 0 to cancel: ";
			continue;
		}
		return input;
	}
}

bool isValidName(const string& name) {
	if (name.length() < 2 || name.length() > 50) {
		return false;
	}
	bool hasLetter = false;
	for (size_t i = 0; i < name.length(); i++) {
		if (isalpha(static_cast<unsigned char>(name[i]))) {
			hasLetter = true;
		}
		else if (name[i] != ' ' && name[i] != '\'' && name[i] != '-') {
			return false;
		}
	}
	return hasLetter;
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

string getSecurePassword(bool showPrompt) {
	string password;
	if (showPrompt) {
		cout << " Enter password (min 6 characters): ";
	}
	getline(cin, password);
	if (!password.empty() && password != "0") {
		cout << "\n Password entered: " << string(password.length(), '*') << endl;
	}
	return password;
}

void pauseEnter() {
	cout << "\n Press Enter to continue...";
	string dummy;
	getline(cin, dummy);
}

void loadingPause() {
	cout << endl;
	cout << " Loading next page";
	for (int i = 0; i < 8; i++) {
		cout << "." << flush;
		Sleep(300);
	}
	cout << endl;
}

string padNumber(int value, int width) {
	string text = to_string(value);
	while (static_cast<int>(text.length()) < width) {
		text = "0" + text;
	}
	return text;
}

int getIntInRange(int minVal, int maxVal) {
	string inputStr;

	while (true) {
		getline(cin, inputStr);
		if (inputStr.empty()) {
			cout << " Please enter a number between " << minVal << "-" << maxVal << ": ";
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
			cout << " Invalid input! Please enter a number between " << minVal << "-" << maxVal << ": ";
			continue;
		}

		int input = stoi(inputStr);
		if (input < minVal || input > maxVal) {
			cout << " Number out of range! Please enter a number between " << minVal << "-" << maxVal << ": ";
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
			cout << " Please enter y or n." << endl;
			continue;
		}
		size_t start = 0;
		while (start < answer.length() && isspace(static_cast<unsigned char>(answer[start]))) {
			start++;
		}
		if (start >= answer.length()) {
			cout << " Please enter y or n." << endl;
			continue;
		}
		char c = static_cast<char>(tolower(static_cast<unsigned char>(answer[start])));
		if (c == 'y') {
			return true;
		}
		if (c == 'n') {
			return false;
		}
		cout << " Please enter y or n." << endl;
	}
}

double roundMoney(double amount) {
	return round(amount * 100.0) / 100.0;
}