#include "../staff_internal.h"

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