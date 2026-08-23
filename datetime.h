#ifndef DATETIME_H
#define DATETIME_H

#include "common.h"

void malaysiaNow(int& year, int& month, int& day, int& hour, int& minute);
string twoDigits(int value);
string makeDate(int day, int month, int year);
string makeClockTime(int hour, int minute);
string weekdayName(int day, int month, int year);
int wrapClock(int value, int minVal, int maxVal);
void drawCheckInClock(int hour, int minute, const string& nowStamp, const string& warn);
void pickCheckInClock(int& hour, int& minute, bool sameDay);
int daysInMonth(int month, int year);
bool isValidDate(int day, int month, int year);
bool parseDate(const string& text, int& day, int& month, int& year);
int dateCompare(int d1, int m1, int y1, int d2, int m2, int y2);
void addDays(int& day, int& month, int& year, int extraDays);

#endif
