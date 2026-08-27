#include "../hotel.h"

vector<string> areasOf(const string& state);
vector<int> hotelsOf(const string& state, const string& area);
string chooseState();
string chooseArea(const string& state);
bool chooseHotel(const string& state, const string& area);
bool applyHotel(int hotelIndex);
bool unpaidBlocksHotelChange();

bool restoreHotelFromUnpaid() {
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
		if (reservations[i].hotelName.empty()) {
			continue;
		}

		currentHotelName = reservations[i].hotelName;
		currentHotelAddress = reservations[i].hotelAddress;
		currentHotelArea = reservations[i].hotelArea;
		currentHotelState = reservations[i].hotelState;

		showPage("Unpaid booking");
		boxField("Hotel   : ", currentHotelName);
		boxField("Area    : ", currentHotelArea + ", " + currentHotelState);
		boxField("Address : ", currentHotelAddress);
		boxRow("Finish payment or cancel it before choosing another hotel.");
		boxLine();
		pauseEnter();
		return true;
	}
	return false;
}

bool unpaidBlocksHotelChange() {
	if (currentUserHasUnpaid() && !currentHotelName.empty()) {
		cout << "\n You still have an unpaid booking at " << currentHotelName << "." << endl;
		cout << " Finish payment or cancel it before choosing another hotel." << endl;
		pauseEnter();
		return true;
	}
	return false;
}

bool findHotelByDestination() {
	if (unpaidBlocksHotelChange()) {
		return false;
	}

	showPage("Choose a destination");
	boxRow("Pick a state, then an area, then a hotel.");
	boxRow("The hotel address is shown before you book.");
	boxLine();
	pauseEnter();

	string state = chooseState();
	if (state.empty()) {
		cout << " Destination cancelled." << endl;
		pauseEnter();
		return false;
	}

	string area = chooseArea(state);
	if (area.empty()) {
		cout << " Destination cancelled." << endl;
		pauseEnter();
		return false;
	}

	return chooseHotel(state, area);
}

bool requireHotelSelected() {
	if (!currentHotelName.empty()) {
		return true;
	}
	cout << "\n Please choose a state, area and hotel first." << endl;
	pauseEnter();
	return findHotelByDestination();
}

void changeDestination() {
	if (currentHotelName.empty()) {
		showPage("Change destination");
		boxRow("You have not chosen a hotel yet.");
		boxRow("Use 1. View Rooms or 2. Booking Room first.");
		boxRow("Choose a state, area and hotel there.");
		boxLine();
		pauseEnter();
		return;
	}
	if (unpaidBlocksHotelChange()) {
		return;
	}

	showPage("Change destination");
	boxField("Now     : ", currentHotelName);
	boxField("Area    : ", currentHotelArea + ", " + currentHotelState);
	boxLine();
	boxRow(optionText(1) + "Change state");
	boxRow(optionText(2) + "Change area");
	boxRow(optionText(3) + "Change hotel");
	boxRow(optionText(0) + "Keep this hotel");
	boxLine();
	cout << " Please choose 0-3: ";
	int choice = getIntInRange(0, 3);

	if (choice == 0) {
		return;
	}
	if (choice == 1) {
		findHotelByDestination();
		return;
	}
	if (choice == 2) {
		string area = chooseArea(currentHotelState);
		if (area.empty()) {
			cout << " Area change cancelled." << endl;
			pauseEnter();
			return;
		}
		chooseHotel(currentHotelState, area);
		return;
	}

	chooseHotel(currentHotelState, currentHotelArea);
}

vector<string> destinationStates() {
	const int PREFERRED = 14;
	const string preferred[PREFERRED] = {
		"Perlis", "Kedah", "Pulau Pinang", "Perak", "Selangor", "Kuala Lumpur",
		"Negeri Sembilan", "Melaka", "Johor", "Pahang", "Terengganu", "Kelantan",
		"Sabah", "Sarawak"
	};
	vector<string> states;

	for (int p = 0; p < PREFERRED; p++) {
		for (size_t i = 0; i < hotelBranches.size(); i++) {
			if (hotelBranches[i].state == preferred[p]) {
				states.push_back(preferred[p]);
				break;
			}
		}
	}

	for (size_t i = 0; i < hotelBranches.size(); i++) {
		bool seen = false;
		for (size_t j = 0; j < states.size(); j++) {
			if (states[j] == hotelBranches[i].state) {
				seen = true;
				break;
			}
		}
		if (!seen) {
			states.push_back(hotelBranches[i].state);
		}
	}
	return states;
}

string chooseState() {
	vector<string> states = destinationStates();
	if (states.empty()) {
		cout << red << " No hotel has been added yet." << original << endl;
		pauseEnter();
		return "";
	}

	showPage("Which state?");
	for (size_t i = 0; i < states.size(); i++) {
		boxRow(optionText(static_cast<int>(i + 1)) + states[i]);
	}
	boxRow(optionText(0) + "Cancel");
	boxLine();
	cout << " Please choose 0-" << states.size() << ": ";
	int choice = getIntInRange(0, static_cast<int>(states.size()));
	if (choice == 0) {
		return "";
	}
	return states[static_cast<size_t>(choice - 1)];
}

string chooseArea(const string& state) {
	vector<string> areas = areasOf(state);
	if (areas.empty()) {
		cout << red << " No area list for this state." << original << endl;
		pauseEnter();
		return "";
	}

	showPage("Which area in " + state + "?");
	for (size_t i = 0; i < areas.size(); i++) {
		boxRow(optionText(static_cast<int>(i + 1)) + areas[i]);
	}
	boxRow(optionText(0) + "Cancel");
	boxLine();
	cout << " Please choose 0-" << areas.size() << ": ";
	int choice = getIntInRange(0, static_cast<int>(areas.size()));
	if (choice == 0) {
		return "";
	}
	return areas[static_cast<size_t>(choice - 1)];
}

bool chooseHotel(const string& state, const string& area) {
	vector<int> list = hotelsOf(state, area);
	string place = area + ", " + state;

	showPage("Hotels in " + place);
	boxRow("Each hotel shows its address.");
	boxRow("Enter 0 to cancel");
	boxLine();

	if (list.empty()) {
		boxRow("No hotel listed for this area yet.");
		boxLine();
		pauseEnter();
		return false;
	}

	for (size_t i = 0; i < list.size(); i++) {
		int idx = list[i];
		string number = optionText(static_cast<int>(i + 1));
		boxRow(number + hotelBranches[static_cast<size_t>(idx)].name);
		boxWrapHang(string(4, ' ') + hotelBranches[static_cast<size_t>(idx)].address, 4);
		if (i + 1 < list.size()) {
			boxRow("");
		}
	}
	boxLine();
	cout << " Please choose 0-" << list.size() << ": ";
	int pick = getIntInRange(0, static_cast<int>(list.size()));
	if (pick == 0) {
		cout << " Destination cancelled." << endl;
		pauseEnter();
		return false;
	}
	return applyHotel(list[static_cast<size_t>(pick - 1)]);
}

bool applyHotel(int hotelIndex) {
	if (hotelIndex < 0 || hotelIndex >= static_cast<int>(hotelBranches.size())) {
		return false;
	}

	string oldHotel = currentHotelName;
	currentHotelName = hotelBranches[static_cast<size_t>(hotelIndex)].name;
	currentHotelAddress = hotelBranches[static_cast<size_t>(hotelIndex)].address;
	currentHotelArea = hotelBranches[static_cast<size_t>(hotelIndex)].area;
	currentHotelState = hotelBranches[static_cast<size_t>(hotelIndex)].state;

	if (oldHotel != currentHotelName && !currentUserHasUnpaid()) {
		resetOccupiedRooms();
	}

	showPage("Hotel selected");
	boxField("Hotel   : ", currentHotelName);
	boxField("Area    : ", currentHotelArea + ", " + currentHotelState);
	boxField("Address : ", currentHotelAddress);
	boxLine();
	cout << " You can now view rooms and book at this hotel." << endl;
	loadingPause();
	return true;
}

vector<string> areasOf(const string& state) {
	vector<string> areas;
	for (size_t i = 0; i < hotelBranches.size(); i++) {
		if (hotelBranches[i].state != state) {
			continue;
		}
		bool seen = false;
		for (size_t j = 0; j < areas.size(); j++) {
			if (areas[j] == hotelBranches[i].area) {
				seen = true;
				break;
			}
		}
		if (!seen) {
			areas.push_back(hotelBranches[i].area);
		}
	}
	return areas;
}

vector<int> hotelsOf(const string& state, const string& area) {
	vector<int> list;
	for (size_t i = 0; i < hotelBranches.size(); i++) {
		if (hotelBranches[i].state == state && hotelBranches[i].area == area) {
			list.push_back(static_cast<int>(i));
		}
	}
	return list;
}