#include "hotel.h"

void bookRoom() {
	loadUnpaidIntoSession();

	if (!currentSessionIDs.empty()) {
		cout << "\n You still have unpaid booking(s) from this stay:" << endl;
		for (size_t i = 0; i < currentSessionIDs.size(); i++) {
			int idx = findReservationIndex(currentSessionIDs[i]);
			if (idx == -1) {
				continue;
			}
			cout << " - #" << reservations[idx].reservationID
				 << "  Room " << reservations[idx].roomNumber
				 << "  " << reservations[idx].roomType
				 << "  " << reservations[idx].nights << " night(s)" << endl;
		}

		cout << endl;
		boxTitle("Continue Payment");
		boxRow("1. Continue to Add On Menu / Payment");
		boxRow("2. Book another room first");
		boxRow("0. Back to Menu Page");
		boxLine();
		cout << " Please choose 0-2: ";
		int pendingChoice = getValidatedInput(0, 2);

		if (pendingChoice == 0) {
			return;
		}
		if (pendingChoice == 1) {
			afterBookingMenu();
			return;
		}
	}

	bool created = createOneBooking();
	if (created || !currentSessionIDs.empty()) {
		afterBookingMenu();
	}
}

bool createOneBooking() {
	cout << endl;
	boxTitle("How many guests?");
	boxRow("Enter how many people will stay");
	boxRow("Enter 0 to cancel");
	boxLine();
	cout << " Number of guests (1-6): ";
	int guests = getIntInRange(0, 6);
	if (guests == 0) {
		cout << " Booking cancelled." << endl;
		return false;
	}

	int shown = displayBookableRooms(guests);
	if (shown == 0) {
		cout << " No available room fits this number of guests." << endl;
		return false;
	}

	string roomNumber;
	int roomIndex = -1;

	while (true) {
		cout << "\n Enter room number to book or 0 to cancel: ";
		getline(cin, roomNumber);

		if (roomNumber == "0") {
			cout << " Booking cancelled." << endl;
			return false;
		}

		roomIndex = findRoomIndex(roomNumber);
		if (roomIndex == -1) {
			cout << " Room not found! Please try again." << endl;
			continue;
		}
		if (roomList[roomIndex].status != "Available") {
			cout << " This room is not available. Status: "
				 << roomList[roomIndex].status << ". Please choose another." << endl;
			continue;
		}
		if (roomList[roomIndex].capacity < guests) {
			cout << " That room only fits " << roomList[roomIndex].capacity
				 << " guest(s). Please choose another." << endl;
			continue;
		}
		break;
	}

	cout << "\n Enter number of nights or 0 to cancel: ";
	int nights = getIntInRange(0, 30);
	if (nights == 0) {
		cout << " Booking cancelled." << endl;
		return false;
	}

	int nowY = 0;
	int nowM = 0;
	int nowD = 0;
	int nowH = 0;
	int nowMin = 0;
	string bookingDate;
	string bookingTime;

	int inD = 0;
	int inM = 0;
	int inY = 0;
	if (!askCheckInDate(inD, inM, inY)) {
		cout << " Booking cancelled." << endl;
		return false;
	}

	int outD = inD;
	int outM = inM;
	int outY = inY;
	addDays(outD, outM, outY, nights);
	string checkInDate = makeDate(inD, inM, inY);
	string checkInTime = "Any time";
	string checkOutDate = makeDate(outD, outM, outY);
	string inWeek = weekdayName(inD, inM, inY);
	string outWeek = weekdayName(outD, outM, outY);

	cout << endl;
	boxTitle("Check-out");
	boxRow("Please check out before 12:00 noon");
	boxRow(outWeek + " " + checkOutDate);
	boxLine();

	double estimated = roundMoney(roomList[roomIndex].price * nights);

	malaysiaNow(nowY, nowM, nowD, nowH, nowMin);
	bookingDate = makeDate(nowD, nowM, nowY);
	bookingTime = makeClockTime(nowH, nowMin);

	cout << fixed << setprecision(2);
	cout << endl;
	boxTitle("Booking Summary");
	boxRow("Guest        : " + currentCustomerName());
	boxRow("Room Number  : " + roomList[roomIndex].roomNumber);
	boxRow("Room Type    : " + roomList[roomIndex].roomType);
	{
		ostringstream line;
		line << "Guests       : " << guests;
		boxRow(line.str());
	}
	{
		ostringstream line;
		line << "Nights       : " << nights;
		boxRow(line.str());
	}
	boxRow("Booking Date : " + bookingDate + "  " + bookingTime);
	boxRow("Check-in     : " + inWeek + " " + checkInDate + "  (any time)");
	boxRow("Check-out    : " + outWeek + " " + checkOutDate + "  before 12:00");
	{
		ostringstream line;
		line << fixed << setprecision(2) << "Price/Night  : RM " << roomList[roomIndex].price;
		boxRow(line.str());
	}
	{
		ostringstream line;
		line << fixed << setprecision(2) << "Room Charge  : RM " << estimated;
		boxRow(line.str());
	}
	boxLine();
	cout << " Add-ons, promo codes and payment will appear after you confirm." << endl;

	if (!confirmYesNo(" Confirm booking? y/n: ")) {
		cout << " Booking cancelled." << endl;
		return false;
	}

	BookingRecord newBooking;
	newBooking.reservationID = generateReservationID();
	nextReservationID++;
	newBooking.customerUsername = currentLoggedInCustomer;
	newBooking.roomNumber = roomList[roomIndex].roomNumber;
	newBooking.roomType = roomList[roomIndex].roomType;
	newBooking.pricePerNight = roomList[roomIndex].price;
	newBooking.nights = nights;
	newBooking.guests = guests;
	newBooking.bookingDate = bookingDate;
	newBooking.bookingTime = bookingTime;
	newBooking.checkInDate = checkInDate;
	newBooking.checkInTime = checkInTime;
	newBooking.checkOutDate = checkOutDate;
	newBooking.status = "Confirmed";
	newBooking.paymentStatus = "Unpaid";
	newBooking.paymentMethod = "-";
	newBooking.promoCode = "";
	newBooking.specialRequest = "-";
	newBooking.discount = 0;
	newBooking.loyaltyDiscount = 0;
	newBooking.serviceCharge = 0;
	newBooking.sst = 0;
	newBooking.totalAmount = 0;

	reservations.push_back(newBooking);
	currentSessionIDs.push_back(newBooking.reservationID);
	roomList[roomIndex].status = "Occupied";

	cout << "\n Reservation confirmed! Your reservation ID is "
		 << newBooking.reservationID << "." << endl;
	maybeGiveWelcomeGift(static_cast<int>(reservations.size()) - 1);
	return true;
}

void offerRoomUpgrade(int& roomIndex, int guests) {
	string nextType = "";
	if (roomList[roomIndex].roomType == "Twin" && guests >= 2) {
		nextType = "Deluxe";
	}
	else if (roomList[roomIndex].roomType == "Family" && guests >= 4) {
		nextType = "Suite";
	}
	else {
		return;
	}

	int upgradeIndex = -1;
	for (size_t i = 0; i < roomList.size(); i++) {
		if (roomList[i].roomType == nextType
			&& roomList[i].status == "Available"
			&& roomList[i].capacity >= guests) {
			upgradeIndex = static_cast<int>(i);
			break;
		}
	}
	if (upgradeIndex == -1) {
		return;
	}

	double extra = roomList[upgradeIndex].price - roomList[roomIndex].price;
	cout << "\n Upgrade offer: " << nextType << " Room "
		 << roomList[upgradeIndex].roomNumber << " is available." << endl;
	cout << " Your " << roomList[roomIndex].roomType << " is RM "
		 << fixed << setprecision(2) << roomList[roomIndex].price << " per night." << endl;
	cout << " " << nextType << " is RM " << roomList[upgradeIndex].price
		 << " per night. Extra RM " << extra << " for a nicer stay." << endl;
	if (confirmYesNo(" Upgrade now? y/n: ")) {
		roomIndex = upgradeIndex;
		cout << " Upgraded to Room " << roomList[roomIndex].roomNumber
			 << " " << roomList[roomIndex].roomType << "." << endl;
	}
}

void maybeGiveWelcomeGift(int resIndex) {
	if (sessionSurpriseGiven) {
		return;
	}
	sessionSurpriseGiven = true;
	if (rand() % 4 != 0) {
		return;
	}

	SelectedAddOn gift;
	gift.name = "Welcome Fruit Platter Complimentary";
	gift.unitPrice = 0;
	gift.quantity = 1;
	gift.lineTotal = 0;
	reservations[resIndex].addOns.push_back(gift);
	cout << " Surprise gift: complimentary welcome fruit platter added to your room!" << endl;
}

