#include "hotel.h"

int pickBookingToCancel();
void showCancelSummary(int resIndex);
double refundAmountFor(int resIndex, int daysLeft, string& policyText);
void printCancelSlip(int resIndex, double refund, const string& policyText);
void cancelOneReservation(int resIndex);

void cancelReservations() {
	while (true) {
		int resIndex = pickBookingToCancel();
		if (resIndex == -1) {
			return;
		}
		confirmAndCancelReservation(resIndex);
	}
}

bool confirmAndCancelReservation(int resIndex) {
	if (resIndex < 0) {
		return false;
	}
	showCancelSummary(resIndex);
	if (!confirmYesNo(" Cancel this reservation? y/n: ")) {
		cout << " Reservation kept." << endl;
		return false;
	}
	cancelOneReservation(resIndex);
	return true;
}

int pickBookingToCancel() {
	vector<int> choices;

	cout << endl;
	boxTitle("Cancel Reservations");
	boxRow("Choose a booking to cancel");
	boxRow("Cancelled bookings stay in history");
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
		if (!reservations[i].hotelName.empty()) {
			boxRow("   " + reservations[i].hotelName);
		}
		boxRow("   Check-in " + reservations[i].checkInDate
			 + "   " + reservations[i].paymentStatus);
	}

	if (choices.empty()) {
		boxRow("No active booking to cancel.");
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

void showCancelSummary(int resIndex) {
	const BookingRecord& b = reservations[resIndex];
	int d = 0;
	int m = 0;
	int y = 0;
	int daysLeft = 0;
	if (parseDate(b.checkInDate, d, m, y)) {
		daysLeft = daysFromToday(d, m, y);
	}

	cout << endl;
	boxTitle("Cancel #" + b.reservationID);
	boxRow("Guest        : " + currentCustomerName());
	if (!b.hotelName.empty()) {
		boxField("Hotel        : ", b.hotelName);
		boxField("Address      : ", b.hotelAddress);
	}
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
	boxRow("Check-in     : " + b.checkInDate);
	boxRow("Check-out    : " + b.checkOutDate);
	boxRow("Payment      : " + b.paymentStatus);
	if (b.paymentStatus == "Paid") {
		ostringstream line;
		line << fixed << setprecision(2) << "Paid amount  : RM " << b.totalAmount;
		boxRow(line.str());
	}

	string policyText;
	double refund = refundAmountFor(resIndex, daysLeft, policyText);
	boxRow("Policy       : " + policyText);
	{
		ostringstream line;
		line << fixed << setprecision(2) << "Refund       : RM " << refund;
		boxRow(line.str());
	}
	boxLine();
}

double refundAmountFor(int resIndex, int daysLeft, string& policyText) {
	const BookingRecord& b = reservations[resIndex];
	if (b.paymentStatus != "Paid") {
		policyText = "Unpaid booking - no charge";
		return 0.00;
	}

	if (daysLeft < 0) {
		policyText = "Stay already started - no refund";
		return 0.00;
	}
	if (daysLeft == 0) {
		policyText = "Same-day cancel - no refund";
		return 0.00;
	}
	if (daysLeft == 1) {
		policyText = "1 day before check-in - 50% refund";
		return roundMoney(b.totalAmount * 0.50);
	}

	policyText = "2+ days before check-in - full refund";
	return roundMoney(b.totalAmount);
}

void printCancelSlip(int resIndex, double refund, const string& policyText) {
	const BookingRecord& b = reservations[resIndex];
	int y = 0;
	int m = 0;
	int d = 0;
	int h = 0;
	int min = 0;
	malaysiaNow(y, m, d, h, min);

	cout << endl;
	boxTitle("Cancellation Slip");
	if (!b.hotelName.empty()) {
		boxRow(b.hotelName);
		boxRow(b.hotelAddress);
	}
	else {
		boxRow("Grand Horizon Hotel");
	}
	boxRow("Date         : " + makeDate(d, m, y) + "  " + makeClockTime(h, min));
	boxRow("Guest        : " + currentCustomerName());
	boxRow("Reservation  : #" + b.reservationID);
	boxRow("Room         : " + b.roomNumber + "  " + b.roomType);
	boxRow("Check-in     : " + b.checkInDate);
	boxRow("Status       : Cancelled");
	boxRow("Policy       : " + policyText);
	{
		ostringstream line;
		line << fixed << setprecision(2) << "Refund       : RM " << refund;
		boxRow(line.str());
	}
	if (refund > 0.0) {
		boxRow("Refund will be returned to the original payment method.");
	}
	boxRow("The room is now available for other guests.");
	boxLine();
}

void cancelOneReservation(int resIndex) {
	int d = 0;
	int m = 0;
	int y = 0;
	int daysLeft = 0;
	if (parseDate(reservations[resIndex].checkInDate, d, m, y)) {
		daysLeft = daysFromToday(d, m, y);
	}

	string policyText;
	double refund = refundAmountFor(resIndex, daysLeft, policyText);
	string roomNumber = reservations[resIndex].roomNumber;

	reservations[resIndex].status = "Cancelled";
	setRoomStatus(roomNumber, "Available");
	removeFromCurrentSession(reservations[resIndex].reservationID);

	if (refund > 0.0) {
		int toRemove = static_cast<int>(refund / 10.0) * POINTS_PER_RM10;
		int custIndex = findCustomerIndex(currentLoggedInCustomer);
		if (custIndex != -1 && toRemove > 0) {
			customers[custIndex].loyaltyPoints -= toRemove;
			if (customers[custIndex].loyaltyPoints < 0) {
				customers[custIndex].loyaltyPoints = 0;
			}
			refreshMembershipTier(custIndex);
			saveCustomersToFile();
			cout << " " << toRemove
				 << " loyalty point(s) from this stay were removed." << endl;
		}
	}

	printCancelSlip(resIndex, refund, policyText);
	cout << " Reservation #" << reservations[resIndex].reservationID
		 << " has been cancelled." << endl;
	pauseEnter();
}