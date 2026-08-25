#include "hotel.h"

void viewMyReservations() {
	cout << endl;
	boxLine();
	boxCenter("YOUR BOOKING HISTORY");
	boxSplitLine();
	int count = 0;

	for (size_t i = 0; i < reservations.size(); i++) {
		if (reservations[i].customerUsername != currentLoggedInCustomer) {
			continue;
		}
		count++;
		boxSplitRow("Reservation #" + to_string(count), "ID: " + reservations[i].reservationID);
		boxSplitRow("Room:", reservations[i].roomNumber + " " + reservations[i].roomType);
		boxSplitRow("Guests:", to_string(reservations[i].guests));
		boxSplitRow("Nights:", to_string(reservations[i].nights) + " night(s)");
		boxSplitRow("Booking Date:", reservations[i].bookingDate + "  " + reservations[i].bookingTime);
		{
			int d = 0;
			int m = 0;
			int y = 0;
			string inText = reservations[i].checkInDate + "  " + reservations[i].checkInTime;
			if (parseDate(reservations[i].checkInDate, d, m, y)) {
				inText = weekdayName(d, m, y) + " " + inText;
			}
			boxSplitRow("Check-in:", inText);
		}
		{
			int d = 0;
			int m = 0;
			int y = 0;
			string outText = reservations[i].checkOutDate + "  before 12:00 noon";
			if (parseDate(reservations[i].checkOutDate, d, m, y)) {
				outText = weekdayName(d, m, y) + " " + outText;
			}
			boxSplitRow("Check-out:", outText);
		}
		if (reservations[i].paymentStatus == "Paid") {
			ostringstream amount;
			amount << fixed << setprecision(2) << "RM " << reservations[i].totalAmount;
			boxSplitRow("Amount:", amount.str());
			boxSplitRow("Payment Method:", reservations[i].paymentMethod);
		}
		else {
			boxSplitRow("Amount:", "Unpaid");
			boxSplitRow("Payment Method:", "-");
		}
		boxSplitRow("Status:", reservations[i].status);
		if (!reservations[i].addOns.empty()) {
			string addText = "";
			for (size_t j = 0; j < reservations[i].addOns.size(); j++) {
				if (j > 0) {
					addText += ", ";
				}
				addText += reservations[i].addOns[j].name + " x" + to_string(reservations[i].addOns[j].quantity);
			}
			boxSplitRow("Add-ons:", addText);
		}
		if (reservations[i].specialRequest != "-" && !reservations[i].specialRequest.empty()) {
			boxSplitRow("Request:", reservations[i].specialRequest);
		}
		if (reservations[i].paymentStatus == "Unpaid") {
			boxSplitRow("Note:", "Choose 2 on Menu Page to pay");
		}
		boxSplitLine();
	}

	if (count == 0) {
		boxSplitRow("Total Bookings:", "0 booking(s)");
		boxLine();
	}
	else {
		boxSplitRow("Total Bookings:", to_string(count) + " booking(s)");
		boxLine();
	}

	if (currentUserHasUnpaid()) {
		if (confirmYesNo(" Go to Add On Menu to pay now? y/n: ")) {
			loadUnpaidIntoSession();
			afterBookingMenu();
		}
	}
}

