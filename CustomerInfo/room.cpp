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

	cout << "\n Enter a room number to view its booking calendar, or press Enter to go back: ";
	string roomNumber;
	getline(cin, roomNumber);
	cout << endl;
	if (!roomNumber.empty() && roomNumber != "0" && findRoomIndex(roomNumber) != -1) {
		showRoomBookingCalendar(roomNumber);
		pauseEnter();
	}
	else {
		pauseEnter();
	}
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

int displayBookableRoomsForDates(int guests, int inD, int inM, int inY, int outD, int outM, int outY) {
	int shown = 0;
	string checkIn = makeDate(inD, inM, inY);
	string checkOut = makeDate(outD, outM, outY);

	showPage("Rooms free for your dates");
	if (!currentHotelName.empty()) {
		boxWrap(currentHotelName);
		boxWrapHang(currentHotelAddress, 0);
		boxWrap(currentHotelArea + ", " + currentHotelState);
	}
	boxRow("Check-in  : " + weekdayName(inD, inM, inY) + " " + checkIn + "  (any time)");
	boxRow("Check-out : " + weekdayName(outD, outM, outY) + " " + checkOut + "  before 12:00");
	boxRow("Only rooms with no booking clash are listed.");
	boxRow("");
	printRoomColumns();
	boxLine();

	for (size_t i = 0; i < roomList.size(); i++) {
		if (roomList[i].capacity < guests) {
			continue;
		}
		if (roomList[i].status == "Cleaning" || roomList[i].status == "Maintenance") {
			continue;
		}
		if (!isRoomAvailableForDates(roomList[i].roomNumber, inD, inM, inY, outD, outM, outY)) {
			continue;
		}

		printRoomDataRow(roomList[i].roomNumber, roomList[i].roomType,
			roomList[i].capacity, roomList[i].price, "Available");
		shown++;
	}

	if (shown == 0) {
		boxRow("No room is free for these dates and guest count.");
		boxRow("Try different dates or fewer guests.");
	}
	boxLine();
	return shown;
}

void showRoomBookingCalendar(const string& roomNumber) {
	int nowY = 0;
	int nowM = 0;
	int nowD = 0;
	int nowH = 0;
	int nowMin = 0;
	malaysiaNow(nowY, nowM, nowD, nowH, nowMin);
	string today = makeDate(nowD, nowM, nowY);

	showPage("Room " + roomNumber + " booking calendar");
	boxRow("Today Malaysia time: " + today + "  " + makeClockTime(nowH, nowMin));
	boxRow("Booked dates block new bookings for that period.");
	boxRow("After check-out day the room can be booked again.");
	boxLine();

	int count = 0;
	for (size_t r = 0; r < reservations.size(); r++) {
		if (reservations[r].status == "Cancelled") {
			continue;
		}
		if (reservations[r].roomNumber != roomNumber) {
			continue;
		}

		int inD = 0;
		int inM = 0;
		int inY = 0;
		int outD = 0;
		int outM = 0;
		int outY = 0;
		if (!parseDate(reservations[r].checkInDate, inD, inM, inY)) {
			continue;
		}
		if (!parseDate(reservations[r].checkOutDate, outD, outM, outY)) {
			continue;
		}

		string label;
		if (dateCompare(outD, outM, outY, nowD, nowM, nowY) <= 0) {
			label = "Past";
		}
		else if (isStayActiveOnDate(inD, inM, inY, outD, outM, outY, nowD, nowM, nowY)) {
			label = "Ongoing";
		}
		else {
			label = "Upcoming";
		}

		ostringstream line;
		line << label << "  #" << reservations[r].reservationID
			 << "  " << weekdayName(inD, inM, inY) << " " << reservations[r].checkInDate
			 << "  to  " << weekdayName(outD, outM, outY) << " " << reservations[r].checkOutDate;
		boxRow(line.str());
		count++;
	}

	if (count == 0) {
		boxRow("No bookings yet. This room is open for your dates.");
	}
	else {
		boxRow("");
		boxRow("Dates outside the booked ranges above are available to book.");
	}
	boxLine();
}

int displayRoomsForModify(int guests, const string& currentRoomNumber,
	int inD, int inM, int inY, int outD, int outM, int outY, const string& skipReservationID) {
	int shown = 0;
	showPage("Rooms that fit " + to_string(guests) + " guest(s)");
	boxRow("Your current room is listed even if booked on these dates.");
	boxRow("Check-in  : " + makeDate(inD, inM, inY));
	boxRow("Check-out : " + makeDate(outD, outM, outY));
	boxRow("");
	printRoomColumns();
	boxLine();

	for (size_t i = 0; i < roomList.size(); i++) {
		bool isCurrent = (roomList[i].roomNumber == currentRoomNumber);
		if (!isCurrent) {
			if (roomList[i].status == "Cleaning" || roomList[i].status == "Maintenance") {
				continue;
			}
			if (!isRoomAvailableForDates(roomList[i].roomNumber, inD, inM, inY, outD, outM, outY, skipReservationID)) {
				continue;
			}
		}
		if (roomList[i].capacity < guests) {
			continue;
		}

		string statusText = isCurrent ? "Current" : "Available";
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