#include "../hotel.h"

void printRoomColumns() {
	cout << " |"
		 << left << setfill(' ')
		 << setw(12) << " Room No"
		 << setw(13) << "Type"
		 << setw(10) << "Capacity"
		 << setw(14) << "Price/Night"
		 << setw(15) << "Status"
		 << "|" << endl;
}

void printRoomDataRow(const string& number, const string& type, int capacity,
	double price, const string& status) {
	ostringstream priceText;
	priceText << fixed << setprecision(2) << "RM" << price;
	cout << " |"
		 << left << setfill(' ')
		 << setw(12) << (" " + number)
		 << setw(15) << type
		 << setw(10) << setw(2) << " " << capacity << setw(5) << " "
		 << setw(14) << priceText.str()
		 << setw(15) << status
		 << "|" << endl;
}

void displayAvailableRoom() {
	displayRoomsByType("ALL");
}

void displayRoomsByType(const string& typeFilter) {
	int shown = 0;
	showPage("Available Rooms");
	if (!currentHotelName.empty()) {
		boxWrap(currentHotelAddress);
		boxWrap(currentHotelArea + ", " + currentHotelState);
	}
	boxLine();
	printRoomColumns();
	boxLine();

	for (size_t i = 0; i < roomList.size(); i++) {
		if (typeFilter != "ALL" && roomList[i].roomType != typeFilter) {
			continue;
		}
		printRoomDataRow(roomList[i].roomNumber, roomList[i].roomType,
			roomList[i].capacity, roomList[i].price, roomList[i].status);
		shown++;
	}

	if (shown == 0) {
		boxRow("No rooms found for this type.");
	}
	boxLine();
	pauseEnter();
}

int displayBookableRooms(int guests) {
	int shown = 0;
	showPage("Rooms that fit " + to_string(guests) + " guest(s)");
	if (!currentHotelName.empty()) {
		boxWrap(currentHotelName);
		boxWrapHang(currentHotelAddress, 0);
		boxWrap(currentHotelArea + ", " + currentHotelState);
	}
	boxRow("Any Available room with enough space can be booked.");
	boxRow("Occupied, cleaning and maintenance rooms stay hidden.");
	boxRow("");
	printRoomColumns();
	boxLine();

	for (size_t i = 0; i < roomList.size(); i++) {
		if (roomList[i].status != "Available") {
			continue;
		}
		if (roomList[i].capacity < guests) {
			continue;
		}
		printRoomDataRow(roomList[i].roomNumber, roomList[i].roomType,
			roomList[i].capacity, roomList[i].price, roomList[i].status);
		shown++;
	}

	if (shown == 0) {
		boxRow("No available room fits this number of guests.");
	}
	boxLine();
	return shown;
}

int displayRoomsForModify(int guests, const string& currentRoomNumber) {
	int shown = 0;
	showPage("Rooms that fit " + to_string(guests) + " guest(s)");
	boxRow("Your current room is listed even if occupied.");
	boxRow("");
	printRoomColumns();
	boxLine();

	for (size_t i = 0; i < roomList.size(); i++) {
		bool isCurrent = (roomList[i].roomNumber == currentRoomNumber);
		if (!isCurrent && roomList[i].status != "Available") {
			continue;
		}
		if (roomList[i].capacity < guests) {
			continue;
		}

		string statusText = isCurrent ? "Current" : roomList[i].status;
		printRoomDataRow(roomList[i].roomNumber, roomList[i].roomType,
			roomList[i].capacity, roomList[i].price, statusText);
		shown++;
	}

	if (shown == 0) {
		boxRow("No rooms that fit this guest count.");
	}
	boxLine();
	return shown;
}