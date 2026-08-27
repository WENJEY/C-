#include "../hotel.h"

int pickSessionBooking();

void afterBookingMenu() {
	int choice;

	do {
		if (currentSessionIDs.empty()) {
			return;
		}

		cout << endl;
		boxTitle("Add On Menu");
		boxRow("Your room is booked. Add extras, then pay.");
		boxRow("Typed something wrong? Use 7 to change or 8 to cancel.");
		boxLine();

		int stayNo = 0;
		for (size_t i = 0; i < currentSessionIDs.size(); i++) {
			int idx = findReservationIndex(currentSessionIDs[i]);
			if (idx == -1) {
				continue;
			}
			if (stayNo > 0) {
				boxLine();
			}
			stayNo++;

			boxRow("Booking ID   : #" + reservations[idx].reservationID);
			if (!reservations[idx].hotelName.empty()) {
				boxField("Hotel        : ", reservations[idx].hotelName);
				boxField("Address      : ", reservations[idx].hotelAddress);
			}
			boxRow("Room         : " + reservations[idx].roomNumber
				 + "  " + reservations[idx].roomType);
			{
				ostringstream line;
				line << "Guests       : " << reservations[idx].guests;
				boxRow(line.str());
			}
			{
				ostringstream line;
				line << "Nights       : " << reservations[idx].nights << " night(s)";
				boxRow(line.str());
			}
			boxRow("Check-in     : " + reservations[idx].checkInDate + "  (any time)");
			boxRow("Check-out    : " + reservations[idx].checkOutDate + "  before 12:00");
			boxRow("Payment      : " + reservations[idx].paymentStatus);
		}

		BillBreakdown preview = calculateSessionBill();
		{
			ostringstream line;
			line << fixed << setprecision(2) << "Est. total   : RM " << preview.total;
			boxRow(line.str());
		}
		boxRow("Includes service charge and 8% SST.");
		boxLine();
		boxRow("1. Hotel Add-ons");
		boxRow("2. View Bill / Make Payment");
		boxRow("3. Book Another Room");
		boxRow("4. Special Requests");
		boxRow("5. Apply Promo Code");
		boxRow("6. Redeem Loyalty Points");
		boxRow("7. Modify this booking");
		boxRow("8. Cancel this booking");
		boxLine();
		cout << " Please choose 1-8: ";
		choice = getIntInRange(1, 8);

		switch (choice) {
		case 1:
			offerAddOns();
			break;
		case 2:
			showSessionBill(true);
			if (confirmYesNo(" Proceed to payment now? y/n: ")) {
				processPayment();
				if (currentSessionIDs.empty()) {
					return;
				}
			}
			break;
		case 3:
			createOneBooking();
			break;
		case 4:
			specialRequestMenu();
			break;
		case 5:
			applyPromoCode();
			break;
		case 6:
			redeemLoyaltyPoints();
			break;
		case 7: {
			int idx = pickSessionBooking();
			if (idx != -1) {
				modifyBookingMenu(idx);
			}
			break;
		}
		case 8: {
			int idx = pickSessionBooking();
			if (idx != -1) {
				confirmAndCancelReservation(idx);
				if (currentSessionIDs.empty()) {
					cout << " Returning to Menu Page." << endl;
					return;
				}
			}
			break;
		}
		}
	} while (true);
}

int pickSessionBooking() {
	vector<int> choices;

	for (size_t i = 0; i < currentSessionIDs.size(); i++) {
		int idx = findReservationIndex(currentSessionIDs[i]);
		if (idx == -1) {
			continue;
		}
		if (reservations[idx].status == "Cancelled") {
			continue;
		}
		choices.push_back(idx);
	}

	if (choices.empty()) {
		cout << " No booking in this session." << endl;
		return -1;
	}
	if (choices.size() == 1) {
		return choices[0];
	}

	cout << endl;
	boxTitle("Which booking?");
	boxRow("Enter 0 to go back");
	boxLine();
	for (size_t i = 0; i < choices.size(); i++) {
		int idx = choices[i];
		ostringstream line;
		line << (i + 1) << ". #" << reservations[idx].reservationID
			 << "  Room " << reservations[idx].roomNumber
			 << "  " << reservations[idx].roomType;
		boxRow(line.str());
	}
	boxLine();
	cout << " Please choose 0-" << choices.size() << ": ";
	int pick = getIntInRange(0, static_cast<int>(choices.size()));
	if (pick == 0) {
		return -1;
	}
	return choices[pick - 1];
}

void offerAddOns() {
	if (currentSessionIDs.empty()) {
		cout << " No booking in this session." << endl;
		return;
	}

	int targetIndex = findReservationIndex(currentSessionIDs[0]);
	if (currentSessionIDs.size() > 1) {
		cout << "\n Which room should receive the add-ons?" << endl;
		for (size_t i = 0; i < currentSessionIDs.size(); i++) {
			int idx = findReservationIndex(currentSessionIDs[i]);
			if (idx == -1) {
				continue;
			}
			cout << " " << (i + 1) << ". Room " << reservations[idx].roomNumber
				 << " " << reservations[idx].roomType << endl;
		}
		cout << " 0. Cancel" << endl;
		cout << " Choose: ";
		int pick = getIntInRange(0, static_cast<int>(currentSessionIDs.size()));
		if (pick == 0) {
			return;
		}
		targetIndex = findReservationIndex(currentSessionIDs[pick - 1]);
	}

	if (targetIndex != -1) {
		addOnsForReservation(targetIndex);
	}
}

void addOnsForReservation(int resIndex) {
	int choice;

	do {
		cout << endl;
		boxTitle("Hotel Add-ons");
		{
			ostringstream line;
			line << "Room " << reservations[resIndex].roomNumber
				 << "   Guests: " << reservations[resIndex].guests
				 << "   Nights: " << reservations[resIndex].nights;
			boxRow(line.str());
		}
		boxLine();
		for (size_t i = 0; i < addOnList.size(); i++) {
			ostringstream line;
			line << (i + 1) << ". " << left << setw(22) << addOnList[i].name
				 << " RM " << setw(6) << fixed << setprecision(2) << addOnList[i].price
				 << " " << addOnList[i].unit;
			boxRow(line.str());
		}
		boxRow("0. Done with add-ons");
		boxLine();

		if (!reservations[resIndex].addOns.empty()) {
			cout << " Already added:" << endl;
			for (size_t i = 0; i < reservations[resIndex].addOns.size(); i++) {
				cout << " - " << reservations[resIndex].addOns[i].name
					 << " x" << reservations[resIndex].addOns[i].quantity
					 << "  RM " << fixed << setprecision(2) << reservations[resIndex].addOns[i].lineTotal << endl;
			}
		}

		cout << " Choose add-on 0-" << addOnList.size() << ": ";
		choice = getIntInRange(0, static_cast<int>(addOnList.size()));
		if (choice == 0) {
			return;
		}

		const AddOnItem& item = addOnList[choice - 1];
		cout << " Quantity for " << item.name << " (1-20 or 0 to cancel): ";
		int qty = getIntInRange(0, 20);
		if (qty == 0) {
			continue;
		}

		double line = roundMoney(item.price * qty);
		bool merged = false;
		for (size_t i = 0; i < reservations[resIndex].addOns.size(); i++) {
			if (reservations[resIndex].addOns[i].name == item.name) {
				reservations[resIndex].addOns[i].quantity += qty;
				reservations[resIndex].addOns[i].lineTotal = roundMoney(
					reservations[resIndex].addOns[i].unitPrice * reservations[resIndex].addOns[i].quantity);
				merged = true;
				break;
			}
		}

		if (!merged) {
			SelectedAddOn selected;
			selected.name = item.name;
			selected.unitPrice = item.price;
			selected.quantity = qty;
			selected.lineTotal = line;
			reservations[resIndex].addOns.push_back(selected);
		}

		cout << " Added " << item.name << " x" << qty
			 << "  RM " << fixed << setprecision(2) << line << "." << endl;
	} while (true);
}

void specialRequestMenu() {
	if (currentSessionIDs.empty()) {
		cout << " No booking in this session." << endl;
		return;
	}

	int resIndex = findReservationIndex(currentSessionIDs.back());
	if (currentSessionIDs.size() > 1) {
		cout << "\n Which room is this request for?" << endl;
		for (size_t i = 0; i < currentSessionIDs.size(); i++) {
			int idx = findReservationIndex(currentSessionIDs[i]);
			if (idx == -1) {
				continue;
			}
			cout << " " << (i + 1) << ". Room " << reservations[idx].roomNumber << endl;
		}
		cout << " 0. Cancel" << endl;
		cout << " Choose: ";
		int pick = getIntInRange(0, static_cast<int>(currentSessionIDs.size()));
		if (pick == 0) {
			return;
		}
		resIndex = findReservationIndex(currentSessionIDs[pick - 1]);
	}

	if (resIndex == -1) {
		return;
	}

	cout << endl;
	boxTitle("Special Requests");
	boxRow("1. High floor");
	boxRow("2. Quiet room / away from lift");
	boxRow("3. Extra pillows");
	boxRow("4. Baby crib");
	boxRow("5. Connecting rooms");
	boxRow("6. Type my own request");
	boxRow("0. Cancel");
	boxLine();
	cout << " Please choose 0-6: ";
	int choice = getValidatedInput(0, 6);

	string request = reservations[resIndex].specialRequest;
	if (request == "-") {
		request = "";
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
	case 0:
		return;
	}

	if (extra.empty()) {
		cout << " No request added." << endl;
		return;
	}

	if (request.empty()) {
		request = extra;
	}
	else {
		request = request + ", " + extra;
	}

	reservations[resIndex].specialRequest = request;
	cout << " Noted! We will try our best: " << request << endl;
}
