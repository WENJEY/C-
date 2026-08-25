#include "hotel.h"

void applyPromoCode() {
	cout << endl;
	cout << " Enter promo code (0 to cancel): ";

	string code;
	getline(cin, code);
	if (code == "0" || code.empty()) {
		return;
	}

	for (size_t i = 0; i < code.length(); i++) {
		code[i] = static_cast<char>(toupper(static_cast<unsigned char>(code[i])));
	}

	int totalNights = 0;
	double roomCharge = 0;
	for (size_t i = 0; i < currentSessionIDs.size(); i++) {
		int idx = findReservationIndex(currentSessionIDs[i]);
		if (idx == -1) {
			continue;
		}
		totalNights += reservations[idx].nights;
		roomCharge += reservations[idx].pricePerNight * reservations[idx].nights;
		roomCharge += addOnTotal(reservations[idx]);
	}

	sessionPromoPercent = 0;
	sessionPromoFlat = 0;
	sessionPromoCode = "";

	if (code == "WELCOME10") {
		sessionPromoPercent = 0.10;
	}
	else if (code == "HORIZON15") {
		if (totalNights < 3) {
			cout << " HORIZON15 needs at least 3 nights in this stay." << endl;
			return;
		}
		sessionPromoPercent = 0.15;
	}
	else if (code == "WEEKEND20") {
		sessionPromoPercent = 0.20;
	}
	else if (code == "STUDENT5") {
		sessionPromoFlat = 5.00;
	}
	else if (code == "GOLDEN50") {
		if (roomCharge < 400.00) {
			cout << " GOLDEN50 needs a subtotal of RM 400.00 or more." << endl;
			return;
		}
		sessionPromoFlat = 50.00;
	}
	else {
		cout << " Invalid promo code." << endl;
		return;
	}

	sessionPromoCode = code;
	cout << " Promo " << code << " applied!" << endl;
	showSessionBill(false);
}

void redeemLoyaltyPoints() {
	int points = loyaltyPointsOfCurrentUser();
	cout << "\n Your loyalty points: " << points << endl;
	cout << " Redeem " << REDEEM_BLOCK << " points = RM " << fixed << setprecision(2) << REDEEM_VALUE << " off." << endl;

	if (points < REDEEM_BLOCK) {
		cout << " Not enough points yet. Keep booking to earn more!" << endl;
		return;
	}

	int maxBlocks = points / REDEEM_BLOCK;
	BillBreakdown bill = calculateSessionBill();
	int affordable = static_cast<int>(bill.afterDiscount / REDEEM_VALUE);
	if (affordable < 1) {
		cout << " Current bill is too small to redeem points." << endl;
		return;
	}
	if (maxBlocks > affordable) {
		maxBlocks = affordable;
	}

	cout << " You can redeem up to " << (maxBlocks * REDEEM_BLOCK) << " points." << endl;
	cout << " Enter points to redeem (multiples of " << REDEEM_BLOCK << " or 0 to cancel): ";
	int redeem = getIntInRange(0, maxBlocks * REDEEM_BLOCK);
	if (redeem == 0) {
		return;
	}
	if (redeem % REDEEM_BLOCK != 0) {
		cout << " Points must be in blocks of " << REDEEM_BLOCK << "." << endl;
		return;
	}

	sessionRedeemedPoints = redeem;
	cout << " " << redeem << " points reserved for this payment." << endl;
	showSessionBill(false);
}

void showSessionBill(bool showPayHint) {
	BillBreakdown bill = calculateSessionBill();
	cout << endl;
	boxTitle("Your Bill");
	boxRow("Guest           : " + currentCustomerName());
	boxRow("Member          : " + membershipOfCurrentUser());
	if (!currentHotelName.empty()) {
		boxRow("Hotel           : " + currentHotelName);
		boxRow("Address         : " + currentHotelAddress);
	}
	boxLine();

	for (size_t i = 0; i < currentSessionIDs.size(); i++) {
		int idx = findReservationIndex(currentSessionIDs[i]);
		if (idx == -1) {
			continue;
		}
		double roomCharge = roundMoney(reservations[idx].pricePerNight * reservations[idx].nights);
		ostringstream head;
		head << "#" << reservations[idx].reservationID
			 << " Room " << reservations[idx].roomNumber
			 << " " << reservations[idx].roomType
			 << "  " << reservations[idx].nights << " night(s)";
		boxRow(head.str());
		{
			ostringstream line;
			line << fixed << setprecision(2) << "Room charge     : RM " << roomCharge;
			boxRow(line.str());
		}
		for (size_t j = 0; j < reservations[idx].addOns.size(); j++) {
			ostringstream line;
			line << fixed << setprecision(2) << reservations[idx].addOns[j].name
				 << " x" << reservations[idx].addOns[j].quantity
				 << " : RM " << reservations[idx].addOns[j].lineTotal;
			boxRow(line.str());
		}
		if (reservations[idx].specialRequest != "-") {
			boxRow("Request         : " + reservations[idx].specialRequest);
		}
	}

	boxLine();
	{
		ostringstream line;
		line << fixed << setprecision(2) << "Subtotal        : RM " << bill.subtotal;
		boxRow(line.str());
	}
	if (bill.promoDiscount > 0) {
		ostringstream line;
		line << fixed << setprecision(2) << "Promo " << sessionPromoCode << "      : -RM " << bill.promoDiscount;
		boxRow(line.str());
	}
	if (bill.memberDiscount > 0) {
		ostringstream line;
		line << fixed << setprecision(2) << "Member discount : -RM " << bill.memberDiscount;
		boxRow(line.str());
	}
	if (bill.loyaltyDiscount > 0) {
		ostringstream line;
		line << fixed << setprecision(2) << "Loyalty points  : -RM " << bill.loyaltyDiscount;
		boxRow(line.str());
	}
	{
		ostringstream line;
		line << fixed << setprecision(2) << "Service 10%     : RM " << bill.serviceCharge;
		boxRow(line.str());
	}
	{
		ostringstream line;
		line << fixed << setprecision(2) << "SST 8%          : RM " << bill.sst;
		boxRow(line.str());
	}
	{
		ostringstream line;
		line << fixed << setprecision(2) << "TOTAL PAYABLE   : RM " << bill.total;
		boxRow(line.str());
	}
	boxLine();

	if (showPayHint) {
		cout << " Payment methods: Card / Touch n Go / GrabPay / Boost" << endl;
	}
}

void processPayment() {
	bool alreadyPaid = true;
	for (size_t i = 0; i < currentSessionIDs.size(); i++) {
		int idx = findReservationIndex(currentSessionIDs[i]);
		if (idx != -1 && reservations[idx].paymentStatus == "Unpaid") {
			alreadyPaid = false;
			break;
		}
	}
	if (alreadyPaid) {
		cout << " This stay is already paid." << endl;
		return;
	}

	BillBreakdown bill = calculateSessionBill();
	showSessionBill(false);

	cout << endl;
	boxTitle("Payment Method");
	boxRow("1. Debit / Credit Card");
	boxRow("2. Touch n Go eWallet");
	boxRow("3. GrabPay");
	boxRow("4. Boost");
	boxRow("0. Cancel payment");
	boxLine();
	cout << " Please choose 0-4: ";
	int method = getValidatedInput(0, 4);
	if (method == 0) {
		cout << " Payment cancelled." << endl;
		return;
	}

	string methodName;
	if (method == 1) {
		methodName = "Card";
	}
	else if (method == 2) {
		methodName = "Touch n Go";
	}
	else if (method == 3) {
		methodName = "GrabPay";
	}
	else {
		methodName = "Boost";
	}

	if (method == 1) {
		while (true) {
			cout << " Enter 16-digit card number or 0 to cancel: ";
			string card;
			getline(cin, card);
			if (card == "0") {
				cout << " Payment cancelled." << endl;
				return;
			}

			string digits = "";
			for (size_t i = 0; i < card.length(); i++) {
				if (isdigit(card[i])) {
					digits += card[i];
				}
			}
			if (digits.length() != 16) {
				cout << " Card number must contain 16 digits." << endl;
				continue;
			}
			cout << " Charging card ending " << digits.substr(12, 4) << " ..." << endl;
			break;
		}
	}
	else {
		cout << " Pay RM " << fixed << setprecision(2) << bill.total
			 << " with " << methodName << " using the phone number on your profile." << endl;
		if (!confirmYesNo(" Confirm e-wallet payment? y/n: ")) {
			cout << " Payment cancelled." << endl;
			return;
		}
	}

	int earned = static_cast<int>(bill.total / 10.0) * POINTS_PER_RM10;
	int custIndex = findCustomerIndex(currentLoggedInCustomer);
	if (custIndex != -1 && sessionRedeemedPoints > 0) {
		customers[custIndex].loyaltyPoints -= sessionRedeemedPoints;
		if (customers[custIndex].loyaltyPoints < 0) {
			customers[custIndex].loyaltyPoints = 0;
		}
	}

	for (size_t i = 0; i < currentSessionIDs.size(); i++) {
		int idx = findReservationIndex(currentSessionIDs[i]);
		if (idx == -1) {
			continue;
		}
		double ownSubtotal = reservations[idx].pricePerNight * reservations[idx].nights + addOnTotal(reservations[idx]);
		double share = 0.0;
		if (bill.subtotal > 0.0) {
			share = ownSubtotal / bill.subtotal;
		}
		reservations[idx].paymentStatus = "Paid";
		reservations[idx].status = "Paid";
		reservations[idx].paymentMethod = methodName;
		reservations[idx].promoCode = sessionPromoCode;
		reservations[idx].discount = roundMoney((bill.promoDiscount + bill.memberDiscount) * share);
		reservations[idx].loyaltyDiscount = roundMoney(bill.loyaltyDiscount * share);
		reservations[idx].serviceCharge = roundMoney(bill.serviceCharge * share);
		reservations[idx].sst = roundMoney(bill.sst * share);
		reservations[idx].totalAmount = roundMoney(bill.total * share);
	}

	addLoyaltyPoints(earned);
	saveCustomersToFile();
	printAndSaveInvoice(bill, methodName);

	cout << "\n Payment successful! Thank you, " << currentCustomerName() << "." << endl;
	cout << " You earned " << earned << " loyalty point(s)." << endl;
	if (custIndex != -1) {
		cout << " Membership: " << customers[custIndex].membershipStatus
			 << "  |  Points: " << customers[custIndex].loyaltyPoints << endl;
	}
	if (rand() % 5 == 0) {
		cout << " Lucky draw: complimentary late checkout voucher for your next stay!" << endl;
	}

	currentSessionIDs.clear();
	resetSessionExtras();
	pauseEnter();
}

void printAndSaveInvoice(const BillBreakdown& bill, const string& method) {
	string invoiceId = padNumber(nextInvoiceNo, 4);
	string invoiceNo = "INV-" + invoiceId;
	nextInvoiceNo++;

	int y = 0;
	int m = 0;
	int d = 0;
	int h = 0;
	int min = 0;
	malaysiaNow(y, m, d, h, min);
	string payDate = makeDate(d, m, y);
	string payTime = makeClockTime(h, min);

	ostringstream receipt;
	auto lineEq = [&]() {
		receipt << " +" << string(BOX_W, '=') << "+\n";
	};
	auto lineDash = [&]() {
		receipt << " +" << string(BOX_W, '-') << "+\n";
	};
	auto center = [&](const string& text) {
		string t = text;
		if (static_cast<int>(t.length()) > BOX_W) {
			t = t.substr(0, BOX_W);
		}
		int pad = BOX_W - static_cast<int>(t.length());
		int leftPad = pad / 2;
		int rightPad = pad - leftPad;
		receipt << " |" << string(leftPad, ' ') << t << string(rightPad, ' ') << "|\n";
	};
	auto inner = [&](const string& leftText, const string& rightText) {
		string body(BOX_W, ' ');
		for (size_t i = 0; i < leftText.length() && i < static_cast<size_t>(BOX_W); i++) {
			body[i] = leftText[i];
		}
		int start = BOX_W - static_cast<int>(rightText.length());
		if (start < 0) {
			start = 0;
		}
		for (size_t i = 0; i < rightText.length() && start + static_cast<int>(i) < BOX_W; i++) {
			body[start + i] = rightText[i];
		}
		receipt << " |" << body << "|\n";
	};
	auto money = [&](const string& label, double amount, bool minus) {
		ostringstream val;
		val << fixed << setprecision(2);
		if (minus) {
			val << "-RM " << amount;
		}
		else {
			val << "RM " << amount;
		}
		inner(" " + label, val.str() + " ");
	};

	receipt << endl;
	lineEq();
	center("GRAND HORIZON HOTEL");
	if (!currentHotelName.empty()) {
		center(currentHotelName);
	}
	center("** RECEIPT **");
	lineEq();
	inner(" Date: " + payDate, "Time: " + payTime + " ");
	inner(" Invoice No: " + invoiceNo, "");
	lineEq();
	center("STAY DETAILS");
	lineEq();
	inner(" Guest          : " + currentCustomerName(), "");
	for (size_t i = 0; i < currentSessionIDs.size(); i++) {
		int idx = findReservationIndex(currentSessionIDs[i]);
		if (idx == -1) {
			continue;
		}
		inner(" Reservation ID : " + reservations[idx].reservationID, "");
		if (!reservations[idx].hotelName.empty()) {
			inner(" Hotel          : " + reservations[idx].hotelName, "");
			inner(" Address        : " + reservations[idx].hotelAddress, "");
		}
		inner(" Room           : " + reservations[idx].roomNumber + " " + reservations[idx].roomType, "");
		inner(" Booking Date   : " + reservations[idx].bookingDate + "  " + reservations[idx].bookingTime, "");
		{
			int d = 0;
			int m = 0;
			int y = 0;
			string inText = reservations[idx].checkInDate + "  " + reservations[idx].checkInTime;
			if (parseDate(reservations[idx].checkInDate, d, m, y)) {
				inText = weekdayName(d, m, y) + " " + inText;
			}
			inner(" Check-in       : " + inText, "");
		}
		{
			int d = 0;
			int m = 0;
			int y = 0;
			string outText = reservations[idx].checkOutDate + "  before 12:00 noon";
			if (parseDate(reservations[idx].checkOutDate, d, m, y)) {
				outText = weekdayName(d, m, y) + " " + outText;
			}
			inner(" Check-out      : " + outText, "");
		}
		inner(" Nights         : " + to_string(reservations[idx].nights) + " night(s)", "");
		inner(" Guests         : " + to_string(reservations[idx].guests), "");
		for (size_t j = 0; j < reservations[idx].addOns.size(); j++) {
			ostringstream addName;
			addName << " Add-on: " << reservations[idx].addOns[j].name
					<< " x" << reservations[idx].addOns[j].quantity;
			ostringstream addPrice;
			addPrice << fixed << setprecision(2) << "RM " << reservations[idx].addOns[j].lineTotal << " ";
			inner(addName.str(), addPrice.str());
		}
	}
	lineEq();
	center("PAYMENT BREAKDOWN");
	lineEq();
	if (currentSessionIDs.size() == 1) {
		int idx = findReservationIndex(currentSessionIDs[0]);
		if (idx != -1) {
			ostringstream roomLabel;
			roomLabel << fixed << setprecision(2) << "Room (" << reservations[idx].nights
					  << " x RM " << reservations[idx].pricePerNight << "):";
			money(roomLabel.str(), bill.roomCharge, false);
		}
		else {
			money("Room charge:", bill.roomCharge, false);
		}
	}
	else {
		money("Room charge:", bill.roomCharge, false);
	}
	money("Add-ons:", bill.addOnCharge, false);
	if (bill.promoDiscount + bill.memberDiscount > 0) {
		money("Promo / Member:", bill.promoDiscount + bill.memberDiscount, true);
	}
	if (bill.loyaltyDiscount > 0) {
		money("Loyalty:", bill.loyaltyDiscount, true);
	}
	money("Service Charge:", bill.serviceCharge, false);
	money("Tax (8%):", bill.sst, false);
	lineDash();
	money("TOTAL AMOUNT:", bill.total, false);
	lineEq();
	inner(" Payment Method: " + method, "");
	lineEq();
	center("Thank you for staying with us!");
	center("Enjoy your stay!");
	lineEq();

	cout << receipt.str();

	string fileName = "invoice_" + invoiceId + ".txt";
	ofstream file(fileName);
	if (file.is_open()) {
		file << receipt.str();
		file.close();
		cout << " Receipt saved as " << fileName << endl;
	}
}

double addOnTotal(const BookingRecord& booking) {
	double total = 0;
	for (size_t i = 0; i < booking.addOns.size(); i++) {
		total += booking.addOns[i].lineTotal;
	}
	return roundMoney(total);
}

BillBreakdown calculateSessionBill() {
	BillBreakdown bill;
	bill.roomCharge = 0;
	bill.addOnCharge = 0;

	for (size_t i = 0; i < currentSessionIDs.size(); i++) {
		int idx = findReservationIndex(currentSessionIDs[i]);
		if (idx == -1) {
			continue;
		}
		bill.roomCharge += reservations[idx].pricePerNight * reservations[idx].nights;
		bill.addOnCharge += addOnTotal(reservations[idx]);
	}

	bill.roomCharge = roundMoney(bill.roomCharge);
	bill.addOnCharge = roundMoney(bill.addOnCharge);
	bill.subtotal = roundMoney(bill.roomCharge + bill.addOnCharge);
	bill.promoDiscount = roundMoney(bill.subtotal * sessionPromoPercent + sessionPromoFlat);
	if (bill.promoDiscount > bill.subtotal) {
		bill.promoDiscount = bill.subtotal;
	}

	double afterPromo = roundMoney(bill.subtotal - bill.promoDiscount);
	bill.memberDiscount = roundMoney(afterPromo * membershipRate(membershipOfCurrentUser()));
	bill.afterDiscount = roundMoney(afterPromo - bill.memberDiscount);
	bill.loyaltyDiscount = roundMoney((sessionRedeemedPoints / REDEEM_BLOCK) * REDEEM_VALUE);
	if (bill.loyaltyDiscount > bill.afterDiscount) {
		bill.loyaltyDiscount = bill.afterDiscount;
	}
	bill.afterDiscount = roundMoney(bill.afterDiscount - bill.loyaltyDiscount);
	bill.serviceCharge = roundMoney(bill.afterDiscount * SERVICE_CHARGE_RATE);
	bill.sst = roundMoney((bill.afterDiscount + bill.serviceCharge) * SST_RATE);
	bill.total = roundMoney(bill.afterDiscount + bill.serviceCharge + bill.sst);
	return bill;
}
