#include "../staff_internal.h"

void staffSearchCustomer() {
    showPage("All customers");
    if (customers.empty()) {
        boxRow("No customer registered yet.");
        boxLine();
        pauseEnter();
        return;
    }

    for (size_t i = 0; i < customers.size(); i++) {
        const Customer& c = customers[i];
        int bookingCount = 0;
        int paidCount = 0;
        for (size_t j = 0; j < reservations.size(); j++) {
            if (reservations[j].customerUsername == c.username
                && reservations[j].status != "Cancelled") {
                bookingCount++;
                if (reservations[j].paymentStatus == "Paid") {
                    paidCount++;
                }
            }
        }
        ostringstream header;
        header << "Customer " << (i+1);
        boxRow(header.str());
        boxField("Username    : ", c.username);
        if (!c.fullName.empty() && c.fullName != "---") {
            boxField("Full Name   : ", c.fullName);
        }
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
        if (i + 1 < customers.size()) {
            boxRow("");
        }
    }
    boxLine();
    pauseEnter();
}
