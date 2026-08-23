#ifndef BOOKING_H
#define BOOKING_H

void bookRoom();
bool createOneBooking();
void afterBookingMenu();
void offerAddOns();
void addOnsForReservation(int resIndex);
void specialRequestMenu();
void offerRoomUpgrade(int& roomIndex, int guests);
void maybeGiveWelcomeGift(int resIndex);

#endif
