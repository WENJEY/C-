#include "../staff_internal.h"

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

	showPage("Reports and statistics");
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
