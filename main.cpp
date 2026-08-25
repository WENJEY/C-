#include "hotel.h"

int main() {
	enableColors();
	srand(static_cast<unsigned int>(time(0)));
	loadCustomersFromFile();
	loadRoomsFromFile();
	loadReservationsFromFile();

	while (true) {
		logo();
		displayUserTypeMenu();
		int choice = getValidatedInput(1, 3);

		switch (choice) {
		case 1:
			customerAuthMenu();
			break;
		case 2:
			staffLogin();
			break;
		case 3:
			cout << endl;
			cout << " Thank you for using Grand Horizon Hotel!" << endl;
			return 0;
		}
	}
}
