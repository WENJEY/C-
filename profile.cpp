#include "hotel.h"

void viewMyProfile() {
	int idx = findCustomerIndex(currentLoggedInCustomer);
	if (idx == -1) {
		cout << " Profile not found." << endl;
		return;
	}

	Customer c = customers[idx];
	int nextNeed = 0;
	string nextTier = "Silver";
	if (c.membershipStatus == "Regular") {
		nextNeed = SILVER_POINTS - c.loyaltyPoints;
		nextTier = "Silver";
	}
	else if (c.membershipStatus == "Silver") {
		nextNeed = GOLD_POINTS - c.loyaltyPoints;
		nextTier = "Gold";
	}

	cout << endl;
	boxTitle("My Profile");
	boxRow("Username    : " + c.username);
	boxRow("Full name   : " + c.fullName);
	boxRow("Age         : " + c.age);
	boxRow("Email       : " + c.email);
	boxRow("Phone       : " + c.phoneNumber);
	boxRow("IC/Passport : " + c.icPassport);
	boxRow("Member      : " + c.membershipStatus);
	{
		ostringstream line;
		line << "Points      : " << c.loyaltyPoints;
		boxRow(line.str());
	}
	if (c.membershipStatus != "Gold") {
		ostringstream line;
		line << "Next tier   : " << nextNeed << " more point(s) to " << nextTier;
		boxRow(line.str());
	}
	else {
		boxRow("Next tier   : You are at the highest tier!");
	}
	boxRow("Perks       : Silver 5% off, Gold 10% off");
	boxLine();
}

