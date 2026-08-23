#include "staff.h"
#include "common.h"
#include "globals.h"
#include "ui.h"
#include "validation.h"

void staffLogin() {
	string id;
	string password;
	int attempts = 0;
	const int MAX_ATTEMPTS = 3;

	while (attempts < MAX_ATTEMPTS) {
		cout << endl;
		boxTitle("Staff Login");
		if (attempts > 0) {
			ostringstream line;
			line << "Attempt " << attempts + 1 << " of " << MAX_ATTEMPTS;
			boxRow(line.str());
			boxLine();
		}

		cout << " Username or 0 to cancel: ";
		getline(cin, id);
		if (id == "0") {
			cout << " Login cancelled." << endl;
			return;
		}

		cout << " Password or 0 to cancel: ";
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
			if (!confirmYesNo(" Would you like to try again? y/n: ")) {
				cout << " Login cancelled." << endl;
				return;
			}
		}
		else {
			cout << " Login failed! Maximum attempts exceeded." << endl;
			cout << " Access denied. Returning to main menu." << endl;
			return;
		}
	}
}

void staffMenu() {
	cout << "\n Staff module. To be continued..." << endl;
	pauseEnter();
}
