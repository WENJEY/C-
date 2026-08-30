#include "../hotel.h"

string parentDir(const string& path) {
	size_t pos = path.find_last_of("\\/");
	if (pos == string::npos || pos == 0) {
		return "";
	}
	return path.substr(0, pos);
}

bool storageFileExists(const string& path) {
	ifstream in(path.c_str());
	return in.good();
}

string projectRootFolder() {
	char exePath[MAX_PATH] = {0};
	GetModuleFileNameA(NULL, exePath, MAX_PATH);
	string dir = parentDir(exePath);

	for (int i = 0; i < 8 && !dir.empty(); i++) {
		if (storageFileExists(dir + "/hotel.h") || storageFileExists(dir + "/CMakeLists.txt")) {
			return dir;
		}
		dir = parentDir(dir);
	}
	return "";
}

string dataFolderPath() {
	static string cached;
	if (cached.empty()) {
		string root = projectRootFolder();
		if (!root.empty()) {
			cached = root + "/data";
		}
		else {
			cached = "data";
		}
	}
	return cached;
}

void ensureDataFolder() {
	CreateDirectoryA(dataFolderPath().c_str(), NULL);
}

string dataFile(const string& name) {
	return dataFolderPath() + "/" + name;
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

void writeHeader(ostream& out, const string& title) {
	out << "========================================" << endl;
	out << "Grand Horizon Hotel" << endl;
	out << title << endl;
	out << "========================================" << endl;
}

void writeSection(ostream& out, const string& title) {
	out << endl;
	out << "---------- " << title << " ----------" << endl;
}

void writeField(ostream& out, const string& label, const string& value) {
	ostringstream line;
	line << left << setw(20) << label << ": " << value;
	out << line.str() << endl;
}

void writeField(ostream& out, const string& label, int value) {
	ostringstream num;
	num << value;
	writeField(out, label, num.str());
}

void writeField(ostream& out, const string& label, double value) {
	ostringstream num;
	num << fixed << setprecision(2) << value;
	writeField(out, label, num.str());
}

string fieldLabel(const string& line) {
	size_t pos = line.find(':');
	if (pos == string::npos) {
		return "";
	}
	string label = line.substr(0, pos);
	trimFileField(label);
	return label;
}

string fieldValue(const string& line) {
	size_t pos = line.find(':');
	if (pos == string::npos) {
		return "";
	}
	string value = line.substr(pos + 1);
	trimFileField(value);
	return value;
}

bool isIntegerLine(const string& text) {
	if (text.empty()) {
		return false;
	}
	size_t i = 0;
	if (text[0] == '+' || text[0] == '-') {
		i = 1;
	}
	if (i >= text.size()) {
		return false;
	}
	for (; i < text.size(); i++) {
		if (!isdigit(static_cast<unsigned char>(text[i]))) {
			return false;
		}
	}
	return true;
}

bool isSectionLine(const string& line) {
	string text = line;
	trimFileField(text);
	return text.find("----------") != string::npos || text == "---";
}

bool isSkipLine(const string& line) {
	string text = line;
	trimFileField(text);
	if (text.empty()) {
		return true;
	}
	if (text.find("=====") != string::npos) {
		return true;
	}
	if (text == "Grand Horizon Hotel") {
		return true;
	}
	if (text == "Customers" || text == "Rooms" || text == "Reservations" || text == "Hotels") {
		return true;
	}
	return false;
}

int toIntSafe(const string& text) {
	if (text.empty()) {
		return 0;
	}
	return stoi(text);
}

double toDoubleSafe(const string& text) {
	if (text.empty()) {
		return 0;
	}
	return stod(text);
}

vector<string> readStorageLines(ifstream& file) {
	vector<string> lines;
	string line;
	while (getline(file, line)) {
		trimFileField(line);
		lines.push_back(line);
	}
	return lines;
}

bool looksLabeled(const vector<string>& lines) {
	for (size_t i = 0; i < lines.size(); i++) {
		if (lines[i].find(':') != string::npos) {
			return true;
		}
	}
	return false;
}

SelectedAddOn parseAddOnLine(const string& value) {
	SelectedAddOn item;
	stringstream ss(value);
	string pricePart;
	string qtyPart;
	string totalPart;
	getline(ss, item.name, '|');
	getline(ss, pricePart, '|');
	getline(ss, qtyPart, '|');
	getline(ss, totalPart, '|');
	trimFileField(item.name);
	trimFileField(pricePart);
	trimFileField(qtyPart);
	trimFileField(totalPart);
	item.unitPrice = toDoubleSafe(pricePart);
	item.quantity = toIntSafe(qtyPart);
	item.lineTotal = toDoubleSafe(totalPart);
	return item;
}

void saveCustomersToFile() {
	ensureDataFolder();
	ofstream file(dataFile("customers.txt"));
	if (!file.is_open()) {
		return;
	}

	writeHeader(file, "Customers");
	writeField(file, "Total", static_cast<int>(customers.size()));

	for (size_t i = 0; i < customers.size(); i++) {
		ostringstream title;
		title << "Customer " << (i + 1);
		writeSection(file, title.str());
		writeField(file, "Username", customers[i].username);
		writeField(file, "Age", customers[i].age);
		writeField(file, "Password", customers[i].password);
		writeField(file, "Full Name", customers[i].fullName);
		writeField(file, "Email", customers[i].email);
		writeField(file, "Phone", customers[i].phoneNumber);
		writeField(file, "IC/Passport", customers[i].icPassport);
		writeField(file, "Membership", customers[i].membershipStatus);
		writeField(file, "Loyalty Points", customers[i].loyaltyPoints);
	}
	file.close();
}

void loadCustomersLabeled(const vector<string>& lines) {
	customers.clear();
	Customer current;
	bool inRecord = false;

	for (size_t i = 0; i < lines.size(); i++) {
		if (isSkipLine(lines[i])) {
			continue;
		}
		if (isSectionLine(lines[i])) {
			if (inRecord && !current.username.empty()) {
				customers.push_back(current);
			}
			current = Customer();
			current.membershipStatus = "Regular";
			current.loyaltyPoints = 0;
			inRecord = true;
			continue;
		}

		string label = fieldLabel(lines[i]);
		string value = fieldValue(lines[i]);
		if (label.empty() || label == "Total") {
			continue;
		}
		if (!inRecord) {
			inRecord = true;
			current.membershipStatus = "Regular";
			current.loyaltyPoints = 0;
		}

		if (label == "Username") {
			current.username = value;
		}
		else if (label == "Age") {
			current.age = value;
		}
		else if (label == "Password") {
			current.password = value;
		}
		else if (label == "Full Name") {
			current.fullName = value;
		}
		else if (label == "Email") {
			current.email = value;
		}
		else if (label == "Phone") {
			current.phoneNumber = value;
		}
		else if (label == "IC/Passport") {
			current.icPassport = value;
		}
		else if (label == "Membership") {
			current.membershipStatus = value.empty() ? "Regular" : value;
		}
		else if (label == "Loyalty Points") {
			current.loyaltyPoints = toIntSafe(value);
		}
	}

	if (inRecord && !current.username.empty()) {
		if (current.fullName.empty()) {
			current.fullName = current.username;
		}
		customers.push_back(current);
	}
}

void loadCustomersLegacy(const vector<string>& lines) {
	if (lines.empty() || !isIntegerLine(lines[0])) {
		return;
	}

	int numCustomers = toIntSafe(lines[0]);
	customers.clear();
	size_t index = 1;

	for (int i = 0; i < numCustomers && index < lines.size(); i++) {
		Customer customer;
		if (index < lines.size()) {
			customer.username = lines[index++];
		}
		if (index < lines.size()) {
			customer.age = lines[index++];
		}
		if (index < lines.size()) {
			customer.password = lines[index++];
		}
		if (index < lines.size()) {
			customer.fullName = lines[index++];
		}
		if (index < lines.size()) {
			customer.email = lines[index++];
		}
		if (index < lines.size()) {
			customer.phoneNumber = lines[index++];
		}
		if (index < lines.size()) {
			customer.icPassport = lines[index++];
		}
		if (index < lines.size()) {
			customer.membershipStatus = lines[index++];
		}
		string pointsStr;
		if (index < lines.size()) {
			pointsStr = lines[index++];
		}
		if (index < lines.size() && lines[index] == "---") {
			index++;
		}

		if (customer.fullName.empty() || customer.fullName == "---") {
			customer.fullName = customer.username;
		}
		if (customer.membershipStatus.empty() || customer.membershipStatus == "---") {
			customer.membershipStatus = "Regular";
			customer.loyaltyPoints = 0;
		}
		else {
			customer.loyaltyPoints = toIntSafe(pointsStr);
		}
		customers.push_back(customer);
	}
}

void loadCustomersFromFile() {
	ifstream file(dataFile("customers.txt"));
	if (!file.is_open()) {
		return;
	}
	vector<string> lines = readStorageLines(file);
	file.close();
	if (looksLabeled(lines)) {
		loadCustomersLabeled(lines);
	}
	else {
		loadCustomersLegacy(lines);
	}
}

void saveRoomsToFile() {
	ensureDataFolder();
	ofstream file(dataFile("rooms.txt"));
	if (!file.is_open()) {
		return;
	}

	writeHeader(file, "Rooms");
	writeField(file, "Total", static_cast<int>(roomList.size()));

	for (size_t i = 0; i < roomList.size(); i++) {
		ostringstream title;
		title << "Room " << (i + 1);
		writeSection(file, title.str());
		writeField(file, "Room Number", roomList[i].roomNumber);
		writeField(file, "Room Type", roomList[i].roomType);
		writeField(file, "Capacity", roomList[i].capacity);
		writeField(file, "Price", roomList[i].price);
		writeField(file, "Status", roomList[i].status);
	}
	file.close();
}

void loadRoomsLabeled(const vector<string>& lines) {
	roomList.clear();
	Room current;
	bool inRecord = false;

	for (size_t i = 0; i < lines.size(); i++) {
		if (isSkipLine(lines[i])) {
			continue;
		}
		if (isSectionLine(lines[i])) {
			if (inRecord && !current.roomNumber.empty()) {
				roomList.push_back(current);
			}
			current = Room();
			inRecord = true;
			continue;
		}

		string label = fieldLabel(lines[i]);
		string value = fieldValue(lines[i]);
		if (label.empty() || label == "Total") {
			continue;
		}
		if (!inRecord) {
			inRecord = true;
		}

		if (label == "Room Number") {
			current.roomNumber = value;
		}
		else if (label == "Room Type") {
			current.roomType = value;
		}
		else if (label == "Capacity") {
			current.capacity = toIntSafe(value);
		}
		else if (label == "Price") {
			current.price = toDoubleSafe(value);
		}
		else if (label == "Status") {
			current.status = value;
		}
	}

	if (inRecord && !current.roomNumber.empty()) {
		roomList.push_back(current);
	}
}

void loadRoomsLegacy(const vector<string>& lines) {
	if (lines.empty() || !isIntegerLine(lines[0])) {
		return;
	}

	int numRooms = toIntSafe(lines[0]);
	if (numRooms <= 0) {
		return;
	}

	roomList.clear();
	size_t index = 1;
	for (int i = 0; i < numRooms && index < lines.size(); i++) {
		Room room;
		if (index < lines.size()) {
			room.roomNumber = lines[index++];
		}
		if (index < lines.size()) {
			room.roomType = lines[index++];
		}
		string capacityStr;
		string priceStr;
		if (index < lines.size()) {
			capacityStr = lines[index++];
		}
		if (index < lines.size()) {
			priceStr = lines[index++];
		}
		if (index < lines.size()) {
			room.status = lines[index++];
		}
		if (index < lines.size() && lines[index] == "---") {
			index++;
		}
		room.capacity = toIntSafe(capacityStr);
		room.price = toDoubleSafe(priceStr);
		roomList.push_back(room);
	}
}

void loadRoomsFromFile() {
	ifstream file(dataFile("rooms.txt"));
	if (!file.is_open()) {
		return;
	}
	vector<string> lines = readStorageLines(file);
	file.close();
	if (looksLabeled(lines)) {
		loadRoomsLabeled(lines);
	}
	else {
		loadRoomsLegacy(lines);
	}
}

void saveReservationsToFile() {
	ensureDataFolder();
	ofstream file(dataFile("reservations.txt"));
	if (!file.is_open()) {
		return;
	}

	writeHeader(file, "Reservations");
	writeField(file, "Total", static_cast<int>(reservations.size()));
	writeField(file, "Next Reservation ID", nextReservationID);
	writeField(file, "Next Invoice No", nextInvoiceNo);

	for (size_t i = 0; i < reservations.size(); i++) {
		ostringstream title;
		title << "Reservation " << (i + 1);
		writeSection(file, title.str());
		writeField(file, "Reservation ID", reservations[i].reservationID);
		writeField(file, "Customer", reservations[i].customerUsername);
		writeField(file, "Hotel Name", reservations[i].hotelName);
		writeField(file, "Hotel Address", reservations[i].hotelAddress);
		writeField(file, "Hotel Area", reservations[i].hotelArea);
		writeField(file, "Hotel State", reservations[i].hotelState);
		writeField(file, "Room Number", reservations[i].roomNumber);
		writeField(file, "Room Type", reservations[i].roomType);
		writeField(file, "Price Per Night", reservations[i].pricePerNight);
		writeField(file, "Nights", reservations[i].nights);
		writeField(file, "Guests", reservations[i].guests);
		writeField(file, "Booking Date", reservations[i].bookingDate);
		writeField(file, "Booking Time", reservations[i].bookingTime);
		writeField(file, "Check-in Date", reservations[i].checkInDate);
		writeField(file, "Check-in Time", reservations[i].checkInTime);
		writeField(file, "Check-out Date", reservations[i].checkOutDate);
		writeField(file, "Status", reservations[i].status);
		writeField(file, "Payment Status", reservations[i].paymentStatus);
		writeField(file, "Payment Method", reservations[i].paymentMethod);
		writeField(file, "Promo Code", reservations[i].promoCode);
		writeField(file, "Special Request", reservations[i].specialRequest);
		writeField(file, "Discount", reservations[i].discount);
		writeField(file, "Loyalty Discount", reservations[i].loyaltyDiscount);
		writeField(file, "Service Charge", reservations[i].serviceCharge);
		writeField(file, "SST", reservations[i].sst);
		writeField(file, "Total Amount", reservations[i].totalAmount);
		writeField(file, "Add-on Count", static_cast<int>(reservations[i].addOns.size()));
		for (size_t j = 0; j < reservations[i].addOns.size(); j++) {
			ostringstream addon;
			addon << reservations[i].addOns[j].name << " | "
				  << fixed << setprecision(2) << reservations[i].addOns[j].unitPrice << " | "
				  << reservations[i].addOns[j].quantity << " | "
				  << fixed << setprecision(2) << reservations[i].addOns[j].lineTotal;
			writeField(file, "Add-on", addon.str());
		}
	}
	file.close();
}

void loadReservationsLabeled(const vector<string>& lines) {
	reservations.clear();
	BookingRecord current;
	bool inRecord = false;
	bool sawIds = false;

	for (size_t i = 0; i < lines.size(); i++) {
		if (isSkipLine(lines[i])) {
			continue;
		}
		if (isSectionLine(lines[i])) {
			if (inRecord && !current.reservationID.empty()) {
				reservations.push_back(current);
			}
			current = BookingRecord();
			inRecord = true;
			continue;
		}

		string label = fieldLabel(lines[i]);
		string value = fieldValue(lines[i]);
		if (label.empty() || label == "Total" || label == "Add-on Count") {
			continue;
		}
		if (label == "Next Reservation ID") {
			nextReservationID = toIntSafe(value);
			sawIds = true;
			continue;
		}
		if (label == "Next Invoice No") {
			nextInvoiceNo = toIntSafe(value);
			sawIds = true;
			continue;
		}
		if (!inRecord) {
			inRecord = true;
		}

		if (label == "Reservation ID") {
			current.reservationID = value;
		}
		else if (label == "Customer") {
			current.customerUsername = value;
		}
		else if (label == "Hotel Name") {
			current.hotelName = value;
		}
		else if (label == "Hotel Address") {
			current.hotelAddress = value;
		}
		else if (label == "Hotel Area") {
			current.hotelArea = value;
		}
		else if (label == "Hotel State") {
			current.hotelState = value;
		}
		else if (label == "Room Number") {
			current.roomNumber = value;
		}
		else if (label == "Room Type") {
			current.roomType = value;
		}
		else if (label == "Price Per Night") {
			current.pricePerNight = toDoubleSafe(value);
		}
		else if (label == "Nights") {
			current.nights = toIntSafe(value);
		}
		else if (label == "Guests") {
			current.guests = toIntSafe(value);
		}
		else if (label == "Booking Date") {
			current.bookingDate = value;
		}
		else if (label == "Booking Time") {
			current.bookingTime = value;
		}
		else if (label == "Check-in Date") {
			current.checkInDate = value;
		}
		else if (label == "Check-in Time") {
			current.checkInTime = value;
		}
		else if (label == "Check-out Date") {
			current.checkOutDate = value;
		}
		else if (label == "Status") {
			current.status = value;
		}
		else if (label == "Payment Status") {
			current.paymentStatus = value;
		}
		else if (label == "Payment Method") {
			current.paymentMethod = value;
		}
		else if (label == "Promo Code") {
			current.promoCode = value;
		}
		else if (label == "Special Request") {
			current.specialRequest = value;
		}
		else if (label == "Discount") {
			current.discount = toDoubleSafe(value);
		}
		else if (label == "Loyalty Discount") {
			current.loyaltyDiscount = toDoubleSafe(value);
		}
		else if (label == "Service Charge") {
			current.serviceCharge = toDoubleSafe(value);
		}
		else if (label == "SST") {
			current.sst = toDoubleSafe(value);
		}
		else if (label == "Total Amount") {
			current.totalAmount = toDoubleSafe(value);
		}
		else if (label == "Add-on") {
			current.addOns.push_back(parseAddOnLine(value));
		}
	}

	if (inRecord && !current.reservationID.empty()) {
		reservations.push_back(current);
	}
	if (!sawIds && nextReservationID < 1) {
		nextReservationID = 1;
	}
}

void loadReservationsLegacy(const vector<string>& lines) {
	if (lines.size() < 3 || !isIntegerLine(lines[0])) {
		return;
	}

	int numReservations = toIntSafe(lines[0]);
	nextReservationID = toIntSafe(lines[1]);
	nextInvoiceNo = toIntSafe(lines[2]);
	reservations.clear();
	size_t index = 3;

	for (int i = 0; i < numReservations && index < lines.size(); i++) {
		BookingRecord booking;
		if (index < lines.size()) {
			booking.reservationID = lines[index++];
		}
		if (index < lines.size()) {
			booking.customerUsername = lines[index++];
		}
		if (index < lines.size()) {
			booking.hotelName = lines[index++];
		}
		if (index < lines.size()) {
			booking.hotelAddress = lines[index++];
		}
		if (index < lines.size()) {
			booking.hotelArea = lines[index++];
		}
		if (index < lines.size()) {
			booking.hotelState = lines[index++];
		}
		if (index < lines.size()) {
			booking.roomNumber = lines[index++];
		}
		if (index < lines.size()) {
			booking.roomType = lines[index++];
		}
		if (index < lines.size()) {
			booking.pricePerNight = toDoubleSafe(lines[index++]);
		}
		if (index < lines.size()) {
			booking.nights = toIntSafe(lines[index++]);
		}
		if (index < lines.size()) {
			booking.guests = toIntSafe(lines[index++]);
		}
		if (index < lines.size()) {
			booking.bookingDate = lines[index++];
		}
		if (index < lines.size()) {
			booking.bookingTime = lines[index++];
		}
		if (index < lines.size()) {
			booking.checkInDate = lines[index++];
		}
		if (index < lines.size()) {
			booking.checkInTime = lines[index++];
		}
		if (index < lines.size()) {
			booking.checkOutDate = lines[index++];
		}
		if (index < lines.size()) {
			booking.status = lines[index++];
		}
		if (index < lines.size()) {
			booking.paymentStatus = lines[index++];
		}
		if (index < lines.size()) {
			booking.paymentMethod = lines[index++];
		}
		if (index < lines.size()) {
			booking.promoCode = lines[index++];
		}
		if (index < lines.size()) {
			booking.specialRequest = lines[index++];
		}
		if (index < lines.size()) {
			booking.discount = toDoubleSafe(lines[index++]);
		}
		if (index < lines.size()) {
			booking.loyaltyDiscount = toDoubleSafe(lines[index++]);
		}
		if (index < lines.size()) {
			booking.serviceCharge = toDoubleSafe(lines[index++]);
		}
		if (index < lines.size()) {
			booking.sst = toDoubleSafe(lines[index++]);
		}
		if (index < lines.size()) {
			booking.totalAmount = toDoubleSafe(lines[index++]);
		}

		int addonCount = 0;
		if (index < lines.size()) {
			addonCount = toIntSafe(lines[index++]);
		}
		for (int a = 0; a < addonCount && index < lines.size(); a++) {
			booking.addOns.push_back(parseAddOnLine(lines[index++]));
		}
		if (index < lines.size() && lines[index] == "---") {
			index++;
		}
		reservations.push_back(booking);
	}
}

void loadReservationsFromFile() {
	ifstream file(dataFile("reservations.txt"));
	if (!file.is_open()) {
		return;
	}
	vector<string> lines = readStorageLines(file);
	file.close();
	if (looksLabeled(lines)) {
		loadReservationsLabeled(lines);
	}
	else {
		loadReservationsLegacy(lines);
	}
}

void saveHotelsToFile() {
	ensureDataFolder();
	ofstream file(dataFile("hotels.txt"));
	if (!file.is_open()) {
		return;
	}

	writeHeader(file, "Hotels");
	writeField(file, "Total", static_cast<int>(hotelBranches.size()));

	for (size_t i = 0; i < hotelBranches.size(); i++) {
		ostringstream title;
		title << "Hotel " << (i + 1);
		writeSection(file, title.str());
		writeField(file, "State", hotelBranches[i].state);
		writeField(file, "Area", hotelBranches[i].area);
		writeField(file, "Name", hotelBranches[i].name);
		writeField(file, "Address", hotelBranches[i].address);
	}
	file.close();
}

void loadHotelsLabeled(const vector<string>& lines) {
	hotelBranches.clear();
	HotelBranch current;
	bool inRecord = false;

	for (size_t i = 0; i < lines.size(); i++) {
		if (isSkipLine(lines[i])) {
			continue;
		}
		if (isSectionLine(lines[i])) {
			if (inRecord && !current.name.empty()) {
				hotelBranches.push_back(current);
			}
			current = HotelBranch();
			inRecord = true;
			continue;
		}

		string label = fieldLabel(lines[i]);
		string value = fieldValue(lines[i]);
		if (label.empty() || label == "Total") {
			continue;
		}
		if (!inRecord) {
			inRecord = true;
		}

		if (label == "State") {
			current.state = value;
		}
		else if (label == "Area") {
			current.area = value;
		}
		else if (label == "Name") {
			current.name = value;
		}
		else if (label == "Address") {
			current.address = value;
		}
	}

	if (inRecord && !current.name.empty()) {
		hotelBranches.push_back(current);
	}
}

void loadHotelsLegacy(const vector<string>& lines) {
	if (lines.empty() || !isIntegerLine(lines[0])) {
		return;
	}

	int numHotels = toIntSafe(lines[0]);
	if (numHotels <= 0) {
		return;
	}

	hotelBranches.clear();
	size_t index = 1;
	for (int i = 0; i < numHotels && index < lines.size(); i++) {
		HotelBranch hotel;
		if (index < lines.size()) {
			hotel.state = lines[index++];
		}
		if (index < lines.size()) {
			hotel.area = lines[index++];
		}
		if (index < lines.size()) {
			hotel.name = lines[index++];
		}
		if (index < lines.size()) {
			hotel.address = lines[index++];
		}
		if (index < lines.size() && lines[index] == "---") {
			index++;
		}
		hotelBranches.push_back(hotel);
	}
}

void loadHotelsFromFile() {
	ifstream file(dataFile("hotels.txt"));
	if (!file.is_open()) {
		return;
	}
	vector<string> lines = readStorageLines(file);
	file.close();
	if (looksLabeled(lines)) {
		loadHotelsLabeled(lines);
	}
	else {
		loadHotelsLegacy(lines);
	}
}
