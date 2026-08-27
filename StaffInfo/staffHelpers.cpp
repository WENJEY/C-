#include "../staff_internal.h"

bool roomHasActiveBooking(const string& roomNumber) {
	for (size_t i = 0; i < reservations.size(); i++) {
		if (reservations[i].roomNumber != roomNumber) {
			continue;
		}
		if (reservations[i].status == "Cancelled") {
			continue;
		}
		return true;
	}
	return false;
}

bool askLine(const string& prompt, string& out) {
	while (true) {
		cout << prompt;
		getline(cin, out);
		cout << endl;
		if (out == "0") {
			cout << " Cancelled." << endl;
			pauseEnter();
			return false;
		}
		if (out.empty()) {
			cout << red << " Cannot be empty." << original << endl;
			continue;
		}
		return true;
	}
}

bool askMoney(const string& prompt, double& amount) {
	while (true) {
		cout << prompt;
		string text;
		getline(cin, text);
		cout << endl;
		if (text == "0") {
			cout << " Cancelled." << endl;
			pauseEnter();
			return false;
		}

		bool ok = true;
		int dots = 0;
		if (text.empty()) {
			ok = false;
		}
		for (size_t i = 0; i < text.length(); i++) {
			if (text[i] == '.') {
				dots++;
			}
			else if (!isdigit(static_cast<unsigned char>(text[i]))) {
				ok = false;
			}
		}
		if (!ok || dots > 1) {
			cout << red << " Enter a number such as 150 or 150.50." << original << endl;
			continue;
		}

		amount = roundMoney(stod(text));
		if (amount <= 0) {
			cout << red << " Price must be more than 0." << original << endl;
			continue;
		}
		return true;
	}
}

string pickStateForStaff(bool allowNew) {
	vector<string> states = destinationStates();
	showPage("Which state?");
	for (size_t i = 0; i < states.size(); i++) {
		boxRow(optionText(static_cast<int>(i + 1)) + states[i]);
	}
	int extra = 0;
	if (allowNew) {
		extra = 1;
		boxRow(optionText(static_cast<int>(states.size() + 1)) + "Type a new state");
	}
	boxRow(optionText(0) + "Cancel");
	boxLine();
	int maxChoice = static_cast<int>(states.size()) + extra;
	if (states.empty() && !allowNew) {
		cout << red << " No hotel yet. Add a hotel first." << original << endl;
		pauseEnter();
		return "";
	}
	if (maxChoice == 0) {
		string state;
		if (!askLine(" New state name (or 0 to cancel): ", state)) {
			return "";
		}
		return state;
	}
	cout << " Please choose 0-" << maxChoice << ": ";
	int choice = getIntInRange(0, maxChoice);
	if (choice == 0) {
		cout << " Cancelled." << endl;
		pauseEnter();
		return "";
	}
	if (allowNew && choice == static_cast<int>(states.size()) + 1) {
		string state;
		if (!askLine(" New state name (or 0 to cancel): ", state)) {
			return "";
		}
		return state;
	}
	return states[static_cast<size_t>(choice - 1)];
}

string pickAreaForStaff(const string& state, bool allowNew) {
	vector<string> areas = areasOf(state);
	showPage("Which area in " + state + "?");
	for (size_t i = 0; i < areas.size(); i++) {
		boxRow(optionText(static_cast<int>(i + 1)) + areas[i]);
	}
	int extra = 0;
	if (allowNew) {
		extra = 1;
		boxRow(optionText(static_cast<int>(areas.size() + 1)) + "Type a new area");
	}
	boxRow(optionText(0) + "Cancel");
	boxLine();
	int maxChoice = static_cast<int>(areas.size()) + extra;
	if (areas.empty() && !allowNew) {
		cout << red << " No area in this state yet." << original << endl;
		pauseEnter();
		return "";
	}
	if (maxChoice == 0) {
		string area;
		if (!askLine(" New area name (or 0 to cancel): ", area)) {
			return "";
		}
		return area;
	}
	cout << " Please choose 0-" << maxChoice << ": ";
	int choice = getIntInRange(0, maxChoice);
	if (choice == 0) {
		cout << " Cancelled." << endl;
		pauseEnter();
		return "";
	}
	if (allowNew && choice == static_cast<int>(areas.size()) + 1) {
		string area;
		if (!askLine(" New area name (or 0 to cancel): ", area)) {
			return "";
		}
		return area;
	}
	return areas[static_cast<size_t>(choice - 1)];
}

int pickHotelIndexForStaff(const string& state, const string& area) {
	vector<int> list = hotelsOf(state, area);
	if (list.empty()) {
		cout << red << " No hotel in this area." << original << endl;
		pauseEnter();
		return -1;
	}
	showPage("Choose a hotel");
	for (size_t i = 0; i < list.size(); i++) {
		int idx = list[i];
		boxRow(optionText(static_cast<int>(i + 1)) + hotelBranches[static_cast<size_t>(idx)].name);
		boxWrapHang(string(4, ' ') + hotelBranches[static_cast<size_t>(idx)].address, 4);
		if (i + 1 < list.size()) {
			boxRow("");
		}
	}
	boxRow(optionText(0) + "Cancel");
	boxLine();
	cout << " Please choose 0-" << list.size() << ": ";
	int pick = getIntInRange(0, static_cast<int>(list.size()));
	if (pick == 0) {
		cout << " Cancelled." << endl;
		pauseEnter();
		return -1;
	}
	return list[static_cast<size_t>(pick - 1)];
}

string pickRoomType() {
	showPage("Room type");
	boxRow(optionText(1) + "Single");
	boxRow(optionText(2) + "Twin");
	boxRow(optionText(3) + "Deluxe");
	boxRow(optionText(4) + "Family");
	boxRow(optionText(5) + "Suite");
	boxRow(optionText(6) + "Presidential");
	boxRow(optionText(7) + "Type another name");
	boxRow(optionText(0) + "Cancel");
	boxLine();
	cout << " Please choose 0-7: ";
	int choice = getIntInRange(0, 7);
	if (choice == 0) {
		cout << " Cancelled." << endl;
		pauseEnter();
		return "";
	}
	if (choice == 1) {
		return "Single";
	}
	if (choice == 2) {
		return "Twin";
	}
	if (choice == 3) {
		return "Deluxe";
	}
	if (choice == 4) {
		return "Family";
	}
	if (choice == 5) {
		return "Suite";
	}
	if (choice == 6) {
		return "Presidential";
	}
	string type;
	if (!askLine(" Type name (or 0 to cancel): ", type)) {
		return "";
	}
	return type;
}

string pickRoomStatus() {
	showPage("Room status");
	boxRow(optionText(1) + "Available");
	boxRow(optionText(2) + "Occupied");
	boxRow(optionText(3) + "Cleaning");
	boxRow(optionText(4) + "Maintenance");
	boxRow(optionText(0) + "Cancel");
	boxLine();
	cout << " Please choose 0-4: ";
	int choice = getIntInRange(0, 4);
	if (choice == 0) {
		cout << " Cancelled." << endl;
		pauseEnter();
		return "";
	}
	if (choice == 1) {
		return "Available";
	}
	if (choice == 2) {
		return "Occupied";
	}
	if (choice == 3) {
		return "Cleaning";
	}
	return "Maintenance";
}

void showStaffHotel(int idx) {
	showPage("Hotel");
	boxField("Hotel   : ", hotelBranches[static_cast<size_t>(idx)].name);
	boxField("Area    : ", hotelBranches[static_cast<size_t>(idx)].area
		+ ", " + hotelBranches[static_cast<size_t>(idx)].state);
	boxField("Address : ", hotelBranches[static_cast<size_t>(idx)].address);
	boxLine();
}
