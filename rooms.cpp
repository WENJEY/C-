#include "hotel.h"

void displayAvailableRoom() {
	displayRoomsByType("ALL");
}

void displayRoomsByType(const string& typeFilter) {
	int shown = 0;
	cout << endl;
	boxLine();
	cout << " |"
		 << left << setfill(' ')
		 << setw(9) << " Room No"
		 << setw(13) << "Type"
		 << setw(10) << "Capacity"
		 << setw(16) << "Price/Night"
		 << setw(16) << "Status"
		 << "|" << endl;
	boxLine();

	for (size_t i = 0; i < roomList.size(); i++) {
		if (typeFilter != "ALL" && roomList[i].roomType != typeFilter) {
			continue;
		}
		ostringstream price;
		price << fixed << setprecision(2) << "RM" << roomList[i].price;
		cout << " |"
			 << left << setfill(' ')
			 << setw(9) << (" " + roomList[i].roomNumber)
			 << setw(13) << roomList[i].roomType
			 << setw(10) << roomList[i].capacity
			 << setw(16) << price.str()
			 << setw(16) << roomList[i].status
			 << "|" << endl;
		shown++;
	}

	if (shown == 0) {
		boxRow("No rooms found for this type.");
	}
	boxLine();
}

int displayBookableRooms(int guests, bool largerRooms) {
	int shown = 0;
	cout << endl;
	if (largerRooms) {
		boxTitle("Larger rooms (more than " + to_string(guests) + " guest(s))");
	}
	else {
		boxTitle("Rooms for " + to_string(guests) + " guest(s)");
	}
	cout << " |"
		 << left << setfill(' ')
		 << setw(9) << " Room No"
		 << setw(13) << "Type"
		 << setw(10) << "Capacity"
		 << setw(16) << "Price/Night"
		 << setw(16) << "Status"
		 << "|" << endl;
	boxLine();

	for (size_t i = 0; i < roomList.size(); i++) {
		if (roomList[i].status != "Available") {
			continue;
		}
		if (largerRooms) {
			if (roomList[i].capacity <= guests) {
				continue;
			}
		}
		else if (roomList[i].capacity != guests) {
			continue;
		}

		ostringstream price;
		price << fixed << setprecision(2) << "RM" << roomList[i].price;
		cout << " |"
			 << left << setfill(' ')
			 << setw(9) << (" " + roomList[i].roomNumber)
			 << setw(13) << roomList[i].roomType
			 << setw(10) << roomList[i].capacity
			 << setw(16) << price.str()
			 << setw(16) << roomList[i].status
			 << "|" << endl;
		shown++;
	}

	if (shown == 0) {
		if (largerRooms) {
			boxRow("No larger rooms are available to book.");
		}
		else {
			boxRow("No rooms of this size are available to book.");
		}
	}
	boxLine();
	return shown;
}
