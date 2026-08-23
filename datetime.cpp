#include "datetime.h"
#include "ui.h"
#include "validation.h"

void malaysiaNow(int& year, int& month, int& day, int& hour, int& minute) {
	time_t now = time(0) + 8 * 3600;
	tm* p = gmtime(&now);
	year = p->tm_year + 1900;
	month = p->tm_mon + 1;
	day = p->tm_mday;
	hour = p->tm_hour;
	minute = p->tm_min;
}

string twoDigits(int value) {
	ostringstream out;
	out << setw(2) << setfill('0') << value;
	return out.str();
}

string makeDate(int day, int month, int year) {
	return twoDigits(day) + "/" + twoDigits(month) + "/" + to_string(year);
}

string makeClockTime(int hour, int minute) {
	return twoDigits(hour) + ":" + twoDigits(minute);
}

string weekdayName(int day, int month, int year) {
	static const string names[7] = {
		"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
	};
	static const int t[12] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
	int y = year;
	int m = month;
	if (m < 3) {
		y -= 1;
	}
	int w = (y + y / 4 - y / 100 + y / 400 + t[m - 1] + day) % 7;
	if (w < 0) {
		w += 7;
	}
	return names[w];
}

int wrapClock(int value, int minVal, int maxVal) {
	if (value < minVal) {
		return maxVal;
	}
	if (value > maxVal) {
		return minVal;
	}
	return value;
}

void drawCheckInClock(int hour, int minute, const string& nowStamp, const string& warn) {
	int prevH = wrapClock(hour - 1, 0, 23);
	int nextH = wrapClock(hour + 1, 0, 23);
	int prevM = wrapClock(minute - 1, 0, 59);
	int nextM = wrapClock(minute + 1, 0, 59);
	boxTitle("Check-in Time");
	boxRow("Now in Malaysia: " + nowStamp);
	boxRow("");
	boxCenter("Hour                    Minute");
	boxCenter(twoDigits(prevH) + "                      " + twoDigits(prevM));
	boxCenter("< " + twoDigits(hour) + " >                < " + twoDigits(minute) + " >");
	boxCenter(twoDigits(nextH) + "                      " + twoDigits(nextM));
	if (warn.empty()) {
		boxRow("");
	}
	else {
		boxRow(warn);
	}
	boxLine();
}

void pickCheckInClock(int& hour, int& minute, bool sameDay) {
	while (true) {
		int y = 0;
		int m = 0;
		int d = 0;
		int h = 0;
		int nowMin = 0;
		malaysiaNow(y, m, d, h, nowMin);
		string nowStamp = makeDate(d, m, y) + "  " + makeClockTime(h, nowMin);

		cout << endl;
		drawCheckInClock(hour, minute, nowStamp, "");
		cout << " Hour (0-23): ";
		hour = getIntInRange(0, 23);

		malaysiaNow(y, m, d, h, nowMin);
		nowStamp = makeDate(d, m, y) + "  " + makeClockTime(h, nowMin);
		cout << endl;
		drawCheckInClock(hour, minute, nowStamp, "");
		cout << " Minute (0-59): ";
		minute = getIntInRange(0, 59);

		malaysiaNow(y, m, d, h, nowMin);
		nowStamp = makeDate(d, m, y) + "  " + makeClockTime(h, nowMin);
		if (sameDay && hour * 60 + minute < h * 60 + nowMin) {
			cout << endl;
			drawCheckInClock(hour, minute, nowStamp, "Time passed. Choose a later time.");
			continue;
		}

		cout << endl;
		drawCheckInClock(hour, minute, nowStamp, "");
		break;
	}
}

int daysInMonth(int month, int year) {
	int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (month == 2) {
		if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0) {
			return 29;
		}
	}
	if (month < 1 || month > 12) {
		return 0;
	}
	return days[month - 1];
}

bool isValidDate(int day, int month, int year) {
	if (year < 2026 || year > 2100) {
		return false;
	}
	if (month < 1 || month > 12) {
		return false;
	}
	if (day < 1 || day > daysInMonth(month, year)) {
		return false;
	}
	return true;
}

bool parseDate(const string& text, int& day, int& month, int& year) {
	int d = 0;
	int m = 0;
	int y = 0;
	char slash1 = 0;
	char slash2 = 0;
	istringstream in(text);
	in >> d >> slash1 >> m >> slash2 >> y;
	if (slash1 != '/' || slash2 != '/' || y < 1000) {
		return false;
	}
	if (!isValidDate(d, m, y)) {
		return false;
	}
	day = d;
	month = m;
	year = y;
	return true;
}

int dateCompare(int d1, int m1, int y1, int d2, int m2, int y2) {
	if (y1 != y2) {
		return (y1 < y2) ? -1 : 1;
	}
	if (m1 != m2) {
		return (m1 < m2) ? -1 : 1;
	}
	if (d1 != d2) {
		return (d1 < d2) ? -1 : 1;
	}
	return 0;
}

void addDays(int& day, int& month, int& year, int extraDays) {
	day += extraDays;
	while (day > daysInMonth(month, year)) {
		day -= daysInMonth(month, year);
		month++;
		if (month > 12) {
			month = 1;
			year++;
		}
	}
}
