#include "hotel.h"

void staffRoomMenu();
void staffPrintRooms();
void staffListRooms();
void staffAddRoom();
void staffEditRoom();
void staffDeleteRoom();
void staffLocationMenu();
void staffListHotels();
void staffAddHotel();
void staffEditHotel();
void staffDeleteHotel();
void staffReports();
void staffViewAllBookings();
void staffSearchCustomer();
void staffTodayCheckIns();
bool roomHasActiveBooking(const string& roomNumber);
bool askLine(const string& prompt, string& out);
bool askMoney(const string& prompt, double& amount);
string pickStateForStaff(bool allowNew);
string pickAreaForStaff(const string& state, bool allowNew);
int pickHotelIndexForStaff(const string& state, const string& area);
string pickRoomStatus();
string pickRoomType();
void showStaffHotel(int idx);

void staffLogin() {
	string id;
	string password;
	int attempts = 0;
	const int MAX_ATTEMPTS = 3;

	while (attempts < MAX_ATTEMPTS) {
		cout << endl;
		boxTitle("Staff Login");
		if (attempts > 0) {
			ostringstream line;
			line << "Attempt " << attempts + 1 << " of " << MAX_ATTEMPTS;
			boxRow(line.str());
			boxLine();
		}

		cout << " Username or 0 to cancel: ";
		getline(cin, id);
		if (id == "0") {
			cout << " Login cancelled." << endl;
			return;
		}

		cout << " Password or 0 to cancel: ";
		password = getSecurePassword(false);
		if (password == "0") {
			cout << " Login cancelled." << endl;
			return;
		}

		if (id == STAFF_ID && password == STAFF_PASSWORD) {
			cout << " Login successful! Welcome, " << id << "!" << endl;
			loadingPause();
			staffMenu();
			return;
		}

		attempts++;
		if (attempts < MAX_ATTEMPTS) {
			cout << " Login failed! Invalid username or password." << endl;
			cout << " You have " << (MAX_ATTEMPTS - attempts) << " attempt(s) remaining." << endl;
			if (!confirmYesNo(" Would you like to try again? y/n: ")) {
				cout << " Login cancelled." << endl;
				return;
			}
		}
		else {
			cout << " Login failed! Maximum attempts exceeded." << endl;
			cout << " Access denied. Returning to main menu." << endl;
			return;
		}
	}
}

void staffMenu() {
	int choice;
	do {
		cout << endl;
		boxTitle("Staff Menu");
		boxRow("Room and location changes last until you close the program.");
		boxLine();
		boxRow(optionText(1) + "Manage rooms");
		boxRow(optionText(2) + "Manage locations");
		boxRow(optionText(3) + "Reports and statistics");
		boxRow(optionText(4) + "View all bookings");
		boxRow(optionText(5) + "Search customer");
		boxRow(optionText(6) + "Today's check-ins");
		boxRow(optionText(0) + "Logout");
		boxLine();
		cout << " Please choose 0-6: ";
		choice = getIntInRange(0, 6);

		switch (choice) {
		case 1:
			staffRoomMenu();
			break;
		case 2:
			staffLocationMenu();
			break;
		case 3:
			staffReports();
			break;
		case 4:
			staffViewAllBookings();
			break;
		case 5:
			staffSearchCustomer();
			break;
		case 6:
			staffTodayCheckIns();
			break;
		case 0:
			cout << "\n Staff logged out." << endl;
			return;
		}
	} while (choice != 0);
}

void staffRoomMenu() {
	int choice;
	do {
		cout << endl;
		boxTitle("Manage rooms");
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
	cout << endl;
	boxTitle("All rooms");
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
	pauseEnter();
}

void staffAddRoom() {
	Room room;
	if (!askLine(" Room number (or 0 to cancel): ", room.roomNumber)) {
		return;
	}
	if (findRoomIndex(room.roomNumber) != -1) {
		cout << " That room number already exists." << endl;
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
		return;
	}

	if (!askMoney(" Price per night RM (or 0 to cancel): ", room.price)) {
		return;
	}

	cout << endl;
	boxTitle("Starting status");
	boxRow(optionText(1) + "Available");
	boxRow(optionText(2) + "Cleaning");
	boxRow(optionText(3) + "Maintenance");
	boxRow(optionText(0) + "Cancel");
	boxLine();
	cout << " Please choose 0-3: ";
	int statusPick = getIntInRange(0, 3);
	if (statusPick == 0) {
		cout << " Add room cancelled." << endl;
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
}

void staffEditRoom() {
	staffPrintRooms();
	string roomNumber;
	if (!askLine(" Room number to edit (or 0 to cancel): ", roomNumber)) {
		return;
	}
	int idx = findRoomIndex(roomNumber);
	if (idx == -1) {
		cout << " Room not found." << endl;
		return;
	}

	cout << endl;
	boxTitle("Edit room " + roomList[idx].roomNumber);
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
		}
	}
	else if (choice == 2) {
		cout << " New capacity 1-6 (or 0 to cancel): ";
		int capacity = getIntInRange(0, 6);
		if (capacity == 0) {
			cout << " Capacity not changed." << endl;
			return;
		}
		roomList[idx].capacity = capacity;
		cout << " Capacity updated." << endl;
	}
	else if (choice == 3) {
		double price = 0;
		if (askMoney(" New price per night RM (or 0 to cancel): ", price)) {
			roomList[idx].price = price;
			cout << " Price updated." << endl;
		}
	}
	else if (choice == 4) {
		string status = pickRoomStatus();
		if (status.empty()) {
			return;
		}
		if (status == "Available" && roomHasActiveBooking(roomList[idx].roomNumber)) {
			cout << " This room still has an active booking. Status not changed." << endl;
			return;
		}
		roomList[idx].status = status;
		cout << " Status updated." << endl;
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
		cout << " Room not found." << endl;
		return;
	}
	if (roomHasActiveBooking(roomNumber)) {
		cout << " Cannot delete. This room still has an active booking." << endl;
		return;
	}
	if (!confirmYesNo(" Delete room " + roomNumber + "? y/n: ")) {
		cout << " Room kept." << endl;
		return;
	}
	roomList.erase(roomList.begin() + static_cast<size_t>(idx));
	cout << " Room " << roomNumber << " deleted." << endl;
}

void staffLocationMenu() {
	int choice;
	do {
		cout << endl;
		boxTitle("Manage locations");
		boxRow(optionText(1) + "View hotels");
		boxRow(optionText(2) + "Add a hotel");
		boxRow(optionText(3) + "Edit a hotel");
		boxRow(optionText(4) + "Delete a hotel");
		boxRow(optionText(0) + "Back");
		boxLine();
		cout << " Please choose 0-4: ";
		choice = getIntInRange(0, 4);
		if (choice == 1) {
			staffListHotels();
		}
		else if (choice == 2) {
			staffAddHotel();
		}
		else if (choice == 3) {
			staffEditHotel();
		}
		else if (choice == 4) {
			staffDeleteHotel();
		}
	} while (choice != 0);
}

void staffListHotels() {
	string state = pickStateForStaff(false);
	if (state.empty()) {
		return;
	}
	string area = pickAreaForStaff(state, false);
	if (area.empty()) {
		return;
	}

	vector<int> list = hotelsOf(state, area);
	cout << endl;
	boxTitle("Hotels in " + area + ", " + state);
	if (list.empty()) {
		boxRow("No hotel in this area.");
		boxLine();
		pauseEnter();
		return;
	}
	for (size_t i = 0; i < list.size(); i++) {
		int idx = list[i];
		boxRow(optionText(static_cast<int>(i + 1)) + hotelBranches[static_cast<size_t>(idx)].name);
		boxWrapHang(string(4, ' ') + hotelBranches[static_cast<size_t>(idx)].address, 4);
		if (i + 1 < list.size()) {
			boxRow("");
		}
	}
	boxLine();
	pauseEnter();
}

void staffAddHotel() {
	string state = pickStateForStaff(true);
	if (state.empty()) {
		return;
	}
	string area = pickAreaForStaff(state, true);
	if (area.empty()) {
		return;
	}

	HotelBranch hotel;
	hotel.state = state;
	hotel.area = area;
	if (!askLine(" Hotel name (or 0 to cancel): ", hotel.name)) {
		return;
	}
	if (!askLine(" Address (or 0 to cancel): ", hotel.address)) {
		return;
	}

	hotelBranches.push_back(hotel);
	cout << endl;
	boxTitle("Hotel added");
	boxField("Hotel   : ", hotel.name);
	boxField("Area    : ", hotel.area + ", " + hotel.state);
	boxField("Address : ", hotel.address);
	boxLine();
	cout << " Customers can pick this hotel when they book." << endl;
}

void staffEditHotel() {
	string state = pickStateForStaff(false);
	if (state.empty()) {
		return;
	}
	string area = pickAreaForStaff(state, false);
	if (area.empty()) {
		return;
	}
	int idx = pickHotelIndexForStaff(state, area);
	if (idx < 0) {
		return;
	}

	showStaffHotel(idx);
	boxRow(optionText(1) + "Change name");
	boxRow(optionText(2) + "Change address");
	boxRow(optionText(3) + "Move to another state / area");
	boxRow(optionText(0) + "Back");
	boxLine();
	cout << " Please choose 0-3: ";
	int choice = getIntInRange(0, 3);
	if (choice == 0) {
		return;
	}
	if (choice == 1) {
		string name;
		if (askLine(" New hotel name (or 0 to cancel): ", name)) {
			string oldName = hotelBranches[static_cast<size_t>(idx)].name;
			hotelBranches[static_cast<size_t>(idx)].name = name;
			if (currentHotelName == oldName) {
				currentHotelName = name;
			}
			cout << " Name updated." << endl;
		}
	}
	else if (choice == 2) {
		string address;
		if (askLine(" New address (or 0 to cancel): ", address)) {
			hotelBranches[static_cast<size_t>(idx)].address = address;
			if (currentHotelName == hotelBranches[static_cast<size_t>(idx)].name) {
				currentHotelAddress = address;
			}
			cout << " Address updated." << endl;
		}
	}
	else if (choice == 3) {
		string newState = pickStateForStaff(true);
		if (newState.empty()) {
			return;
		}
		string newArea = pickAreaForStaff(newState, true);
		if (newArea.empty()) {
			return;
		}
		hotelBranches[static_cast<size_t>(idx)].state = newState;
		hotelBranches[static_cast<size_t>(idx)].area = newArea;
		if (currentHotelName == hotelBranches[static_cast<size_t>(idx)].name) {
			currentHotelState = newState;
			currentHotelArea = newArea;
		}
		cout << " Location updated." << endl;
	}
}

void staffDeleteHotel() {
	string state = pickStateForStaff(false);
	if (state.empty()) {
		return;
	}
	string area = pickAreaForStaff(state, false);
	if (area.empty()) {
		return;
	}
	int idx = pickHotelIndexForStaff(state, area);
	if (idx < 0) {
		return;
	}

	showStaffHotel(idx);
	if (!confirmYesNo(" Delete this hotel from the list? y/n: ")) {
		cout << " Hotel kept." << endl;
		return;
	}

	string removedName = hotelBranches[static_cast<size_t>(idx)].name;
	hotelBranches.erase(hotelBranches.begin() + static_cast<size_t>(idx));
	if (currentHotelName == removedName) {
		clearCurrentHotel();
	}
	cout << " Hotel deleted. Old bookings still keep this hotel name in history." << endl;
}

void showStaffHotel(int idx) {
	cout << endl;
	boxTitle("Hotel");
	boxField("Hotel   : ", hotelBranches[static_cast<size_t>(idx)].name);
	boxField("Area    : ", hotelBranches[static_cast<size_t>(idx)].area
		+ ", " + hotelBranches[static_cast<size_t>(idx)].state);
	boxField("Address : ", hotelBranches[static_cast<size_t>(idx)].address);
	boxLine();
}

void staffReports() {
	int paid = 0;
	int unpaid = 0;
	int cancelled = 0;
	int active = 0;
	double revenue = 0;
	int regular = 0;
	int silver = 0;
	int gold = 0;
	int available = 0;
	int occupied = 0;
	int cleaning = 0;
	int maintenance = 0;

	for (size_t i = 0; i < reservations.size(); i++) {
		if (reservations[i].status == "Cancelled") {
			cancelled++;
			continue;
		}
		active++;
		if (reservations[i].paymentStatus == "Paid") {
			paid++;
			revenue += reservations[i].totalAmount;
		}
		else {
			unpaid++;
		}
	}

	for (size_t i = 0; i < customers.size(); i++) {
		if (customers[i].membershipStatus == "Gold") {
			gold++;
		}
		else if (customers[i].membershipStatus == "Silver") {
			silver++;
		}
		else {
			regular++;
		}
	}

	for (size_t i = 0; i < roomList.size(); i++) {
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

	cout << endl;
	boxTitle("Reports and statistics");
	{
		ostringstream line;
		line << customers.size();
		boxField("Customers     : ", line.str());
	}
	{
		ostringstream line;
		line << "Regular " << regular << "  Silver " << silver << "  Gold " << gold;
		boxField("Members       : ", line.str());
	}
	{
		ostringstream line;
		line << hotelBranches.size();
		boxField("Hotels        : ", line.str());
	}
	{
		ostringstream line;
		line << roomList.size();
		boxField("Rooms         : ", line.str());
	}
	{
		ostringstream line;
		line << "Avail " << available << "  Occ " << occupied
			 << "  Clean " << cleaning << "  Maint " << maintenance;
		boxField("Room status   : ", line.str());
	}
	boxLine();
	{
		ostringstream line;
		line << reservations.size();
		boxField("All bookings  : ", line.str());
	}
	{
		ostringstream line;
		line << active;
		boxField("Active        : ", line.str());
	}
	{
		ostringstream line;
		line << paid;
		boxField("Paid          : ", line.str());
	}
	{
		ostringstream line;
		line << unpaid;
		boxField("Unpaid        : ", line.str());
	}
	{
		ostringstream line;
		line << cancelled;
		boxField("Cancelled     : ", line.str());
	}
	{
		ostringstream line;
		line << fixed << setprecision(2) << "RM " << roundMoney(revenue);
		boxField("Paid revenue  : ", line.str());
	}
	boxLine();

	vector<string> states = destinationStates();
	if (!states.empty()) {
		boxRow("Bookings by state (active, not cancelled)");
		for (size_t s = 0; s < states.size(); s++) {
			int count = 0;
			for (size_t i = 0; i < reservations.size(); i++) {
				if (reservations[i].status == "Cancelled") {
					continue;
				}
				if (reservations[i].hotelState == states[s]) {
					count++;
				}
			}
			if (count > 0) {
				ostringstream line;
				line << count;
				boxField(states[s] + " : ", line.str());
			}
		}
		boxLine();
	}
	pauseEnter();
}

void staffViewAllBookings() {
	cout << endl;
	boxTitle("All bookings");
	if (reservations.empty()) {
		boxRow("No booking in this run yet.");
		boxLine();
		pauseEnter();
		return;
	}

	for (size_t i = 0; i < reservations.size(); i++) {
		ostringstream head;
		head << "#" << reservations[i].reservationID
			 << "  " << reservations[i].customerUsername
			 << "  Room " << reservations[i].roomNumber;
		boxRow(head.str());
		if (!reservations[i].hotelName.empty()) {
			boxWrapHang(string(4, ' ') + reservations[i].hotelName, 4);
		}
		boxWrapHang(string(4, ' ') + reservations[i].checkInDate
			+ "  " + reservations[i].paymentStatus
			+ "  " + reservations[i].status, 4);
		if (i + 1 < reservations.size()) {
			boxRow("");
		}
	}
	boxLine();
	pauseEnter();
}

void staffSearchCustomer() {
	string username;
	if (!askLine(" Username to search (or 0 to cancel): ", username)) {
		return;
	}
	int idx = findCustomerIndex(username);
	if (idx == -1) {
		cout << " Customer not found." << endl;
		return;
	}

	const Customer& c = customers[idx];
	int bookingCount = 0;
	int paidCount = 0;
	for (size_t i = 0; i < reservations.size(); i++) {
		if (reservations[i].customerUsername == c.username
			&& reservations[i].status != "Cancelled") {
			bookingCount++;
			if (reservations[i].paymentStatus == "Paid") {
				paidCount++;
			}
		}
	}

	cout << endl;
	boxTitle("Customer search");
	boxField("Username    : ", c.username);
	boxField("Full name   : ", c.fullName);
	boxField("Age         : ", c.age);
	boxField("Email       : ", c.email);
	boxField("Phone       : ", c.phoneNumber);
	boxField("Member      : ", c.membershipStatus);
	{
		ostringstream line;
		line << c.loyaltyPoints;
		boxField("Points      : ", line.str());
	}
	{
		ostringstream line;
		line << bookingCount << " active, " << paidCount << " paid";
		boxField("Bookings    : ", line.str());
	}
	boxLine();
	pauseEnter();
}

void staffTodayCheckIns() {
	int y = 0;
	int m = 0;
	int d = 0;
	int h = 0;
	int min = 0;
	malaysiaNow(y, m, d, h, min);
	string today = makeDate(d, m, y);

	cout << endl;
	boxTitle("Today's check-ins");
	boxField("Today : ", weekdayName(d, m, y) + " " + today);
	boxLine();

	int shown = 0;
	for (size_t i = 0; i < reservations.size(); i++) {
		if (reservations[i].status == "Cancelled") {
			continue;
		}
		if (reservations[i].checkInDate != today) {
			continue;
		}
		shown++;
		ostringstream head;
		head << "#" << reservations[i].reservationID
			 << "  " << reservations[i].customerUsername
			 << "  Room " << reservations[i].roomNumber;
		boxRow(head.str());
		if (!reservations[i].hotelName.empty()) {
			boxWrapHang(string(4, ' ') + reservations[i].hotelName, 4);
		}
		boxWrapHang(string(4, ' ') + reservations[i].paymentStatus
			+ "  " + to_string(reservations[i].guests) + " guest(s)", 4);
		boxRow("");
	}
	if (shown == 0) {
		boxRow("No check-in for today in this run.");
	}
	boxLine();
	pauseEnter();
}

bool roomHasActiveBooking(const string& roomNumber) {
	for (size_t i = 0; i < reservations.size(); i++) {
		if (reservations[i].roomNumber != roomNumber) {
			continue;
		}
		if (reservations[i].status == "Cancelled") {
			continue;
		}
		return true;
	}
	return false;
}

bool askLine(const string& prompt, string& out) {
	while (true) {
		cout << prompt;
		getline(cin, out);
		if (out == "0") {
			cout << " Cancelled." << endl;
			return false;
		}
		if (out.empty()) {
			cout << " Cannot be empty." << endl;
			continue;
		}
		return true;
	}
}

bool askMoney(const string& prompt, double& amount) {
	while (true) {
		cout << prompt;
		string text;
		getline(cin, text);
		if (text == "0") {
			cout << " Cancelled." << endl;
			return false;
		}

		bool ok = true;
		int dots = 0;
		if (text.empty()) {
			ok = false;
		}
		for (size_t i = 0; i < text.length(); i++) {
			if (text[i] == '.') {
				dots++;
			}
			else if (!isdigit(static_cast<unsigned char>(text[i]))) {
				ok = false;
			}
		}
		if (!ok || dots > 1) {
			cout << " Enter a number such as 150 or 150.50." << endl;
			continue;
		}

		amount = roundMoney(stod(text));
		if (amount <= 0) {
			cout << " Price must be more than 0." << endl;
			continue;
		}
		return true;
	}
}

string pickStateForStaff(bool allowNew) {
	vector<string> states = destinationStates();
	cout << endl;
	boxTitle("Which state?");
	for (size_t i = 0; i < states.size(); i++) {
		boxRow(optionText(static_cast<int>(i + 1)) + states[i]);
	}
	int extra = 0;
	if (allowNew) {
		extra = 1;
		boxRow(optionText(static_cast<int>(states.size() + 1)) + "Type a new state");
	}
	boxRow(optionText(0) + "Cancel");
	boxLine();
	int maxChoice = static_cast<int>(states.size()) + extra;
	if (states.empty() && !allowNew) {
		cout << " No hotel yet. Add a hotel first." << endl;
		return "";
	}
	if (maxChoice == 0) {
		string state;
		if (!askLine(" New state name (or 0 to cancel): ", state)) {
			return "";
		}
		return state;
	}
	cout << " Please choose 0-" << maxChoice << ": ";
	int choice = getIntInRange(0, maxChoice);
	if (choice == 0) {
		return "";
	}
	if (allowNew && choice == static_cast<int>(states.size()) + 1) {
		string state;
		if (!askLine(" New state name (or 0 to cancel): ", state)) {
			return "";
		}
		return state;
	}
	return states[static_cast<size_t>(choice - 1)];
}

string pickAreaForStaff(const string& state, bool allowNew) {
	vector<string> areas = areasOf(state);
	cout << endl;
	boxTitle("Which area in " + state + "?");
	for (size_t i = 0; i < areas.size(); i++) {
		boxRow(optionText(static_cast<int>(i + 1)) + areas[i]);
	}
	int extra = 0;
	if (allowNew) {
		extra = 1;
		boxRow(optionText(static_cast<int>(areas.size() + 1)) + "Type a new area");
	}
	boxRow(optionText(0) + "Cancel");
	boxLine();
	int maxChoice = static_cast<int>(areas.size()) + extra;
	if (areas.empty() && !allowNew) {
		cout << " No area in this state yet." << endl;
		return "";
	}
	if (maxChoice == 0) {
		string area;
		if (!askLine(" New area name (or 0 to cancel): ", area)) {
			return "";
		}
		return area;
	}
	cout << " Please choose 0-" << maxChoice << ": ";
	int choice = getIntInRange(0, maxChoice);
	if (choice == 0) {
		return "";
	}
	if (allowNew && choice == static_cast<int>(areas.size()) + 1) {
		string area;
		if (!askLine(" New area name (or 0 to cancel): ", area)) {
			return "";
		}
		return area;
	}
	return areas[static_cast<size_t>(choice - 1)];
}

int pickHotelIndexForStaff(const string& state, const string& area) {
	vector<int> list = hotelsOf(state, area);
	if (list.empty()) {
		cout << " No hotel in this area." << endl;
		return -1;
	}
	cout << endl;
	boxTitle("Choose a hotel");
	for (size_t i = 0; i < list.size(); i++) {
		int idx = list[i];
		boxRow(optionText(static_cast<int>(i + 1)) + hotelBranches[static_cast<size_t>(idx)].name);
		boxWrapHang(string(4, ' ') + hotelBranches[static_cast<size_t>(idx)].address, 4);
		if (i + 1 < list.size()) {
			boxRow("");
		}
	}
	boxRow(optionText(0) + "Cancel");
	boxLine();
	cout << " Please choose 0-" << list.size() << ": ";
	int pick = getIntInRange(0, static_cast<int>(list.size()));
	if (pick == 0) {
		return -1;
	}
	return list[static_cast<size_t>(pick - 1)];
}

string pickRoomType() {
	cout << endl;
	boxTitle("Room type");
	boxRow(optionText(1) + "Single");
	boxRow(optionText(2) + "Twin");
	boxRow(optionText(3) + "Deluxe");
	boxRow(optionText(4) + "Family");
	boxRow(optionText(5) + "Suite");
	boxRow(optionText(6) + "Presidential");
	boxRow(optionText(7) + "Type another name");
	boxRow(optionText(0) + "Cancel");
	boxLine();
	cout << " Please choose 0-7: ";
	int choice = getIntInRange(0, 7);
	if (choice == 0) {
		return "";
	}
	if (choice == 1) {
		return "Single";
	}
	if (choice == 2) {
		return "Twin";
	}
	if (choice == 3) {
		return "Deluxe";
	}
	if (choice == 4) {
		return "Family";
	}
	if (choice == 5) {
		return "Suite";
	}
	if (choice == 6) {
		return "Presidential";
	}
	string type;
	if (!askLine(" Type name (or 0 to cancel): ", type)) {
		return "";
	}
	return type;
}

string pickRoomStatus() {
	cout << endl;
	boxTitle("Room status");
	boxRow(optionText(1) + "Available");
	boxRow(optionText(2) + "Occupied");
	boxRow(optionText(3) + "Cleaning");
	boxRow(optionText(4) + "Maintenance");
	boxRow(optionText(0) + "Cancel");
	boxLine();
	cout << " Please choose 0-4: ";
	int choice = getIntInRange(0, 4);
	if (choice == 0) {
		return "";
	}
	if (choice == 1) {
		return "Available";
	}
	if (choice == 2) {
		return "Occupied";
	}
	if (choice == 3) {
		return "Cleaning";
	}
	return "Maintenance";
}
