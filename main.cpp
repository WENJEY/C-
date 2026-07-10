#include <iostream>
#include <iomanip>
#include <windows.h>
#include <vector>
#include <string>
#include <sstream>
using namespace std ;

#define red "\033[91m"
#define original "\033[0m"
#define window_width 211

void enableColors() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD mode;
    if (GetConsoleMode(hConsole, &mode)) {
        SetConsoleMode(hConsole, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
}

void MainPage() {
    cout << setfill(' ') << endl;
    cout << "Welcome to Grand Horizon Hotel" << endl;
    cout << setw(30) << setfill('=') << '=' << endl;
    cout << "Login" << endl;
    cout << setw(20) << setfill('=') << "=" << endl;
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
    MainPage();
    cout << "\n\nPress Enter to exit...";
    cin.get();
    return 0;
}