#include "hotel.h"

const int STATE_COUNT = 14;
const string STATE_ORDER[STATE_COUNT] = {
	"Perlis", "Kedah", "Pulau Pinang", "Perak", "Selangor", "Kuala Lumpur",
	"Negeri Sembilan", "Melaka", "Johor", "Pahang", "Terengganu", "Kelantan",
	"Sabah", "Sarawak"
};

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

		cout << endl;
		boxTitle("Unpaid booking");
		boxWrap("Hotel   : " + currentHotelName);
		boxWrap("Area    : " + currentHotelArea + ", " + currentHotelState);
		boxWrap("Address : " + currentHotelAddress);
		boxRow("Finish payment or cancel it before choosing another hotel.");
		boxLine();
		return true;
	}
	return false;
}

bool unpaidBlocksHotelChange() {
	if (currentUserHasUnpaid() && !currentHotelName.empty()) {
		cout << "\n You still have an unpaid booking at " << currentHotelName << "." << endl;
		cout << " Finish payment or cancel it before choosing another hotel." << endl;
		return true;
	}
	return false;
}

bool findHotelByDestination() {
	if (unpaidBlocksHotelChange()) {
		return false;
	}

	cout << endl;
	boxTitle("Choose a destination");
	boxRow("Pick a state, then an area, then a hotel.");
	boxRow("The hotel address is shown before you book.");
	boxLine();

	string state = chooseState();
	if (state.empty()) {
		cout << " Destination cancelled." << endl;
		return false;
	}

	string area = chooseArea(state);
	if (area.empty()) {
		cout << " Destination cancelled." << endl;
		return false;
	}

	return chooseHotel(state, area);
}

bool requireHotelSelected() {
	if (!currentHotelName.empty()) {
		return true;
	}
	cout << "\n Please choose a state, area and hotel first." << endl;
	return findHotelByDestination();
}

void changeDestination() {
	if (currentHotelName.empty()) {
		findHotelByDestination();
		return;
	}
	if (unpaidBlocksHotelChange()) {
		return;
	}

	cout << endl;
	boxTitle("Change destination");
	boxWrap("Now     : " + currentHotelName);
	boxWrap("Area    : " + currentHotelArea + ", " + currentHotelState);
	boxLine();
	boxRow("1. Change state");
	boxRow("2. Change area");
	boxRow("3. Change hotel");
	boxRow("0. Keep this hotel");
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
			return;
		}
		chooseHotel(currentHotelState, area);
		return;
	}

	chooseHotel(currentHotelState, currentHotelArea);
}

string chooseState() {
	cout << endl;
	boxTitle("Which state?");
	for (int i = 0; i < STATE_COUNT; i++) {
		boxRow(to_string(i + 1) + ". " + STATE_ORDER[i]);
	}
	boxRow("0. Cancel");
	boxLine();
	cout << " Please choose 0-" << STATE_COUNT << ": ";
	int choice = getIntInRange(0, STATE_COUNT);
	if (choice == 0) {
		return "";
	}
	return STATE_ORDER[choice - 1];
}

string chooseArea(const string& state) {
	vector<string> areas = areasOf(state);
	if (areas.empty()) {
		cout << " No area list for this state." << endl;
		return "";
	}

	cout << endl;
	boxTitle("Which area in " + state + "?");
	for (size_t i = 0; i < areas.size(); i++) {
		boxRow(to_string(i + 1) + ". " + areas[i]);
	}
	boxRow("0. Cancel");
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

	cout << endl;
	boxTitle("Hotels in " + place);
	boxRow("Each hotel shows its address.");
	boxRow("Enter 0 to cancel");
	boxLine();

	if (list.empty()) {
		boxRow("No hotel listed for this area yet.");
		boxLine();
		return false;
	}

	for (size_t i = 0; i < list.size(); i++) {
		int idx = list[i];
		boxWrap(to_string(i + 1) + ". " + hotelBranches[static_cast<size_t>(idx)].name);
		boxWrap("   " + hotelBranches[static_cast<size_t>(idx)].address);
		if (i + 1 < list.size()) {
			boxLine();
		}
	}
	boxLine();
	cout << " Please choose 0-" << list.size() << ": ";
	int pick = getIntInRange(0, static_cast<int>(list.size()));
	if (pick == 0) {
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

	cout << endl;
	boxTitle("Hotel selected");
	boxWrap("Hotel   : " + currentHotelName);
	boxWrap("Area    : " + currentHotelArea + ", " + currentHotelState);
	boxWrap("Address : " + currentHotelAddress);
	boxLine();
	cout << " You can now view rooms and book at this hotel." << endl;
	pauseEnter();
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
