#include "../staff_internal.h"

void staffLogin() {
	string id;
	string password;
	int attempts = 0;
	const int MAX_ATTEMPTS = 3;

	while (attempts < MAX_ATTEMPTS) {
		showPage("Staff Login");
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
		getline(cin, id);
		cout << endl;
		if (id == "0") {
			cout << " Login cancelled." << endl;
			pauseEnter();
			return;
		}

		cout << " Password or 0 to cancel: ";
		password = getSecurePassword(false);
		if (password == "0") {
			cout << " Login cancelled." << endl;
			pauseEnter();
			return;
		}

		if (id == STAFF_ID && password == STAFF_PASSWORD) {
			cout << " Login successful! Welcome, " << id << "!" << endl;
			loadingPause();
			staffMenu();
			return;
		}

		attempts++;
		if (attempts < MAX_ATTEMPTS) {
			cout << red << " Login failed! Invalid username or password." << original << endl;
			cout << " You have " << (MAX_ATTEMPTS - attempts) << " attempt(s) remaining." << endl;
			if (!confirmYesNo(" Would you like to try again? y/n: ")) {
				cout << " Login cancelled." << endl;
				pauseEnter();
				return;
			}
		}
		else {
			cout << red << " Login failed! Maximum attempts exceeded." << original << endl;
			cout << red << " Access denied. Returning to main menu." << original << endl;
			pauseEnter();
			return;
		}
	}
}

void staffMenu() {
	int choice;
	do {
		showPage("Staff Menu");
		boxRow("Room and location changes last until you close the program.");
		boxLine();
		boxRow(optionText(1) + "Manage rooms");
		boxRow(optionText(2) + "Manage locations");
		boxRow(optionText(3) + "Reports and statistics");
		boxRow(optionText(4) + "View all bookings");
		boxRow(optionText(5) + "Search customer");
		boxRow(optionText(6) + "Today's check-ins");
		boxRow(optionText(0) + "Logout");
		boxLine();
		cout << " Please choose 0-6: ";
		choice = getIntInRange(0, 6);

		switch (choice) {
		case 1:
			staffRoomMenu();
			break;
		case 2:
			staffLocationMenu();
			break;
		case 3:
			staffReports();
			break;
		case 4:
			staffViewAllBookings();
			break;
		case 5:
			staffSearchCustomer();
			break;
		case 6:
			staffTodayCheckIns();
			break;
		case 0:
			cout << "\n Staff logged out." << endl;
			loadingPause();
			return;
		}
	} while (choice != 0);
}
