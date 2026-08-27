#include "hotel.h"

int main() {
	enableColors();
	srand(static_cast<unsigned int>(time(0)));
	loadCustomersFromFile();

	while (true) {
		clearScreen();
		logo();
		displayUserTypeMenu();
		int choice = getValidatedInput(0, 2);

		switch (choice) {
			case 1:
				customerAuthMenu();
				break;
			case 2:
				staffLogin();
				break;
			case 0:
				cout << endl;
				showBanner();
				boxTitle("Goodbye");
				boxRow("Thank you for using Grand Horizon Hotel.");
				boxRow("Have a nice day!");
				boxLine();
				pauseEnter();
				return 0;
		}
	}
}