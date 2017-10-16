#include <iostream> // std::cout, std::cin, std::getline
#include <string> // std::string
#include <math.h> // fmod ()
#include <algorithm> // string.erase (), string.remove ()
#include <sstream>
#include <vector>

using namespace std;

int getsigamount (int whole_number, double decimal, string input) {

	// TODO: use longs instead of ints to prevent number overflow. 
	if (whole_number >= 2147483648 || whole_number <= -2147483648) {
		cout << "number is too large! please use a number between -2147483648 and 2147483648" << endl;
		return -1;
	}

	string whole_string = to_string (whole_number);

	string decimal_string = input.substr (input.find ('.') + 1);

	int sigfigs = 0;

	// Get all sig figs from whole number
	if (input.find ('.') == string::npos) {
		for (int i = 0; i < whole_string.length(); i++) {
			if (!isdigit (whole_string.at(i))) {
				continue;
			}

			int num = whole_string.at(i) - '0';

			if (num != 0) {
				cout << whole_string.at(i) << " is " << (int)whole_string.at(i) << " - 48 = " << num << endl;
				sigfigs ++;
				continue;
			}

			if (whole_string.at (whole_string.length () - 1) - '0' != 0) {
				sigfigs ++;
				continue;
			}

			for (int j = i+1; j < whole_string.length(); j++) {
				if (whole_string.at(j)-'0' !=0) {
					cout << whole_string.at(j)-'0' << " at index " << j << endl;

					sigfigs ++;
					break;
				}
			}	
		}

		return sigfigs;

	} else if (whole_number != 0) {
		// MAKE SURE TO DOUBLE CHECK THIS RULE
		int total = 0;

		for (int i = 0; i < whole_string.length (); i++) {
			if (isdigit (whole_string.at (i))) {
				cout << whole_string.at (i) << endl;
				total ++;
			}
		}

		sigfigs = total + decimal_string.length ();
		return sigfigs;
	}

	if (decimal_string.length() > 0) {
		// if there are only zeroes in the decimal (i.e. 1.0000), then all of those zeroes are sig figs.
		int numOfInts = 0;
		cout << "decimal_string: " << decimal_string << endl;

		for (int i = 0; i < decimal_string.length(); i++) {
			if (decimal_string.at(i) == '.') {
				continue;
			}

			int num = decimal_string.at(i) - '0';

			if (num != 0) {
				cout << decimal_string.at(i) << " is " << (int)decimal_string.at(i) << " - 48 = " << num << endl;
				numOfInts ++;
			}
		}

		cout << "int num: " << numOfInts << endl;

		if (numOfInts == 0) {
			if (whole_number == 0) {
				// MAKE SURE TO DOUBLE CHECK THIS RULE

				// If a number is composed entirely of zeroes (i.e 0.000), it has zero sig figs.
				return 0;
			}

			sigfigs += decimal_string.length();
			return sigfigs;
		}

		for (int i = 0; i < decimal_string.length(); i++) {
			if (decimal_string.at(i) == '.') {
				continue;
			}

			int num = decimal_string.at(i) - '0';

			if (num == 0 && i == 0) {
				continue;
			}

			if (num != 0) {
				sigfigs ++;
				continue;
			}

			
			for (int j = i-1; j > 0; j--) {
				int back_num = decimal_string.at(j) - '0';
				
				if (back_num != 0) {
					sigfigs ++;
					break;
				}
			}
		}
	}

	return sigfigs;
}

int setsigamount (int whole_number, double decimal, string input, int sigamount) {
	double number = whole_number + decimal;
	stringstream ss;
    ss << number;

	if (getsigamount (whole_number, decimal, input) < sigamount) {
		// increase sigfigs
		if (input.find ('.') == string::npos) {
			
		} else {
			
		}
	} else if (getsigamount (whole_number, decimal, input) > sigamount) {
		// decrease sigfigs

	}

	return number;
}

int main () {
	string input_string;

	cout << "Enter a number or expression." << endl;

	getline (cin, input_string);


	// Remove unneeded whitespaces ("9. 0 0 1" becomes "9.001")
	input_string.erase( remove( input_string.begin(), input_string.end(), ' ' ), input_string.end() );

	// TODO: parse input for expressions (i.e. "9.0+10.001") to calcuate sig figs of individual numbers and perform computations
	// Look into the shunting-yard algorithm


	int whole_number;
	double decimal = 0;

	try {
		whole_number = (int)stod(input_string);

		if ((double)whole_number != stod(input_string)) {
			decimal = fmod (stod(input_string), (double)whole_number);
		}
	} catch (invalid_argument) {

		whole_number = stoi (input_string);
		decimal = 0;
	}

	cout << "sigfigs: " << getsigamount(whole_number, decimal, input_string) << endl;
}
