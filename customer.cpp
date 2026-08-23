#include "hotel.h"

void customerAuthMenu() {
	int choice;

	do {
		cout << endl;
		boxTitle("Customer Access");
		boxRow("1. Register New Account");
		boxRow("2. Login to Existing Account");
		boxRow("0. Back to Main Menu");
		boxLine();
		cout << " Please choose 0-2: ";
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

void customerRegister() {
	Customer newCustomer;
	string confirmPassword;

	cout << endl;
	boxTitle("Customer Registration");
	boxRow("Please fill in the following information");
	boxRow("Enter 0 at any field to cancel");
	boxLine();

	while (true) {
		cout << "\n Enter username (3-20 characters, letters/numbers only): ";
		getline(cin, newCustomer.username);

		if (newCustomer.username == "0") {
			cout << " Registration cancelled." << endl;
			return;
		}
		if (newCustomer.username.empty()) {
			cout << " Username cannot be empty!" << endl;
			continue;
		}
		if (newCustomer.username.length() < 3 || newCustomer.username.length() > 20) {
			cout << " Username must be 3-20 characters long!" << endl;
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
			cout << " Username can only contain letters and numbers!" << endl;
			continue;
		}
		if (customerExists(newCustomer.username)) {
			cout << " Username already exists! Please choose another." << endl;
			continue;
		}
		break;
	}

	while (true) {
		cout << "\n Enter your age (or 0 to cancel): ";
		getline(cin, newCustomer.age);
		if (newCustomer.age == "0") {
			cout << " Registration cancelled." << endl;
			return;
		}
		if (!isValidAge(newCustomer.age)) {
			cout << " You must be 18 years old and above to book our hotel." << endl;
			continue;
		}
		break;
	}

	while (true) {
		cout << "\n Enter password (minimum 6 characters or 0 to cancel): ";
		newCustomer.password = getSecurePassword(false);
		if (newCustomer.password == "0") {
			cout << " Registration cancelled." << endl;
			return;
		}
		if (!isValidPassword(newCustomer.password)) {
			cout << " Password must be at least 6 characters long!" << endl;
			continue;
		}

		cout << "\n Confirm password (or 0 to cancel): ";
		confirmPassword = getSecurePassword(false);
		if (confirmPassword == "0") {
			cout << " Registration cancelled." << endl;
			return;
		}
		if (newCustomer.password != confirmPassword) {
			cout << " Passwords do not match! Please try again." << endl;
			continue;
		}
		cout << " Password confirmed successfully!" << endl;
		break;
	}

	while (true) {
		cout << "\n Enter full name (or 0 to cancel): ";
		getline(cin, newCustomer.fullName);
		if (newCustomer.fullName == "0") {
			cout << " Registration cancelled." << endl;
			return;
		}
		if (newCustomer.fullName.empty()) {
			cout << " Full name cannot be empty!" << endl;
			continue;
		}
		if (!isValidName(newCustomer.fullName)) {
			cout << " Invalid name format! Names should contain only letters and spaces (2-50 characters)." << endl;
			continue;
		}
		formatName(newCustomer.fullName);
		break;
	}

	while (true) {
		cout << "\n Enter Gmail address (example: yourname@gmail.com or 0 to cancel): ";
		getline(cin, newCustomer.email);
		if (newCustomer.email == "0") {
			cout << " Registration cancelled." << endl;
			return;
		}
		if (newCustomer.email.empty()) {
			cout << " Email cannot be empty!" << endl;
			continue;
		}
		if (!isValidEmail(newCustomer.email)) {
			cout << " Invalid email! Please use Gmail format: yourname@gmail.com" << endl;
			continue;
		}
		cout << " Valid Gmail address!" << endl;
		break;
	}

	while (true) {
		cout << "\n Enter Malaysian phone number (+60xxxxxxxxx or 01xxxxxxxx or 0 to cancel): ";
		getline(cin, newCustomer.phoneNumber);
		if (newCustomer.phoneNumber == "0") {
			cout << " Registration cancelled." << endl;
			return;
		}
		if (newCustomer.phoneNumber.empty()) {
			cout << " Phone number cannot be empty!" << endl;
			continue;
		}
		if (!isValidPhoneNumber(newCustomer.phoneNumber)) {
			cout << " Invalid format! Use: +60123456789 or 0123456789" << endl;
			continue;
		}
		cout << " Valid Malaysian phone number!" << endl;
		break;
	}

	while (true) {
		cout << "\n Enter IC number (12 digits) or Passport number (or 0 to cancel): ";
		getline(cin, newCustomer.icPassport);
		if (newCustomer.icPassport == "0") {
			cout << " Registration cancelled." << endl;
			return;
		}
		if (newCustomer.icPassport.empty()) {
			cout << " IC/Passport cannot be empty!" << endl;
			continue;
		}
		if (isValidMalaysianIC(newCustomer.icPassport) || isValidPassport(newCustomer.icPassport)) {
			cout << " Valid IC/Passport!" << endl;
			break;
		}
		cout << " Invalid IC/Passport format!" << endl;
	}

	newCustomer.membershipStatus = "Regular";
	newCustomer.loyaltyPoints = 0;
	customers.push_back(newCustomer);
	saveCustomersToFile();
	cout << "\n Registration successful! Welcome, " << newCustomer.fullName << "!" << endl;
	cout << " You can now login with username: " << newCustomer.username << endl;
	cout << " Loyalty membership started at Regular (0 points)." << endl;
	cout << " Earn 1 point for every RM 10 spent. Silver = 5% off, Gold = 10% off." << endl;
}

bool customerLogin() {
	string username;
	string password;
	int attempts = 0;
	const int MAX_ATTEMPTS = 3;

	while (attempts < MAX_ATTEMPTS) {
		cout << "\n ===== Customer Login =====" << endl;
		if (attempts > 0) {
			cout << " Attempt " << attempts + 1 << " of " << MAX_ATTEMPTS << endl;
		}

		cout << " Username or 0 to cancel: ";
		getline(cin, username);
		if (username == "0") {
			cout << " Login cancelled." << endl;
			return false;
		}

		cout << " Password or 0 to cancel: ";
		password = getSecurePassword(false);
		if (password == "0") {
			cout << " Login cancelled." << endl;
			return false;
		}

		for (size_t i = 0; i < customers.size(); i++) {
			if (customers[i].username == username && customers[i].password == password) {
				currentLoggedInCustomer = customers[i].username;
				cout << "\n Login successful! Welcome back, " << customers[i].fullName << "!" << endl;
				cout << " Membership: " << customers[i].membershipStatus
					 << "  |  Points: " << customers[i].loyaltyPoints << endl;
				return true;
			}
		}

		attempts++;
		if (attempts < MAX_ATTEMPTS) {
			cout << " Login failed! Invalid username or password." << endl;
			cout << " You have " << (MAX_ATTEMPTS - attempts) << " attempt(s) remaining." << endl;
			if (!confirmYesNo(" Would you like to try again? y/n: ")) {
				cout << " Login cancelled." << endl;
				return false;
			}
		}
		else {
			cout << " Login failed! Maximum attempts exceeded." << endl;
			cout << " Access denied. Returning to main menu." << endl;
			return false;
		}
	}
	return false;
}

void customerMenu() {
	int choice;

	do {
		showUnpaidReminder();
		cout << endl;
		boxTitle("Menu Page");
		boxRow("1. View Available Rooms");
		boxRow("2. Booking Room");
		boxRow("3. View My Reservations");
		boxRow("4. Modify Reservations");
		boxRow("5. Cancel Reservations");
		boxRow("6. View My Profile");
		boxRow("0. Back to Main Menu");
		boxLine();
		cout << " Please choose 0-6: ";
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
			cout << " 4. Module. To be continued..." << endl;
			break;
		case 5:
			cout << " 5. Module. To be continued..." << endl;
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

void viewMyReservations() {
	cout << endl;
	boxLine();
	boxCenter("YOUR BOOKING HISTORY");
	boxSplitLine();
	int count = 0;

	for (size_t i = 0; i < reservations.size(); i++) {
		if (reservations[i].customerUsername != currentLoggedInCustomer) {
			continue;
		}
		count++;
		boxSplitRow("Reservation #" + to_string(count), "ID: " + reservations[i].reservationID);
		boxSplitRow("Room:", reservations[i].roomNumber + " " + reservations[i].roomType);
		boxSplitRow("Guests:", to_string(reservations[i].guests));
		boxSplitRow("Nights:", to_string(reservations[i].nights) + " night(s)");
		boxSplitRow("Booking Date:", reservations[i].bookingDate + "  " + reservations[i].bookingTime);
		{
			int d = 0;
			int m = 0;
			int y = 0;
			string inText = reservations[i].checkInDate + "  " + reservations[i].checkInTime;
			if (parseDate(reservations[i].checkInDate, d, m, y)) {
				inText = weekdayName(d, m, y) + " " + inText;
			}
			boxSplitRow("Check-in:", inText);
		}
		{
			int d = 0;
			int m = 0;
			int y = 0;
			string outText = reservations[i].checkOutDate + "  before 12:00 noon";
			if (parseDate(reservations[i].checkOutDate, d, m, y)) {
				outText = weekdayName(d, m, y) + " " + outText;
			}
			boxSplitRow("Check-out:", outText);
		}
		if (reservations[i].paymentStatus == "Paid") {
			ostringstream amount;
			amount << fixed << setprecision(2) << "RM " << reservations[i].totalAmount;
			boxSplitRow("Amount:", amount.str());
			boxSplitRow("Payment Method:", reservations[i].paymentMethod);
		}
		else {
			boxSplitRow("Amount:", "Unpaid");
			boxSplitRow("Payment Method:", "-");
		}
		boxSplitRow("Status:", reservations[i].status);
		if (!reservations[i].addOns.empty()) {
			string addText = "";
			for (size_t j = 0; j < reservations[i].addOns.size(); j++) {
				if (j > 0) {
					addText += ", ";
				}
				addText += reservations[i].addOns[j].name + " x" + to_string(reservations[i].addOns[j].quantity);
			}
			boxSplitRow("Add-ons:", addText);
		}
		if (reservations[i].specialRequest != "-" && !reservations[i].specialRequest.empty()) {
			boxSplitRow("Request:", reservations[i].specialRequest);
		}
		if (reservations[i].paymentStatus == "Unpaid") {
			boxSplitRow("Note:", "Continue from 2. Booking Room");
		}
		boxSplitLine();
	}

	if (count == 0) {
		boxSplitRow("Total Bookings:", "0 booking(s)");
		boxLine();
	}
	else {
		boxSplitRow("Total Bookings:", to_string(count) + " booking(s)");
		boxLine();
	}
}

void viewMyProfile() {
	int idx = findCustomerIndex(currentLoggedInCustomer);
	if (idx == -1) {
		cout << " Profile not found." << endl;
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

	cout << endl;
	boxTitle("My Profile");
	boxRow("Username    : " + c.username);
	boxRow("Full name   : " + c.fullName);
	boxRow("Age         : " + c.age);
	boxRow("Email       : " + c.email);
	boxRow("Phone       : " + c.phoneNumber);
	boxRow("IC/Passport : " + c.icPassport);
	boxRow("Member      : " + c.membershipStatus);
	{
		ostringstream line;
		line << "Points      : " << c.loyaltyPoints;
		boxRow(line.str());
	}
	if (c.membershipStatus != "Gold") {
		ostringstream line;
		line << "Next tier   : " << nextNeed << " more point(s) to " << nextTier;
		boxRow(line.str());
	}
	else {
		boxRow("Next tier   : You are at the highest tier!");
	}
	boxRow("Perks       : Silver 5% off, Gold 10% off");
	boxLine();
}

void showUnpaidReminder() {
	bool any = false;
	for (size_t i = 0; i < reservations.size(); i++) {
		if (reservations[i].customerUsername != currentLoggedInCustomer) {
			continue;
		}
		if (reservations[i].paymentStatus != "Unpaid") {
			continue;
		}
		if (reservations[i].status == "Cancelled") {
			continue;
		}
		if (!any) {
			cout << "\n You still have a booking in progress:" << endl;
			any = true;
		}
		cout << " #" << reservations[i].reservationID
			 << "  Room " << reservations[i].roomNumber
			 << "  " << reservations[i].roomType
			 << "  " << reservations[i].paymentStatus << endl;
	}
	if (any) {
		cout << " This booking is still yours. It did not disappear." << endl;
		cout << " Choose 2. Booking Room to continue add-ons / payment." << endl;
		cout << " Choose 3. View My Reservations to see the details." << endl;
	}
}
