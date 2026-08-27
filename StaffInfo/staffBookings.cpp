#include "../staff_internal.h"

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