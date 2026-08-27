#include "../staff_internal.h"

void staffRoomMenu() {
	int choice;
	do {
		showPage("Manage rooms");
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
	showPage("All rooms");
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
	loadingPause();
}

void staffAddRoom() {
	Room room;
	if (!askLine(" Room number (or 0 to cancel): ", room.roomNumber)) {
		return;
	}
	if (findRoomIndex(room.roomNumber) != -1) {
		cout << red << " That room number already exists." << original << endl;
		pauseEnter();
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
		pauseEnter();
		return;
	}

	if (!askMoney(" Price per night RM (or 0 to cancel): ", room.price)) {
		return;
	}

	showPage("Starting status");
	boxRow(optionText(1) + "Available");
	boxRow(optionText(2) + "Cleaning");
	boxRow(optionText(3) + "Maintenance");
	boxRow(optionText(0) + "Cancel");
	boxLine();
	cout << " Please choose 0-3: ";
	int statusPick = getIntInRange(0, 3);
	if (statusPick == 0) {
		cout << " Add room cancelled." << endl;
		pauseEnter();
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
	pauseEnter();
}

void staffEditRoom() {
	staffPrintRooms();
	string roomNumber;
	if (!askLine(" Room number to edit (or 0 to cancel): ", roomNumber)) {
		return;
	}
	int idx = findRoomIndex(roomNumber);
	if (idx == -1) {
		cout << red << " Room not found." << original << endl;
		pauseEnter();
		return;
	}

	showPage("Edit room " + roomList[idx].roomNumber);
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
			pauseEnter();
		}
	}
	else if (choice == 2) {
		cout << " New capacity 1-6 (or 0 to cancel): ";
		int capacity = getIntInRange(0, 6);
		if (capacity == 0) {
			cout << " Capacity not changed." << endl;
			pauseEnter();
			return;
		}
		roomList[idx].capacity = capacity;
		cout << " Capacity updated." << endl;
		pauseEnter();
	}
	else if (choice == 3) {
		double price = 0;
		if (askMoney(" New price per night RM (or 0 to cancel): ", price)) {
			roomList[idx].price = price;
			cout << " Price updated." << endl;
			pauseEnter();
		}
	}
	else if (choice == 4) {
		string status = pickRoomStatus();
		if (status.empty()) {
			return;
		}
		if (status == "Available" && roomHasActiveBooking(roomList[idx].roomNumber)) {
			cout << red << " This room still has an active booking. Status not changed." << original << endl;
			pauseEnter();
			return;
		}
		roomList[idx].status = status;
		cout << " Status updated." << endl;
		pauseEnter();
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
		cout << red << " Room not found." << original << endl;
		pauseEnter();
		return;
	}
	if (roomHasActiveBooking(roomNumber)) {
		cout << red << " Cannot delete. This room still has an active booking." << original << endl;
		pauseEnter();
		return;
	}
	if (!confirmYesNo(" Delete room " + roomNumber + "? y/n: ")) {
		cout << " Room kept." << endl;
		pauseEnter();
		return;
	}
	roomList.erase(roomList.begin() + static_cast<size_t>(idx));
	cout << " Room " << roomNumber << " deleted." << endl;
	pauseEnter();
}
