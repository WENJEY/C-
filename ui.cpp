#include "ui.h"
#ifdef _WIN32
#include <windows.h>
#endif

void enableColors() {
#ifdef _WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD mode;
	if (GetConsoleMode(hConsole, &mode)) {
		SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
	}
#endif
}

void logo() {
	string logoArt = R"(
  /$$$$$$                                 /$$      /$$   /$$                  /$$
 /$$__  $$                               | $$     | $$  | $$                 |__/
| $$  \__/ /$$$$$$ /$$$$$$ /$$$$$$$  /$$$$$$$     | $$  | $$ /$$$$$$  /$$$$$$ /$$/$$$$$$$$ /$$$$$$ /$$$$$$$
| $$ /$$$$/$$__  $|____  $| $$__  $$/$$__  $$     | $$$$$$$$/$$__  $$/$$__  $| $|____ /$$//$$__  $| $$__  $$
| $$|_  $| $$  \__//$$$$$$| $$  \ $| $$  | $$     | $$__  $| $$  \ $| $$  \__| $$   /$$/ | $$  \ $| $$  \ $$
| $$  \ $| $$     /$$__  $| $$  | $| $$  | $$     | $$  | $| $$  | $| $$     | $$ /$$/   | $$  | $$ $$  | $$
|  $$$$$$| $$    |  $$$$$$| $$  | $|  $$$$$$$     | $$  | $|  $$$$$$| $$     | $$/$$$$$$$|  $$$$$$| $$  | $$
 \______/|__/     \_______|__/  |__/\_______/     |__/  |__/\______/|__/     |__|________/\______/|__/  |__/
)";
	string systemArt = R"(
      _ ___ _       _   _  __  _  _          ___ ___  _          __     __ ___ _
 |_| / \ | |_ |    |_) |_ (_  |_ |_) \  / /\  |   |  / \ |\ |   (_ \_/ (_   | |_ |\/|
 | | \_/ | |_ |_   | \ |_ __) |_ | \  \/ /--\ |  _|_ \_/ | \|   __) |  __)  | |_ |  |
                                                                                      )";
	stringstream ss(logoArt), s(systemArt);
	string line;

	cout << string(22, '=') << setw(window_width) << setfill('=') << "=" << string(22, '=') << endl;
	cout << red;
	while (getline(ss, line)) {
		cout << string(22, ' ') << setw(2) << setfill(' ') << " " << line << '\n';
	}
	cout << original;
	while (getline(s, line)) {
		cout << string(25, ' ') << setw(10) << setfill(' ') << " " << line << '\n';
	}
	cout << string(22, '=') << setw(window_width) << setfill('=') << "=" << string(22, '=') << endl;
}

void boxLine() {
	cout << setfill(' ') << " +" << string(BOX_W, '-') << "+" << endl;
}

void boxTitle(const string& title) {
	string t = " " + title + " ";
	int remain = BOX_W - static_cast<int>(t.length());
	if (remain < 0) {
		remain = 0;
		t = t.substr(0, BOX_W);
	}
	int leftDash = remain / 2;
	int rightDash = remain - leftDash;
	cout << setfill(' ') << " +" << string(leftDash, '-') << t << string(rightDash, '-') << "+" << endl;
}

void boxRow(const string& text) {
	string t = " " + text;
	if (static_cast<int>(t.length()) > BOX_W) {
		t = t.substr(0, BOX_W);
	}
	cout << setfill(' ') << " |" << left << setw(BOX_W) << t << "|" << endl;
}

void boxLineEq() {
	cout << setfill(' ') << " +" << string(BOX_W, '=') << "+" << endl;
}

void boxCenter(const string& text) {
	string t = text;
	if (static_cast<int>(t.length()) > BOX_W) {
		t = t.substr(0, BOX_W);
	}
	int pad = BOX_W - static_cast<int>(t.length());
	int leftPad = pad / 2;
	int rightPad = pad - leftPad;
	cout << " |" << string(leftPad, ' ') << t << string(rightPad, ' ') << "|" << endl;
}

void boxInner(const string& leftText, const string& rightText) {
	string inner(BOX_W, ' ');
	string left = leftText;
	string right = rightText;
	for (size_t i = 0; i < left.length() && i < static_cast<size_t>(BOX_W); i++) {
		inner[i] = left[i];
	}
	int start = BOX_W - static_cast<int>(right.length());
	if (start < 0) {
		start = 0;
	}
	for (size_t i = 0; i < right.length() && start + static_cast<int>(i) < BOX_W; i++) {
		inner[start + i] = right[i];
	}
	cout << " |" << inner << "|" << endl;
}

void boxSplitLine() {
	cout << setfill(' ') << " +" << string(COL_L, '-') << "+" << string(COL_R, '-') << "+" << endl;
}

void boxSplitRow(const string& leftText, const string& rightText) {
	string left = " " + leftText;
	string right = " " + rightText;
	if (static_cast<int>(left.length()) > COL_L) {
		left = left.substr(0, COL_L);
	}
	if (static_cast<int>(right.length()) > COL_R) {
		right = right.substr(0, COL_R);
	}
	left += string(COL_L - static_cast<int>(left.length()), ' ');
	right += string(COL_R - static_cast<int>(right.length()), ' ');
	cout << " |" << left << "|" << right << "|" << endl;
}

void displayUserTypeMenu() {
	cout << setfill(' ') << endl;
	cout << " Welcome to Grand Horizon Hotel" << endl;
	cout << " " << setw(30) << setfill('=') << '=' << endl;
	cout << "\n +---------------- Hotel Reservation System ----------------+" << endl;
	cout << " | 1. Customer                                              |" << endl;
	cout << " | 2. Staff                                                 |" << endl;
	cout << " | 3. Exit System                                           |" << endl;
	cout << " +----------------------------------------------------------+" << endl;
	cout << " Please select user type (1-3): ";
}
