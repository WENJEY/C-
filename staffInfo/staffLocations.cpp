#include "staff_internal.h"

void staffLocationMenu() {
	int choice;
	do {
		cout << endl;
		boxTitle("Manage locations");
		boxRow(optionText(1) + "View hotels");
		boxRow(optionText(2) + "Add a hotel");
		boxRow(optionText(3) + "Edit a hotel");
		boxRow(optionText(4) + "Delete a hotel");
		boxRow(optionText(0) + "Back");
		boxLine();
		cout << " Please choose 0-4: ";
		choice = getIntInRange(0, 4);
		if (choice == 1) {
			staffListHotels();
		}
		else if (choice == 2) {
			staffAddHotel();
		}
		else if (choice == 3) {
			staffEditHotel();
		}
		else if (choice == 4) {
			staffDeleteHotel();
		}
	} while (choice != 0);
}

void staffListHotels() {
	string state = pickStateForStaff(false);
	if (state.empty()) {
		return;
	}
	string area = pickAreaForStaff(state, false);
	if (area.empty()) {
		return;
	}

	vector<int> list = hotelsOf(state, area);
	cout << endl;
	boxTitle("Hotels in " + area + ", " + state);
	if (list.empty()) {
		boxRow("No hotel in this area.");
		boxLine();
		pauseEnter();
		return;
	}
	for (size_t i = 0; i < list.size(); i++) {
		int idx = list[i];
		boxRow(optionText(static_cast<int>(i + 1)) + hotelBranches[static_cast<size_t>(idx)].name);
		boxWrapHang(string(4, ' ') + hotelBranches[static_cast<size_t>(idx)].address, 4);
		if (i + 1 < list.size()) {
			boxRow("");
		}
	}
	boxLine();
	pauseEnter();
}

void staffAddHotel() {
	string state = pickStateForStaff(true);
	if (state.empty()) {
		return;
	}
	string area = pickAreaForStaff(state, true);
	if (area.empty()) {
		return;
	}

	HotelBranch hotel;
	hotel.state = state;
	hotel.area = area;
	if (!askLine(" Hotel name (or 0 to cancel): ", hotel.name)) {
		return;
	}
	if (!askLine(" Address (or 0 to cancel): ", hotel.address)) {
		return;
	}

	hotelBranches.push_back(hotel);
	cout << endl;
	boxTitle("Hotel added");
	boxField("Hotel   : ", hotel.name);
	boxField("Area    : ", hotel.area + ", " + hotel.state);
	boxField("Address : ", hotel.address);
	boxLine();
	cout << " Customers can pick this hotel when they book." << endl;
}

void staffEditHotel() {
	string state = pickStateForStaff(false);
	if (state.empty()) {
		return;
	}
	string area = pickAreaForStaff(state, false);
	if (area.empty()) {
		return;
	}
	int idx = pickHotelIndexForStaff(state, area);
	if (idx < 0) {
		return;
	}

	showStaffHotel(idx);
	boxRow(optionText(1) + "Change name");
	boxRow(optionText(2) + "Change address");
	boxRow(optionText(3) + "Move to another state / area");
	boxRow(optionText(0) + "Back");
	boxLine();
	cout << " Please choose 0-3: ";
	int choice = getIntInRange(0, 3);
	if (choice == 0) {
		return;
	}
	if (choice == 1) {
		string name;
		if (askLine(" New hotel name (or 0 to cancel): ", name)) {
			string oldName = hotelBranches[static_cast<size_t>(idx)].name;
			hotelBranches[static_cast<size_t>(idx)].name = name;
			if (currentHotelName == oldName) {
				currentHotelName = name;
			}
			cout << " Name updated." << endl;
		}
	}
	else if (choice == 2) {
		string address;
		if (askLine(" New address (or 0 to cancel): ", address)) {
			hotelBranches[static_cast<size_t>(idx)].address = address;
			if (currentHotelName == hotelBranches[static_cast<size_t>(idx)].name) {
				currentHotelAddress = address;
			}
			cout << " Address updated." << endl;
		}
	}
	else if (choice == 3) {
		string newState = pickStateForStaff(true);
		if (newState.empty()) {
			return;
		}
		string newArea = pickAreaForStaff(newState, true);
		if (newArea.empty()) {
			return;
		}
		hotelBranches[static_cast<size_t>(idx)].state = newState;
		hotelBranches[static_cast<size_t>(idx)].area = newArea;
		if (currentHotelName == hotelBranches[static_cast<size_t>(idx)].name) {
			currentHotelState = newState;
			currentHotelArea = newArea;
		}
		cout << " Location updated." << endl;
	}
}

void staffDeleteHotel() {
	string state = pickStateForStaff(false);
	if (state.empty()) {
		return;
	}
	string area = pickAreaForStaff(state, false);
	if (area.empty()) {
		return;
	}
	int idx = pickHotelIndexForStaff(state, area);
	if (idx < 0) {
		return;
	}

	showStaffHotel(idx);
	if (!confirmYesNo(" Delete this hotel from the list? y/n: ")) {
		cout << " Hotel kept." << endl;
		return;
	}

	string removedName = hotelBranches[static_cast<size_t>(idx)].name;
	hotelBranches.erase(hotelBranches.begin() + static_cast<size_t>(idx));
	if (currentHotelName == removedName) {
		clearCurrentHotel();
	}
	cout << " Hotel deleted. Old bookings still keep this hotel name in history." << endl;
}
