#include "../hotel.h"

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

bool askAvailabilityBrowseDate(int& day, int& month, int& year) {
	string error;

	while (true) {
		int nowY = 0;
		int nowM = 0;
		int nowD = 0;
		int nowH = 0;
		int nowMin = 0;
		malaysiaNow(nowY, nowM, nowD, nowH, nowMin);

		showPage("Check Availability");
		boxRow("Today Malaysia time: " + makeDate(nowD, nowM, nowY) + "  " + makeClockTime(nowH, nowMin));
		boxRow("Today is " + weekdayName(nowD, nowM, nowY));
		boxRow("Choose a date to see which rooms are free");
		boxRow("This is NOT your check-in date yet");
		boxRow("Enter day, then month, then year");
		boxRow("Enter 0 at any step to cancel");
		boxLine();

		if (!error.empty()) {
			cout << red << error << original << endl << endl;
		}

		cout << " Day   (1-31)      : ";
		int d = getIntInRange(0, 31);
		if (d == 0) {
			return false;
		}

		cout << " Month (1-12)      : ";
		int m = getIntInRange(0, 12);
		if (m == 0) {
			return false;
		}

		cout << " Year  (e.g. 2026) : ";
		int y = getIntInRange(0, 2100);
		if (y == 0) {
			return false;
		}

		if (!isValidDate(d, m, y)) {
			error = " That date does not exist. Please enter again.";
			continue;
		}
		if (dateCompare(d, m, y, nowD, nowM, nowY) < 0) {
			error = " Date cannot be before today.";
			continue;
		}

		day = d;
		month = m;
		year = y;
		return true;
	}
}

bool askCheckInDate(int& day, int& month, int& year) {
	string error;

	while (true) {
		int nowY = 0;
		int nowM = 0;
		int nowD = 0;
		int nowH = 0;
		int nowMin = 0;
		malaysiaNow(nowY, nowM, nowD, nowH, nowMin);

		showPage("Check-in Date");
		boxRow("Today Malaysia time: " + makeDate(nowD, nowM, nowY) + "  " + makeClockTime(nowH, nowMin));
		boxRow("Today is " + weekdayName(nowD, nowM, nowY));
		boxRow("Now choose your actual check-in date");
		boxRow("Enter day, then month, then year");
		boxRow("You can check in any time on that date");
		boxRow("Enter 0 at any step to cancel");
		boxLine();

		if (!error.empty()) {
			cout << red << error << original << endl << endl;
		}

		cout << " Day   (1-31)      : ";
		int d = getIntInRange(0, 31);
		if (d == 0) {
			return false;
		}

		cout << " Month (1-12)      : ";
		int m = getIntInRange(0, 12);
		if (m == 0) {
			return false;
		}

		cout << " Year  (e.g. 2026) : ";
		int y = getIntInRange(0, 2100);
		if (y == 0) {
			return false;
		}

		if (!isValidDate(d, m, y)) {
			error = " That date does not exist. Please enter again.";
			continue;
		}
		if (dateCompare(d, m, y, nowD, nowM, nowY) < 0) {
			error = " Check-in date cannot be before today.";
			continue;
		}

		day = d;
		month = m;
		year = y;
		return true;
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

int daysFromToday(int day, int month, int year) {
	int nowY = 0;
	int nowM = 0;
	int nowD = 0;
	int nowH = 0;
	int nowMin = 0;
	malaysiaNow(nowY, nowM, nowD, nowH, nowMin);

	int cmp = dateCompare(day, month, year, nowD, nowM, nowY);
	if (cmp == 0) {
		return 0;
	}
	if (cmp < 0) {
		return -1;
	}

	int d = nowD;
	int m = nowM;
	int y = nowY;
	int count = 0;
	while (dateCompare(d, m, y, day, month, year) < 0) {
		addDays(d, m, y, 1);
		count++;
		if (count > 5000) {
			break;
		}
	}
	return count;
}

bool datesOverlap(int inD1, int inM1, int inY1, int outD1, int outM1, int outY1,
	int inD2, int inM2, int inY2, int outD2, int outM2, int outY2) {
	return dateCompare(inD1, inM1, inY1, outD2, outM2, outY2) < 0
		&& dateCompare(inD2, inM2, inY2, outD1, outM1, outY1) < 0;
}

bool isStayActiveOnDate(int inD, int inM, int inY, int outD, int outM, int outY,
	int onD, int onM, int onY) {
	return dateCompare(onD, onM, onY, inD, inM, inY) >= 0
		&& dateCompare(onD, onM, onY, outD, outM, outY) < 0;
}