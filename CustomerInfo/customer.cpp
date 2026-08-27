#include "../hotel.h"

void customerAuthMenu() {
	int choice;
	do {
		showPage("Customer Access");
		boxRow("1. Login to Existing Account");
		boxRow("2. Register New Account");
		boxRow("0. Back to Main Menu");
		boxLine();
		cout << " Please choose 0-2: ";
		choice = getValidatedInput(0, 2);

		switch (choice) {
		case 1:
			if (customerLogin()) {
				restoreHotelFromUnpaid();
				customerMenu();
			}
			break;

		case 2:
			customerRegister();
			break;
		case 0:
			return;
		}
	} while (true);
}

void customerRegister() {
	Customer newCustomer;
	string confirmPassword;

	showPage("Customer Registration");
	boxRow("Please fill in the following information");
	boxRow("Enter 0 at any field to cancel");
	boxLine();

	while (true) {
		cout << "\n Enter username (3-20 characters, letters/numbers only): ";
		getline(cin, newCustomer.username);

		cout << endl;
		if (newCustomer.username == "0") {
			cout << " Registration cancelled." << endl;
			pauseEnter();
			return;
		}
		if (newCustomer.username.empty()) {
			cout << red << " Username cannot be empty!" << original << endl;
			continue;
		}
		if (newCustomer.username.length() < 3 || newCustomer.username.length() > 20) {
			cout << red << " Username must be 3-20 characters long!" << original << endl;
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
			cout << red << " Username can only contain letters and numbers!" << original << endl;
			continue;
		}
		if (customerExists(newCustomer.username)) {
			cout << red << " Username already exists! Please choose another." << original << endl;
			continue;
		}
		break;
	}

	while (true) {
		cout << "\n Enter your age (or 0 to cancel): ";
		getline(cin, newCustomer.age);

		cout << endl;
		if (newCustomer.age == "0") {
			cout << " Registration cancelled." << endl;
			pauseEnter();
			return;
		}
		if (!isValidAge(newCustomer.age)) {
			cout << red << " You must be 18 years old and above to book our hotel." << original << endl;
			continue;
		}
		break;
	}

	while (true) {
		cout << "\n Enter password (minimum 6 characters or 0 to cancel): ";
		newCustomer.password = getSecurePassword(false);

		cout << endl;
		if (newCustomer.password == "0") {
			cout << " Registration cancelled." << endl;
			pauseEnter();
			return;
		}
		if (!isValidPassword(newCustomer.password)) {
			cout << red << " Password must be at least 6 characters long!" << original << endl;
			continue;
		}

		cout << "\n Confirm password (or 0 to cancel): ";
		confirmPassword = getSecurePassword(false);

		cout << endl;
		if (confirmPassword == "0") {
			cout << " Registration cancelled." << endl;
			pauseEnter();
			return;
		}
		if (newCustomer.password != confirmPassword) {
			cout << red << " Passwords do not match! Please try again." << original << endl;
			continue;
		}
		cout << " Password confirmed successfully!" << endl;
		break;
	}

	while (true) {
		cout << "\n Enter full name (or 0 to cancel): ";
		getline(cin, newCustomer.fullName);

		cout << endl;
		if (newCustomer.fullName == "0") {
			cout << " Registration cancelled." << endl;
			pauseEnter();
			return;
		}
		if (newCustomer.fullName.empty()) {
			cout << red << " Full name cannot be empty!" << original << endl;
			continue;
		}
		if (!isValidName(newCustomer.fullName)) {
			cout << red << " Invalid name format! Names should contain only letters and spaces (2-50 characters)." << original << endl;
			continue;
		}
		formatName(newCustomer.fullName);
		break;
	}

	while (true) {
		cout << "\n Enter Gmail address (example: yourname@gmail.com or 0 to cancel): ";
		getline(cin, newCustomer.email);

		cout << endl;
		if (newCustomer.email == "0") {
			cout << " Registration cancelled." << endl;
			pauseEnter();
			return;
		}
		if (newCustomer.email.empty()) {
			cout << red << " Email cannot be empty!" << original << endl;
			continue;
		}
		if (!isValidEmail(newCustomer.email)) {
			cout << red << " Invalid email! Please use Gmail format: yourname@gmail.com" << original << endl;
			continue;
		}
		cout << " Valid Gmail address!" << endl;
		break;
	}

	while (true) {
		cout << "\n Enter Malaysian phone number (+60xxxxxxxxx or 01xxxxxxxx or 0 to cancel): ";
		getline(cin, newCustomer.phoneNumber);

		cout << endl;
		if (newCustomer.phoneNumber == "0") {
			cout << " Registration cancelled." << endl;
			pauseEnter();
			return;
		}
		if (newCustomer.phoneNumber.empty()) {
			cout << red << " Phone number cannot be empty!" << original << endl;
			continue;
		}
		if (!isValidPhoneNumber(newCustomer.phoneNumber)) {
			cout << red << " Invalid format! Use: +60123456789 or 0123456789" << original << endl;
			continue;
		}
		cout << " Valid Malaysian phone number!" << endl;
		break;
	}

	while (true) {
		cout << "\n Enter IC number (12 digits) or Passport number (or 0 to cancel): ";
		getline(cin, newCustomer.icPassport);

		cout << endl;
		if (newCustomer.icPassport == "0") {
			cout << " Registration cancelled." << endl;
			pauseEnter();
			return;
		}
		if (newCustomer.icPassport.empty()) {
			cout << red << " IC/Passport cannot be empty!" << original << endl;
			continue;
		}
		if (isValidMalaysianIC(newCustomer.icPassport) || isValidPassport(newCustomer.icPassport)) {
			cout << " Valid IC/Passport!" << endl;
			break;
		}
		cout << red << " Invalid IC/Passport format!" << original << endl;
	}

	newCustomer.membershipStatus = "Regular";
	newCustomer.loyaltyPoints = 0;
	customers.push_back(newCustomer);
	saveCustomersToFile();
	cout << "\n Registration successful! Welcome, " << newCustomer.fullName << "!" << endl;
	cout << " You can now login with username: " << newCustomer.username << endl;
	cout << " Loyalty membership started at Regular (0 points)." << endl;
	cout << " Earn 1 point for every RM 10 spent. Silver = 5% off, Gold = 10% off." << endl;
	loadingPause();
}

bool customerLogin() {
	string username;
	string password;
	int attempts = 0;
	const int MAX_ATTEMPTS = 3;
	customers[0]= {"1","1","1","1","1","1"};

	while (attempts < MAX_ATTEMPTS) {
		showPage("Customer Login");
		if (attempts > 0) {
			ostringstream line;
			line << "Attempt " << attempts + 1 << " of " << MAX_ATTEMPTS;
			boxRow(line.str());
			boxLine();
		}
		else {
			boxRow("Enter 0 on username to cancel");
			boxLine();
		}

		cout << " Username or 0 to cancel: ";
		getline(cin, username);
		cout << endl;
		if (username == "0") {
			cout << " Login cancelled." << endl;
			pauseEnter();
			return false;
		}

		cout << " Password or 0 to cancel: ";
		password = getSecurePassword(false);
		if (password == "0") {
			cout << " Login cancelled." << endl;
			pauseEnter();
			return false;
		}

		for (size_t i = 0; i < customers.size(); i++) {
			if (customers[i].username == username && customers[i].password == password) {
				currentLoggedInCustomer = customers[i].username;
				if (customers[i].fullName.empty() || customers[i].fullName == "---") {
					customers[i].fullName = customers[i].username;
				}
				cout << "\n Login successful! Welcome back, " << currentCustomerName() << "!" << endl;
				cout << " Membership: " << customers[i].membershipStatus
					 << "  |  Points: " << customers[i].loyaltyPoints << endl;
				loadingPause();
				return true;
			}
		}

		attempts++;
		if (attempts < MAX_ATTEMPTS) {
			cout << red << " Login failed! Invalid username or password." << original << endl;
			cout << " You have " << (MAX_ATTEMPTS - attempts) << " attempt(s) remaining." << endl;
			if (!confirmYesNo(" Would you like to try again? y/n: ")) {
				cout << " Login cancelled." << endl;
				pauseEnter();
				return false;
			}
		}
		else {
			cout << red << " Login failed! Maximum attempts exceeded." << original << endl;
			cout << red << " Access denied. Returning to main menu." << original << endl;
			pauseEnter();
			return false;
		}
	}
	return false;
}

void customerMenu() {
	int choice;

	do {
		showPage("Menu Page");
		if (!currentHotelName.empty()) {
			boxField("Hotel : ", currentHotelName);
			boxField("Area  : ", currentHotelArea + ", " + currentHotelState);
			boxField("Addr  : ", currentHotelAddress);
			boxLine();
		}
		showUnpaidReminder();
		boxRow(optionText(1) + "View Available Rooms");
		if (currentUserHasUnpaid()) {
			boxRow(optionText(2) + "Continue Payment / Book Another Room");
		}
		else {
			boxRow(optionText(2) + "Booking Room");
		}
		boxRow(optionText(3) + "View My Reservations");
		boxRow(optionText(4) + "Modify Reservations");
		boxRow(optionText(5) + "Cancel Reservations");
		boxRow(optionText(6) + "View My Profile");
		boxRow(optionText(7) + "Change Destination");
		boxRow(optionText(0) + "Back to Main Menu");
		boxLine();
		cout << " Please choose 0-7: ";
		choice = getValidatedInput(0, 7);

		switch (choice) {
		case 1:
			if (requireHotelSelected()) {
				displayAvailableRoom();
			}
			break;
		case 2:
			if (requireHotelSelected()) {
				bookRoom();
			}
			break;
		case 3:
			viewMyReservations();
			break;
		case 4:
			modifyReservations();
			break;
		case 5:
			cancelReservations();
			break;
		case 6:
			viewMyProfile();
			if (currentLoggedInCustomer.empty()) {
				return;
			}
			break;
		case 7:
			changeDestination();
			break;
		case 0:
			currentLoggedInCustomer = "";
			clearCurrentHotel();
			resetOccupiedRooms();
			resetSessionExtras();
			currentSessionIDs.clear();
			cout << "\n Logged out successfully." << endl;
			loadingPause();
			return;
		}
	} while (choice != 0);
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
			boxRow("You still have unpaid booking(s):");
			any = true;
		}
		ostringstream line;
		line << "#" << reservations[i].reservationID
			 << "  Room " << reservations[i].roomNumber
			 << "  " << reservations[i].roomType;
		boxRow(line.str());
	}
	if (any) {
		boxRow("Choose 2 to pay, or 3 to view details.");
		boxLine();
	}
}

bool currentUserHasUnpaid() {
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
		return true;
	}
	return false;
}
