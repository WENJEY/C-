#include "../staff_internal.h"

void staffSearchCustomer() {
    showPage("Customer search");
    boxRow("Enter a username to look up");
    boxLine();

    string username;
    if (!askLine(" Username to search (or 0 to cancel): ", username)) {
        return;
    }
    int idx = findCustomerIndex(username);
    if (idx == -1) {
        cout << red << " Customer not found." << original << endl;
        pauseEnter();
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

    showPage("Customer search");
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
