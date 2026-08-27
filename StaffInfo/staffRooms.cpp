#include "../staff_internal.h"

void staffRoomMenu() {
	int choice;
	do {
		cout << endl;
		boxTitle("Manage rooms");
		boxRow(optionText(1) + "View rooms");
		boxRow(optionText(2) + "Add a room");
		boxRow(optionText(3) + "Edit a room");
		boxRow(optionText(4) + "Delete a room");
		boxRow(optionText(0) + "Back");
		boxLine();
		cout << " Please choose 0-4: ";
		choice = getIntInRange(0, 4);
		if (choice == 1) {
			staffListRooms();
		}
		else if (choice == 2) {
			staffAddRoom();
		}
		else if (choice == 3) {
			staffEditRoom();
		}
		else if (choice == 4) {
			staffDeleteRoom();
		}
	} while (choice != 0);
}

void staffPrintRooms() {
	int available = 0;
	int occupied = 0;
	int cleaning = 0;
	int maintenance = 0;
	cout << endl;
	boxTitle("All rooms");
	printRoomColumns();
	boxLine();
	for (size_t i = 0; i < roomList.size(); i++) {
		printRoomDataRow(roomList[i].roomNumber, roomList[i].roomType,
			roomList[i].capacity, roomList[i].price, roomList[i].status);
		if (roomList[i].status == "Available") {
			available++;
		}
		else if (roomList[i].status == "Occupied") {
			occupied++;
		}
		else if (roomList[i].status == "Cleaning") {
			cleaning++;
		}
		else if (roomList[i].status == "Maintenance") {
			maintenance++;
		}
	}
	if (roomList.empty()) {
		boxRow("No rooms yet. Use Add a room.");
	}
	boxLine();
	{
		ostringstream line;
		line << "Available " << available
			 << "  Occupied " << occupied
			 << "  Cleaning " << cleaning
			 << "  Maintenance " << maintenance;
		boxRow(line.str());
	}
	boxLine();
}

void staffListRooms() {
	staffPrintRooms();
	pauseEnter();
}

void staffAddRoom() {
	Room room;
	if (!askLine(" Room number (or 0 to cancel): ", room.roomNumber)) {
		return;
	}
	if (findRoomIndex(room.roomNumber) != -1) {
		cout << " That room number already exists." << endl;
		return;
	}

	room.roomType = pickRoomType();
	if (room.roomType.empty()) {
		return;
	}

	cout << " Capacity 1-6 (or 0 to cancel): ";
	room.capacity = getIntInRange(0, 6);
	if (room.capacity == 0) {
		cout << " Add room cancelled." << endl;
		return;
	}

	if (!askMoney(" Price per night RM (or 0 to cancel): ", room.price)) {
		return;
	}

	cout << endl;
	boxTitle("Starting status");
	boxRow(optionText(1) + "Available");
	boxRow(optionText(2) + "Cleaning");
	boxRow(optionText(3) + "Maintenance");
	boxRow(optionText(0) + "Cancel");
	boxLine();
	cout << " Please choose 0-3: ";
	int statusPick = getIntInRange(0, 3);
	if (statusPick == 0) {
		cout << " Add room cancelled." << endl;
		return;
	}
	if (statusPick == 1) {
		room.status = "Available";
	}
	else if (statusPick == 2) {
		room.status = "Cleaning";
	}
	else {
		room.status = "Maintenance";
	}

	roomList.push_back(room);
	cout << " Room " << room.roomNumber << " added." << endl;
}

void staffEditRoom() {
	staffPrintRooms();
	string roomNumber;
	if (!askLine(" Room number to edit (or 0 to cancel): ", roomNumber)) {
		return;
	}
	int idx = findRoomIndex(roomNumber);
	if (idx == -1) {
		cout << " Room not found." << endl;
		return;
	}

	cout << endl;
	boxTitle("Edit room " + roomList[idx].roomNumber);
	boxField("Type     : ", roomList[idx].roomType);
	{
		ostringstream line;
		line << roomList[idx].capacity;
		boxField("Capacity : ", line.str());
	}
	{
		ostringstream line;
		line << fixed << setprecision(2) << "RM " << roomList[idx].price;
		boxField("Price    : ", line.str());
	}
	boxField("Status   : ", roomList[idx].status);
	boxLine();
	boxRow(optionText(1) + "Change type");
	boxRow(optionText(2) + "Change capacity");
	boxRow(optionText(3) + "Change price");
	boxRow(optionText(4) + "Change status");
	boxRow(optionText(0) + "Back");
	boxLine();
	cout << " Please choose 0-4: ";
	int choice = getIntInRange(0, 4);
	if (choice == 0) {
		return;
	}
	if (choice == 1) {
		string type = pickRoomType();
		if (!type.empty()) {
			roomList[idx].roomType = type;
			cout << " Type updated." << endl;
		}
	}
	else if (choice == 2) {
		cout << " New capacity 1-6 (or 0 to cancel): ";
		int capacity = getIntInRange(0, 6);
		if (capacity == 0) {
			cout << " Capacity not changed." << endl;
			return;
		}
		roomList[idx].capacity = capacity;
		cout << " Capacity updated." << endl;
	}
	else if (choice == 3) {
		double price = 0;
		if (askMoney(" New price per night RM (or 0 to cancel): ", price)) {
			roomList[idx].price = price;
			cout << " Price updated." << endl;
		}
	}
	else if (choice == 4) {
		string status = pickRoomStatus();
		if (status.empty()) {
			return;
		}
		if (status == "Available" && roomHasActiveBooking(roomList[idx].roomNumber)) {
			cout << " This room still has an active booking. Status not changed." << endl;
			return;
		}
		roomList[idx].status = status;
		cout << " Status updated." << endl;
	}
}

void staffDeleteRoom() {
	staffPrintRooms();
	string roomNumber;
	if (!askLine(" Room number to delete (or 0 to cancel): ", roomNumber)) {
		return;
	}
	int idx = findRoomIndex(roomNumber);
	if (idx == -1) {
		cout << " Room not found." << endl;
		return;
	}
	if (roomHasActiveBooking(roomNumber)) {
		cout << " Cannot delete. This room still has an active booking." << endl;
		return;
	}
	if (!confirmYesNo(" Delete room " + roomNumber + "? y/n: ")) {
		cout << " Room kept." << endl;
		return;
	}
	roomList.erase(roomList.begin() + static_cast<size_t>(idx));
	cout << " Room " << roomNumber << " deleted." << endl;
}
