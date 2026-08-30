#include "../hotel.h"

void showProfileCard(const Customer& c);
void editFullName(int idx);
void editAge(int idx);
void editEmail(int idx);
void editPhone(int idx);
void editIcPassport(int idx);
void changePassword(int idx);
void editUsername(int idx);
void deleteMyAccount(int idx);

void viewMyProfile() {
	while (true) {
		int idx = findCustomerIndex(currentLoggedInCustomer);
		if (idx == -1) {
		cout << red << " Profile not found." << original << endl;
		pauseEnter();
		return;
		}

		showProfileCard(customers[idx]);
		boxRow(optionText(1) + "Edit full name");
		boxRow(optionText(2) + "Edit age");
		boxRow(optionText(3) + "Edit email");
		boxRow(optionText(4) + "Edit phone");
		boxRow(optionText(5) + "Edit IC / Passport");
		boxRow(optionText(6) + "Change password");
		boxRow(optionText(7) + "Edit username");
		boxRow(optionText(8) + "Delete account");
		boxRow(optionText(0) + "Back to Menu Page");
		boxLine();
		cout << " Please choose 0-8: ";
		int choice = getIntInRange(0, 8);

		if (choice == 0) {
			return;
		}
		if (choice == 1) {
			editFullName(idx);
		}
		else if (choice == 2) {
			editAge(idx);
		}
		else if (choice == 3) {
			editEmail(idx);
		}
		else if (choice == 4) {
			editPhone(idx);
		}
		else if (choice == 5) {
			editIcPassport(idx);
		}
		else if (choice == 6) {
			changePassword(idx);
		}
		else if (choice == 7) {
			editUsername(idx);
		}
		else if (choice == 8) {
			deleteMyAccount(idx);
			if (currentLoggedInCustomer.empty()) {
				return;
			}
		}
	}
}

void showProfileCard(const Customer& c) {
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

	showPage("My Profile");
	boxField("Username    : ", c.username);
	boxField("Full name   : ", c.fullName);
	boxField("Age         : ", c.age);
	boxField("Email       : ", c.email);
	boxField("Phone       : ", c.phoneNumber);
	boxField("IC/Passport : ", c.icPassport);
	boxField("Member      : ", c.membershipStatus);
	{
		ostringstream line;
		line << c.loyaltyPoints;
		boxField("Points      : ", line.str());
	}
	if (c.membershipStatus != "Gold") {
		ostringstream line;
		line << nextNeed << " more point(s) to " << nextTier;
		boxField("Next tier   : ", line.str());
	}
	else {
		boxField("Next tier   : ", "Highest tier");
	}
	boxField("Perks       : ", "Silver 5% off, Gold 10% off");
	boxRow("Member tier and points cannot be typed here.");
	boxLine();
}

void editFullName(int idx) {
	cout << "\n Enter new full name (or 0 to cancel): ";
	string name;
	getline(cin, name);
	cout << endl;
	if (name == "0" || name.empty()) {
		cout << " Name not changed." << endl;
		pauseEnter();
		return;
	}
	if (!isValidName(name)) {
		cout << red << " Invalid name. Use letters and spaces, 2-50 characters." << original << endl;
		pauseEnter();
		return;
	}
	formatName(name);
	customers[idx].fullName = name;
	saveCustomersToFile();
	cout << " Full name updated." << endl;
	pauseEnter();
}

void editAge(int idx) {
	cout << "\n Enter new age (or 0 to cancel): ";
	string age;
	getline(cin, age);
	cout << endl;
	if (age == "0" || age.empty()) {
		cout << " Age not changed." << endl;
		pauseEnter();
		return;
	}
	if (!isValidAge(age)) {
		cout << red << " Age must be 18 to 120." << original << endl;
		pauseEnter();
		return;
	}
	customers[idx].age = age;
	saveCustomersToFile();
	cout << " Age updated." << endl;
	pauseEnter();
}

void editEmail(int idx) {
	cout << "\n Enter new Gmail (example: yourname@gmail.com or 0 to cancel): ";
	string email;
	getline(cin, email);
	cout << endl;
	if (email == "0" || email.empty()) {
		cout << " Email not changed." << endl;
		pauseEnter();
		return;
	}
	if (!isValidEmail(email)) {
		cout << red << " Invalid email. Use Gmail format: yourname@gmail.com" << original << endl;
		pauseEnter();
		return;
	}
	customers[idx].email = email;
	saveCustomersToFile();
	cout << " Email updated." << endl;
	pauseEnter();
}

void editPhone(int idx) {
	cout << "\n Enter new phone (+60xxxxxxxxx or 01xxxxxxxx or 0 to cancel): ";
	string phone;
	getline(cin, phone);
	cout << endl;
	if (phone == "0" || phone.empty()) {
		cout << " Phone not changed." << endl;
		pauseEnter();
		return;
	}
	if (!isValidPhoneNumber(phone)) {
		cout << red << " Invalid format. Use: +60123456789 or 0123456789" << original << endl;
		pauseEnter();
		return;
	}
	customers[idx].phoneNumber = phone;
	saveCustomersToFile();
	cout << " Phone updated." << endl;
	pauseEnter();
}

void editIcPassport(int idx) {
	cout << "\n Enter new IC (12 digits) or passport (or 0 to cancel): ";
	string ic;
	getline(cin, ic);
	cout << endl;
	if (ic == "0" || ic.empty()) {
		cout << " IC/Passport not changed." << endl;
		pauseEnter();
		return;
	}
	if (!isValidMalaysianIC(ic) && !isValidPassport(ic)) {
		cout << red << " Invalid IC/Passport format." << original << endl;
		pauseEnter();
		return;
	}
	customers[idx].icPassport = ic;
	saveCustomersToFile();
	cout << " IC/Passport updated." << endl;
	pauseEnter();
}

void changePassword(int idx) {
	cout << "\n Enter current password (or 0 to cancel): ";
	string current = getSecurePassword(false);
	if (current == "0") {
		cout << " Password not changed." << endl;
		pauseEnter();
		return;
	}
	if (current != customers[idx].password) {
		cout << " Current password is wrong." << endl;
		pauseEnter();
		return;
	}

	string nextPassword;
	string confirmPassword;
	while (true) {
		cout << "\n Enter new password (minimum 6 characters or 0 to cancel): ";
		nextPassword = getSecurePassword(false);
		if (nextPassword == "0") {
			cout << " Password not changed." << endl;
			pauseEnter();
			return;
		}
		if (!isValidPassword(nextPassword)) {
			cout << red << " Password must be at least 6 characters long." << original << endl;
			continue;
		}
		cout << "\n Confirm new password (or 0 to cancel): ";
		confirmPassword = getSecurePassword(false);
		if (confirmPassword == "0") {
			cout << " Password not changed." << endl;
			pauseEnter();
			return;
		}
		if (nextPassword != confirmPassword) {
			cout << red << " Passwords do not match. Please try again." << original << endl;
			continue;
		}
		break;
	}

	customers[idx].password = nextPassword;
	saveCustomersToFile();
	cout << " Password updated." << endl;
	pauseEnter();
}

void editUsername(int idx) {
	cout << "\n Current username: " << customers[idx].username << endl;
	cout << " Enter new username (3-20 letters/numbers or 0 to cancel): ";
	string newUsername;
	getline(cin, newUsername);
	cout << endl;

	if (newUsername == "0" || newUsername.empty()) {
		cout << " Username not changed." << endl;
		pauseEnter();
		return;
	}
	if (!isValidUsername(newUsername)) {
		cout << red << " Username must be 3-20 characters, letters and numbers only." << original << endl;
		pauseEnter();
		return;
	}
	if (newUsername == customers[idx].username) {
		cout << " That is already your username." << endl;
		pauseEnter();
		return;
	}
	if (usernameTakenExcept(newUsername, idx)) {
		cout << red << " Username already taken. Please choose another." << original << endl;
		pauseEnter();
		return;
	}

	cout << " Enter current password to confirm (or 0 to cancel): ";
	string current = getSecurePassword(false);
	if (current == "0") {
		cout << " Username not changed." << endl;
		pauseEnter();
		return;
	}
	if (current != customers[idx].password) {
		cout << " Current password is wrong." << endl;
		pauseEnter();
		return;
	}

	string oldUsername = customers[idx].username;
	customers[idx].username = newUsername;
	currentLoggedInCustomer = newUsername;

	for (size_t i = 0; i < reservations.size(); i++) {
		if (reservations[i].customerUsername == oldUsername) {
			reservations[i].customerUsername = newUsername;
		}
	}

	saveCustomersToFile();
	saveReservationsToFile();
	cout << " Username updated to " << newUsername << "." << endl;
	pauseEnter();
}

void deleteMyAccount(int idx) {
	if (currentUserHasUnpaid()) {
		cout << "\n Pay or cancel unpaid bookings before deleting this account." << endl;
		pauseEnter();
		return;
	}
	if (!confirmYesNo(" Delete this account forever? y/n: ")) {
		cout << " Account kept." << endl;
		pauseEnter();
		return;
	}

	customers.erase(customers.begin() + static_cast<size_t>(idx));
	saveCustomersToFile();
	currentLoggedInCustomer = "";
	clearCurrentHotel();
	resetOccupiedRooms();
	resetSessionExtras();
	currentSessionIDs.clear();
	cout << "\n Account deleted. You have been logged out." << endl;
	pauseEnter();
}