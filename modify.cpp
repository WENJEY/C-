#include "hotel.h"

void showModifySummary(int resIndex);
int pickBookingToModify();
void modifyBookingMenu(int resIndex);
bool stayAlreadyStarted(int resIndex);
void refreshCheckoutDate(int resIndex);
void saveStayChanges();
void remindPaidChange(int resIndex);
void changeCheckInDate(int resIndex);
void changeNights(int resIndex);
void changeGuests(int resIndex);
void changeRoom(int resIndex);
void changeSpecialRequest(int resIndex);

void modifyReservations() {
	while (true) {
		int resIndex = pickBookingToModify();
		if (resIndex == -1) {
			return;
		}
		modifyBookingMenu(resIndex);
	}
}

int pickBookingToModify() {
	vector<int> choices;

	cout << endl;
	boxTitle("Modify Reservations");
	boxRow("Choose a booking to change");
	boxRow("Cancelled bookings cannot be modified");
	boxRow("Enter 0 to go back");
	boxLine();

	for (size_t i = 0; i < reservations.size(); i++) {
		if (reservations[i].customerUsername != currentLoggedInCustomer) {
			continue;
		}
		if (reservations[i].status == "Cancelled") {
			continue;
		}
		choices.push_back(static_cast<int>(i));
		ostringstream line;
		line << choices.size() << ". #" << reservations[i].reservationID
			 << "  Room " << reservations[i].roomNumber
			 << "  " << reservations[i].roomType;
		boxRow(line.str());
		boxRow("   Check-in " + reservations[i].checkInDate
			 + "   " + reservations[i].paymentStatus);
	}

	if (choices.empty()) {
		boxRow("No active booking to modify.");
		boxLine();
		return -1;
	}

	boxLine();
	cout << " Please choose 0-" << choices.size() << ": ";
	int pick = getIntInRange(0, static_cast<int>(choices.size()));
	if (pick == 0) {
		return -1;
	}
	return choices[pick - 1];
}

void showModifySummary(int resIndex) {
	const BookingRecord& b = reservations[resIndex];
	cout << endl;
	boxTitle("Modify #" + b.reservationID);
	boxRow("Room         : " + b.roomNumber + "  " + b.roomType);
	{
		ostringstream line;
		line << "Guests       : " << b.guests;
		boxRow(line.str());
	}
	{
		ostringstream line;
		line << "Nights       : " << b.nights << " night(s)";
		boxRow(line.str());
	}
	boxRow("Check-in     : " + b.checkInDate + "  (any time)");
	boxRow("Check-out    : " + b.checkOutDate + "  before 12:00");
	boxRow("Payment      : " + b.paymentStatus);
	if (b.paymentStatus == "Paid") {
		ostringstream line;
		line << fixed << setprecision(2) << "Paid amount  : RM " << b.totalAmount;
		boxRow(line.str());
	}
	{
		ostringstream line;
		line << fixed << setprecision(2) << "Room charge  : RM "
			 << roundMoney(b.pricePerNight * b.nights);
		boxRow(line.str());
	}
	if (b.specialRequest != "-" && !b.specialRequest.empty()) {
		boxRow("Request      : " + b.specialRequest);
	}
}

void modifyBookingMenu(int resIndex) {
	int choice;

	do {
		showModifySummary(resIndex);
		boxLine();

		bool started = stayAlreadyStarted(resIndex);
		if (started) {
			boxRow("Stay already started. Only special request can change.");
			boxRow("1. Change special request");
			boxRow("0. Back");
			boxLine();
			cout << " Please choose 0-1: ";
			choice = getIntInRange(0, 1);
			if (choice == 1) {
				changeSpecialRequest(resIndex);
			}
		}
		else {
			boxRow("1. Change check-in date");
			boxRow("2. Change number of nights");
			boxRow("3. Change number of guests");
			boxRow("4. Change room");
			boxRow("5. Change special request");
			boxRow("0. Back");
			boxLine();
			cout << " Please choose 0-5: ";
			choice = getIntInRange(0, 5);

			switch (choice) {
			case 1:
				changeCheckInDate(resIndex);
				break;
			case 2:
				changeNights(resIndex);
				break;
			case 3:
				changeGuests(resIndex);
				break;
			case 4:
				changeRoom(resIndex);
				break;
			case 5:
				changeSpecialRequest(resIndex);
				break;
			}
		}
	} while (choice != 0);
}

bool stayAlreadyStarted(int resIndex) {
	int d = 0;
	int m = 0;
	int y = 0;
	if (!parseDate(reservations[resIndex].checkInDate, d, m, y)) {
		return false;
	}
	return daysFromToday(d, m, y) < 0;
}

void refreshCheckoutDate(int resIndex) {
	int d = 0;
	int m = 0;
	int y = 0;
	if (!parseDate(reservations[resIndex].checkInDate, d, m, y)) {
		return;
	}
	addDays(d, m, y, reservations[resIndex].nights);
	reservations[resIndex].checkOutDate = makeDate(d, m, y);
}

void saveStayChanges() {
	// Bookings and room status stay in memory only.
	// Closing the program restores the original room list in data.cpp.
}

void remindPaidChange(int resIndex) {
	if (reservations[resIndex].paymentStatus != "Paid") {
		cout << " Unpaid booking updated. Pay from Menu Page option 2." << endl;
		return;
	}
	cout << fixed << setprecision(2);
	cout << " This stay is already paid (RM "
		 << reservations[resIndex].totalAmount << ")." << endl;
	cout << " New room charge: RM "
		 << roundMoney(reservations[resIndex].pricePerNight * reservations[resIndex].nights)
		 << endl;
	cout << " If the new stay costs more, pay the difference at the hotel counter." << endl;
	cout << " If it costs less, a refund will be arranged at check-in." << endl;
}

void changeCheckInDate(int resIndex) {
	int d = 0;
	int m = 0;
	int y = 0;
	if (!askCheckInDate(d, m, y)) {
		cout << " Check-in date not changed." << endl;
		return;
	}

	reservations[resIndex].checkInDate = makeDate(d, m, y);
	reservations[resIndex].checkInTime = "Any time";
	refreshCheckoutDate(resIndex);
	saveStayChanges();
	cout << " Check-in updated to " << weekdayName(d, m, y)
		 << " " << reservations[resIndex].checkInDate << "." << endl;
	cout << " Check-out is now " << reservations[resIndex].checkOutDate
		 << " before 12:00 noon." << endl;
	remindPaidChange(resIndex);
}

void changeNights(int resIndex) {
	cout << "\n Current nights: " << reservations[resIndex].nights << endl;
	cout << " New number of nights (1-30 or 0 to cancel): ";
	int nights = getIntInRange(0, 30);
	if (nights == 0) {
		cout << " Nights not changed." << endl;
		return;
	}

	reservations[resIndex].nights = nights;
	refreshCheckoutDate(resIndex);
	saveStayChanges();
	cout << " Stay is now " << nights << " night(s)." << endl;
	cout << " Check-out is now " << reservations[resIndex].checkOutDate
		 << " before 12:00 noon." << endl;
	remindPaidChange(resIndex);
}

void changeGuests(int resIndex) {
	int roomIndex = findRoomIndex(reservations[resIndex].roomNumber);
	int capacity = 1;
	if (roomIndex != -1) {
		capacity = roomList[roomIndex].capacity;
	}

	cout << "\n Current guests: " << reservations[resIndex].guests << endl;
	cout << " This room fits up to " << capacity << " guest(s)." << endl;
	cout << " New guest count (1-" << capacity << " or 0 to cancel): ";
	int guests = getIntInRange(0, 6);
	if (guests == 0) {
		cout << " Guest count not changed." << endl;
		return;
	}
	if (guests > capacity) {
		cout << " This room only fits " << capacity
			 << " guest(s). Change room first, then guest count." << endl;
		return;
	}

	reservations[resIndex].guests = guests;
	saveStayChanges();
	cout << " Guest count updated to " << guests << "." << endl;
}

void changeRoom(int resIndex) {
	int guests = reservations[resIndex].guests;
	string oldRoom = reservations[resIndex].roomNumber;
	int shown = displayRoomsForModify(guests, oldRoom);
	if (shown == 0) {
		cout << " No other room fits this guest count." << endl;
		return;
	}

	string roomNumber;
	int roomIndex = -1;
	while (true) {
		cout << "\n Enter new room number or 0 to cancel: ";
		getline(cin, roomNumber);
		if (roomNumber == "0") {
			cout << " Room not changed." << endl;
			return;
		}

		roomIndex = findRoomIndex(roomNumber);
		if (roomIndex == -1) {
			cout << " Room not found! Please try again." << endl;
			continue;
		}
		if (roomList[roomIndex].roomNumber != oldRoom
			&& roomList[roomIndex].status != "Available") {
			cout << " That room is not available. Status: "
				 << roomList[roomIndex].status << endl;
			continue;
		}
		if (roomList[roomIndex].capacity < guests) {
			cout << " That room only fits " << roomList[roomIndex].capacity
				 << " guest(s)." << endl;
			continue;
		}
		break;
	}

	if (roomList[roomIndex].roomNumber == oldRoom) {
		cout << " You are already in Room " << oldRoom << "." << endl;
		return;
	}

	cout << fixed << setprecision(2);
	cout << "\n Current: Room " << oldRoom << "  "
		 << reservations[resIndex].roomType << "  RM "
		 << reservations[resIndex].pricePerNight << " / night" << endl;
	cout << " New:     Room " << roomList[roomIndex].roomNumber << "  "
		 << roomList[roomIndex].roomType << "  RM "
		 << roomList[roomIndex].price << " / night" << endl;

	if (!confirmYesNo(" Confirm room change? y/n: ")) {
		cout << " Room not changed." << endl;
		return;
	}

	setRoomStatus(oldRoom, "Available");
	setRoomStatus(roomList[roomIndex].roomNumber, "Occupied");
	reservations[resIndex].roomNumber = roomList[roomIndex].roomNumber;
	reservations[resIndex].roomType = roomList[roomIndex].roomType;
	reservations[resIndex].pricePerNight = roomList[roomIndex].price;
	saveStayChanges();
	cout << " Room changed to " << reservations[resIndex].roomNumber
		 << " " << reservations[resIndex].roomType << "." << endl;
	remindPaidChange(resIndex);
}

void changeSpecialRequest(int resIndex) {
	cout << endl;
	boxTitle("Special Requests");
	if (reservations[resIndex].specialRequest != "-"
		&& !reservations[resIndex].specialRequest.empty()) {
		boxRow("Current: " + reservations[resIndex].specialRequest);
	}
	boxRow("1. High floor");
	boxRow("2. Quiet room / away from lift");
	boxRow("3. Extra pillows");
	boxRow("4. Baby crib");
	boxRow("5. Connecting rooms");
	boxRow("6. Type my own request");
	boxRow("7. Clear special request");
	boxRow("0. Cancel");
	boxLine();
	cout << " Please choose 0-7: ";
	int choice = getIntInRange(0, 7);

	if (choice == 0) {
		return;
	}
	if (choice == 7) {
		reservations[resIndex].specialRequest = "-";
		saveStayChanges();
		cout << " Special request cleared." << endl;
		return;
	}

	string extra;
	switch (choice) {
	case 1:
		extra = "High floor";
		break;
	case 2:
		extra = "Quiet room";
		break;
	case 3:
		extra = "Extra pillows";
		break;
	case 4:
		extra = "Baby crib";
		break;
	case 5:
		extra = "Connecting rooms";
		break;
	case 6:
		cout << " Type your request: ";
		getline(cin, extra);
		break;
	}

	if (extra.empty()) {
		cout << " No request added." << endl;
		return;
	}

	string request = reservations[resIndex].specialRequest;
	if (request == "-" || request.empty()) {
		request = extra;
	}
	else {
		request = request + ", " + extra;
	}
	reservations[resIndex].specialRequest = request;
	saveStayChanges();
	cout << " Noted! We will try our best: " << request << endl;
}
