#include <iostream>
#include <iomanip>
#include <windows.h>
#include <vector>
using namespace std ;

#define red "\033[91m"
#define original "\033[0m"
#define window_width 211

void enableColors() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD mode;
    GetConsoleMode(hConsole, &mode);
    SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}
int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

void logo() {
    string logo = R"(
  /$$$$$$                                 /$$       /$$   /$$                  /$$
 /$$__  $$                               | $$      | $$  | $$                 |__/
| $$  \__/ /$$$$$$ /$$$$$$ /$$$$$$$  /$$$$$$$      | $$  | $$ /$$$$$$  /$$$$$$ /$$/$$$$$$$$ /$$$$$$ /$$$$$$$
| $$ /$$$$/$$__  $|____  $| $$__  $$/$$__  $$      | $$$$$$$$/$$__  $$/$$__  $| $|____ /$$//$$__  $| $$__  $$
| $$|_  $| $$  \__//$$$$$$| $$  \ $| $$  | $$      | $$__  $| $$  \ $| $$  \__| $$  /$$$$/| $$  \ $| $$  \ $$
| $$  \ $| $$     /$$__  $| $$  | $| $$  | $$      | $$  | $| $$  | $| $$     | $$ /$$__/ | $$  | $| $$  | $$
|  $$$$$$| $$    |  $$$$$$| $$  | $|  $$$$$$$      | $$  | $|  $$$$$$| $$     | $$/$$$$$$$|  $$$$$$| $$  | $$
 \______/|__/     \_______|__/  |__/\_______/      |__/  |__/\______/|__/     |__|________/\______/|__/  |__/
)";
    string system = R"(
      _ ___ _       _   _  __  _  _          ___ ___  _          __     __ ___ _
 |_| / \ | |_ |    |_) |_ (_  |_ |_) \  / /\  |   |  / \ |\ |   (_ \_/ (_   | |_ |\/|
 | | \_/ | |_ |_   | \ |_ __) |_ | \  \/ /--\ |  _|_ \_/ | \|   __) |  __)  | |_ |  |
                                                                                      )";
    stringstream ss(logo) , s(system);
    string line;
    cout << setw(window_width) << setfill('=') << "=" << endl;
    cout << red;
    while (getline(ss, line)) {
        cout << setw(51) << setfill(' ') << " " <<  line << '\n';
    }
    cout << original;
    while (getline(s, line)) {
        cout << setw(60) << setfill(' ') << " " <<  line << '\n';
    }
    cout << setw(window_width) << setfill('=') << "=" << endl;

}

int main () {
    enableColors();
    logo();
    return 0;
}