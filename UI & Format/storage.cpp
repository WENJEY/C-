#include "../hotel.h"

void saveCustomersToFile() {
	ofstream file("customers.txt");
	if (file.is_open()) {
		file << customers.size() << endl;
		for (size_t i = 0; i < customers.size(); i++) {
			file << customers[i].username << endl;
			file << customers[i].age << endl;
			file << customers[i].password << endl;
			file << customers[i].fullName << endl;
			file << customers[i].email << endl;
			file << customers[i].phoneNumber << endl;
			file << customers[i].icPassport << endl;
			file << customers[i].membershipStatus << endl;
			file << customers[i].loyaltyPoints << endl;
			file << "---" << endl;
		}
		file.close();
	}
}

void trimFileField(string& text) {
	if (!text.empty() && text[text.length() - 1] == '\r') {
		text.erase(text.length() - 1);
	}
	size_t start = 0;
	while (start < text.length() && isspace(static_cast<unsigned char>(text[start]))) {
		start++;
	}
	size_t end = text.length();
	while (end > start && isspace(static_cast<unsigned char>(text[end - 1]))) {
		end--;
	}
	text = text.substr(start, end - start);
}

void loadCustomersFromFile() {
	ifstream file("customers.txt");
	if (!file.is_open()) {
		return;
	}

	int numCustomers = 0;
	file >> numCustomers;
	file.ignore();
	customers.clear();

	for (int i = 0; i < numCustomers; i++) {
		Customer customer;
		string separator;
		string pointsStr;
		getline(file, customer.username);
		getline(file, customer.age);
		getline(file, customer.password);
		getline(file, customer.fullName);
		getline(file, customer.email);
		getline(file, customer.phoneNumber);
		getline(file, customer.icPassport);
		getline(file, customer.membershipStatus);
		getline(file, pointsStr);
		getline(file, separator);

		trimFileField(customer.username);
		trimFileField(customer.age);
		trimFileField(customer.password);
		trimFileField(customer.fullName);
		trimFileField(customer.email);
		trimFileField(customer.phoneNumber);
		trimFileField(customer.icPassport);
		trimFileField(customer.membershipStatus);
		trimFileField(pointsStr);

		if (customer.fullName.empty() || customer.fullName == "---") {
			customer.fullName = customer.username;
		}

		if (customer.membershipStatus.empty() || customer.membershipStatus == "---") {
			customer.membershipStatus = "Regular";
			customer.loyaltyPoints = 0;
		}
		else if (pointsStr.empty()) {
			customer.loyaltyPoints = 0;
		}
		else {
			customer.loyaltyPoints = stoi(pointsStr);
		}
		customers.push_back(customer);
	}
	file.close();
}

void saveRoomsToFile() {
	ofstream file("rooms.txt");
	if (file.is_open()) {
		file << roomList.size() << endl;
		for (size_t i = 0; i < roomList.size(); i++) {
			file << roomList[i].roomNumber << endl;
			file << roomList[i].roomType << endl;
			file << roomList[i].capacity << endl;
			file << roomList[i].price << endl;
			file << roomList[i].status << endl;
			file << "---" << endl;
		}
		file.close();
	}
}

void loadRoomsFromFile() {
	ifstream file("rooms.txt");
	if (!file.is_open()) {
		return;
	}

	int numRooms = 0;
	file >> numRooms;
	file.ignore();
	if (numRooms <= 0) {
		file.close();
		return;
	}

	roomList.clear();
	for (int i = 0; i < numRooms; i++) {
		Room room;
		string capacityStr;
		string priceStr;
		string separator;
		getline(file, room.roomNumber);
		getline(file, room.roomType);
		getline(file, capacityStr);
		getline(file, priceStr);
		getline(file, room.status);
		getline(file, separator);
		room.capacity = stoi(capacityStr);
		room.price = stod(priceStr);
		roomList.push_back(room);
	}
	file.close();
}

void saveReservationsToFile() {
	ofstream file("reservations.txt");
	if (!file.is_open()) {
		return;
	}

	file << reservations.size() << endl;
	file << nextReservationID << endl;
	file << nextInvoiceNo << endl;

	for (size_t i = 0; i < reservations.size(); i++) {
		file << reservations[i].reservationID << endl;
		file << reservations[i].customerUsername << endl;
		file << reservations[i].roomNumber << endl;
		file << reservations[i].roomType << endl;
		file << reservations[i].pricePerNight << endl;
		file << reservations[i].nights << endl;
		file << reservations[i].guests << endl;
		file << reservations[i].bookingDate << endl;
		file << reservations[i].bookingTime << endl;
		file << reservations[i].checkInDate << endl;
		file << reservations[i].checkInTime << endl;
		file << reservations[i].checkOutDate << endl;
		file << reservations[i].status << endl;
		file << reservations[i].paymentStatus << endl;
		file << reservations[i].paymentMethod << endl;
		file << reservations[i].promoCode << endl;
		file << reservations[i].specialRequest << endl;
		file << reservations[i].discount << endl;
		file << reservations[i].loyaltyDiscount << endl;
		file << reservations[i].serviceCharge << endl;
		file << reservations[i].sst << endl;
		file << reservations[i].totalAmount << endl;
		file << reservations[i].addOns.size() << endl;
		for (size_t j = 0; j < reservations[i].addOns.size(); j++) {
			file << reservations[i].addOns[j].name << "|"
				 << reservations[i].addOns[j].unitPrice << "|"
				 << reservations[i].addOns[j].quantity << "|"
				 << reservations[i].addOns[j].lineTotal << endl;
		}
		file << "---" << endl;
	}
	file.close();
}

void loadReservationsFromFile() {
	ifstream file("reservations.txt");
	if (!file.is_open()) {
		return;
	}

	int numReservations = 0;
	file >> numReservations;
	file >> nextReservationID;
	file >> nextInvoiceNo;
	file.ignore();
	reservations.clear();

	for (int i = 0; i < numReservations; i++) {
		BookingRecord booking;
		string priceStr;
		string nightsStr;
		string guestsStr;
		string discountStr;
		string loyaltyStr;
		string serviceStr;
		string sstStr;
		string totalStr;
		string addonCountStr;
		string separator;

		getline(file, booking.reservationID);
		getline(file, booking.customerUsername);
		getline(file, booking.roomNumber);
		getline(file, booking.roomType);
		getline(file, priceStr);
		getline(file, nightsStr);
		getline(file, guestsStr);
		getline(file, booking.bookingDate);
		getline(file, booking.bookingTime);
		getline(file, booking.checkInDate);
		getline(file, booking.checkInTime);
		getline(file, booking.checkOutDate);
		getline(file, booking.status);
		getline(file, booking.paymentStatus);
		getline(file, booking.paymentMethod);
		getline(file, booking.promoCode);
		getline(file, booking.specialRequest);
		getline(file, discountStr);
		getline(file, loyaltyStr);
		getline(file, serviceStr);
		getline(file, sstStr);
		getline(file, totalStr);
		getline(file, addonCountStr);

		booking.pricePerNight = stod(priceStr);
		booking.nights = stoi(nightsStr);
		booking.guests = stoi(guestsStr);
		booking.discount = stod(discountStr);
		booking.loyaltyDiscount = stod(loyaltyStr);
		booking.serviceCharge = stod(serviceStr);
		booking.sst = stod(sstStr);
		booking.totalAmount = stod(totalStr);

		int addonCount = stoi(addonCountStr);
		for (int a = 0; a < addonCount; a++) {
			string line;
			getline(file, line);
			SelectedAddOn item;
			stringstream ss(line);
			string pricePart;
			string qtyPart;
			string totalPart;
			getline(ss, item.name, '|');
			getline(ss, pricePart, '|');
			getline(ss, qtyPart, '|');
			getline(ss, totalPart, '|');
			item.unitPrice = stod(pricePart);
			item.quantity = stoi(qtyPart);
			item.lineTotal = stod(totalPart);
			booking.addOns.push_back(item);
		}
		getline(file, separator);
		reservations.push_back(booking);
	}
	file.close();
}