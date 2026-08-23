#ifndef PAYMENT_H
#define PAYMENT_H

#include "common.h"

void applyPromoCode();
void redeemLoyaltyPoints();
void showSessionBill(bool showPayHint);
void processPayment();
void printAndSaveInvoice(const BillBreakdown& bill, const string& method);
double addOnTotal(const BookingRecord& booking);
BillBreakdown calculateSessionBill();

#endif
