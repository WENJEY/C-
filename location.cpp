#include "hotel.h"

const int STATE_COUNT = 4;
const int MAX_AREAS = 4;

string stateNameOf(int stateIndex);
int areaCountOf(int stateIndex);
string areaNameOf(int stateIndex, int areaIndex);
double areaDistanceKm(int stateIndex, int fromArea, int toArea);
int chooseState();
int chooseArea(int stateIndex);
bool pickRecommendedHotel(int stateIndex, int areaIndex);

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
		boxRow("Hotel   : " + currentHotelName);
		boxRow("Area    : " + currentHotelArea + ", " + currentHotelState);
		boxRow("Address : " + currentHotelAddress);
		boxRow("Finish payment or cancel it before choosing another hotel.");
		boxLine();
		return true;
	}
	return false;
}

bool findHotelByDestination() {
	if (currentUserHasUnpaid() && !currentHotelName.empty()) {
		cout << "\n You still have an unpaid booking at " << currentHotelName << "." << endl;
		cout << " Finish payment or cancel it before choosing another hotel." << endl;
		return false;
	}

	cout << endl;
	boxTitle("Find a Hotel");
	boxRow("Tell us where you want to go.");
	boxRow("We will recommend nearby Grand Horizon hotels.");
	boxLine();

	int stateIndex = chooseState();
	if (stateIndex < 0) {
		cout << " Destination search cancelled." << endl;
		return false;
	}

	int areaIndex = chooseArea(stateIndex);
	if (areaIndex < 0) {
		cout << " Destination search cancelled." << endl;
		return false;
	}

	return pickRecommendedHotel(stateIndex, areaIndex);
}

bool requireHotelSelected() {
	if (!currentHotelName.empty()) {
		return true;
	}
	cout << "\n Please choose a destination first." << endl;
	return findHotelByDestination();
}

int chooseState() {
	cout << endl;
	boxTitle("Which state?");
	boxRow("1. Selangor");
	boxRow("2. Kuala Lumpur");
	boxRow("3. Penang");
	boxRow("4. Johor");
	boxRow("0. Cancel");
	boxLine();
	cout << " Please choose 0-4: ";
	int choice = getIntInRange(0, STATE_COUNT);
	if (choice == 0) {
		return -1;
	}
	return choice - 1;
}

int chooseArea(int stateIndex) {
	int count = areaCountOf(stateIndex);
	cout << endl;
	boxTitle("Which area in " + stateNameOf(stateIndex) + "?");
	for (int i = 0; i < count; i++) {
		boxRow(to_string(i + 1) + ". " + areaNameOf(stateIndex, i));
	}
	boxRow("0. Cancel");
	boxLine();
	cout << " Please choose 0-" << count << ": ";
	int choice = getIntInRange(0, count);
	if (choice == 0) {
		return -1;
	}
	return choice - 1;
}

bool pickRecommendedHotel(int stateIndex, int areaIndex) {
	vector<int> hotelIndex;
	vector<double> hotelKm;

	for (size_t i = 0; i < hotelBranches.size(); i++) {
		if (hotelBranches[i].stateIndex != stateIndex) {
			continue;
		}
		double km = areaDistanceKm(stateIndex, areaIndex, hotelBranches[i].areaIndex);
		if (km < 0.01) {
			km = hotelBranches[i].localKm;
		}
		hotelIndex.push_back(static_cast<int>(i));
		hotelKm.push_back(km);
	}

	for (size_t i = 0; i < hotelKm.size(); i++) {
		size_t best = i;
		for (size_t j = i + 1; j < hotelKm.size(); j++) {
			if (hotelKm[j] < hotelKm[best]) {
				best = j;
			}
		}
		if (best != i) {
			double kmTmp = hotelKm[i];
			hotelKm[i] = hotelKm[best];
			hotelKm[best] = kmTmp;
			int idxTmp = hotelIndex[i];
			hotelIndex[i] = hotelIndex[best];
			hotelIndex[best] = idxTmp;
		}
	}

	string place = areaNameOf(stateIndex, areaIndex) + ", " + stateNameOf(stateIndex);
	cout << endl;
	boxTitle("Hotels near " + place);
	boxRow("Sorted by distance from your destination.");
	boxRow("Enter 0 to cancel");
	boxLine();

	if (hotelIndex.empty()) {
		boxRow("No Grand Horizon hotel in this state yet.");
		boxLine();
		return false;
	}

	for (size_t i = 0; i < hotelIndex.size(); i++) {
		int idx = hotelIndex[i];
		ostringstream dist;
		dist << fixed << setprecision(1) << hotelKm[i] << " km away";
		boxRow(to_string(i + 1) + ". " + hotelBranches[idx].name);
		boxRow("   " + dist.str());
		boxRow("   " + hotelBranches[idx].address);
		if (i + 1 < hotelIndex.size()) {
			boxLine();
		}
	}
	boxLine();
	cout << " Please choose 0-" << hotelIndex.size() << ": ";
	int pick = getIntInRange(0, static_cast<int>(hotelIndex.size()));
	if (pick == 0) {
		return false;
	}

	int idx = hotelIndex[pick - 1];
	string oldHotel = currentHotelName;
	currentHotelName = hotelBranches[idx].name;
	currentHotelAddress = hotelBranches[idx].address;
	currentHotelArea = areaNameOf(hotelBranches[idx].stateIndex, hotelBranches[idx].areaIndex);
	currentHotelState = stateNameOf(hotelBranches[idx].stateIndex);

	if (oldHotel != currentHotelName && !currentUserHasUnpaid()) {
		resetOccupiedRooms();
	}

	cout << endl;
	boxTitle("Hotel selected");
	boxRow("Hotel   : " + currentHotelName);
	boxRow("Area    : " + currentHotelArea + ", " + currentHotelState);
	boxRow("Address : " + currentHotelAddress);
	{
		ostringstream dist;
		dist << fixed << setprecision(1) << hotelKm[pick - 1] << " km from " << place;
		boxRow("Distance: " + dist.str());
	}
	boxLine();
	cout << " You can now view rooms and book at this hotel." << endl;
	pauseEnter();
	return true;
}

string stateNameOf(int stateIndex) {
	string names[STATE_COUNT] = {
		"Selangor", "Kuala Lumpur", "Penang", "Johor"
	};
	if (stateIndex < 0 || stateIndex >= STATE_COUNT) {
		return "";
	}
	return names[stateIndex];
}

int areaCountOf(int stateIndex) {
	int counts[STATE_COUNT] = {4, 4, 3, 3};
	if (stateIndex < 0 || stateIndex >= STATE_COUNT) {
		return 0;
	}
	return counts[stateIndex];
}

string areaNameOf(int stateIndex, int areaIndex) {
	string names[STATE_COUNT][MAX_AREAS] = {
		{"Petaling Jaya", "Shah Alam", "Subang Jaya", "Kajang"},
		{"Kepong", "KLCC", "Bukit Bintang", "Cheras"},
		{"Georgetown", "Batu Ferringhi", "Bayan Lepas", ""},
		{"Johor Bahru", "Iskandar Puteri", "Skudai", ""}
	};
	if (stateIndex < 0 || stateIndex >= STATE_COUNT) {
		return "";
	}
	if (areaIndex < 0 || areaIndex >= areaCountOf(stateIndex)) {
		return "";
	}
	return names[stateIndex][areaIndex];
}

double areaDistanceKm(int stateIndex, int fromArea, int toArea) {
	int count = areaCountOf(stateIndex);
	if (fromArea < 0 || toArea < 0 || fromArea >= count || toArea >= count) {
		return 0.0;
	}
	if (fromArea == toArea) {
		return 0.0;
	}

	if (stateIndex == 0) {
		double km[4][4] = {
			{0.0, 18.0, 8.0, 27.0},
			{18.0, 0.0, 16.0, 32.0},
			{8.0, 16.0, 0.0, 22.0},
			{27.0, 32.0, 22.0, 0.0}
		};
		return km[fromArea][toArea];
	}
	if (stateIndex == 1) {
		double km[4][4] = {
			{0.0, 12.0, 14.0, 18.0},
			{12.0, 0.0, 2.5, 9.0},
			{14.0, 2.5, 0.0, 7.0},
			{18.0, 9.0, 7.0, 0.0}
		};
		return km[fromArea][toArea];
	}
	if (stateIndex == 2) {
		double km[3][3] = {
			{0.0, 16.0, 18.0},
			{16.0, 0.0, 22.0},
			{18.0, 22.0, 0.0}
		};
		return km[fromArea][toArea];
	}

	double km[3][3] = {
		{0.0, 12.0, 10.0},
		{12.0, 0.0, 14.0},
		{10.0, 14.0, 0.0}
	};
	return km[fromArea][toArea];
}
