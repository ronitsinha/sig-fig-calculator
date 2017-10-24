#include <iostream> // std::cout, std::cin, std::getline
#include <string> // std::string
#include <math.h> // fmod (), pow ()
#include <cmath> // abs
#include <algorithm> // string::erase (), string::remove ()
#include <sstream>
#include <vector>
#include <iomanip>

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
				sigfigs ++;
				continue;
			}

			if (whole_string.at (whole_string.length () - 1) - '0' != 0) {
				sigfigs ++;
				continue;
			}

			for (int j = i+1; j < whole_string.length(); j++) {
				if (whole_string.at(j)-'0' !=0) {
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
				total ++;
			}
		}

		sigfigs = total + decimal_string.length ();
		return sigfigs;
	}

	if (decimal_string.length() > 0) {
		// if there are only zeroes in the decimal (i.e. 1.0000), then all of those zeroes are sig figs.
		int numOfInts = 0;

		for (int i = 0; i < decimal_string.length(); i++) {
			if (decimal_string.at(i) == '.') {
				continue;
			}

			int num = decimal_string.at(i) - '0';

			if (num != 0) {
				numOfInts ++;
			}
		}

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

int getdigits (int num) {
    int cur_num = num;
    int digits = 0;

    if (num == 0) {
        return 0;
    }

    while (cur_num > 0) {
        digits ++;
        cur_num /= 10;
    }

    return digits;
}

string setsigamount (int whole_number, double decimal, string input, int sigamount) {
	double number = whole_number + decimal;
	stringstream ss;
    ss << input;

    int currentsigamount = getsigamount (whole_number, decimal, input);

	if (currentsigamount < sigamount) {
	    ss.str("");	
        // increase sigfigs
		if (input.find ('.') == string::npos) {
	        int hypotheticalsigamount = getsigamount (whole_number, decimal, input + ".");
    
            if (hypotheticalsigamount > sigamount) {
                // Scientific notation
                ss << fixed << setprecision (sigamount - 1) << number / pow (10.0, getdigits((int)number) - 1) << "e" << getdigits((int)number) - 1 << endl;
            } else if (hypotheticalsigamount == sigamount) {
                ss << number << "." << endl;
            } else {
                ss << fixed << setprecision (sigamount - hypotheticalsigamount) << number << endl;
            }	
		} else if (whole_number != 0) {
            ss << fixed << setprecision (sigamount - to_string(whole_number).length()) << number << endl;			
		} else {
			ss << fixed << setprecision (sigamount) << number << endl;
		}
	} else if (currentsigamount  > sigamount) {
        ss.str("");
        
        int sign = int( number / abs(number));
        string newNumber;

        // decrease sigfigs

        if (input.find('.') == string::npos) {
            newNumber = to_string ((int)abs(number));

            for (int i = newNumber.length()-1; i > 0; i--) {
                if (i != 0) {
                    if (newNumber[i] - 48 >= 5 && newNumber[i-1] != '9') {
                        newNumber[i-1] ++;
                    }
                }

                newNumber[i] = '0';

                if (getsigamount (stoi(newNumber), decimal, newNumber) == sigamount) {
                    break;
                } else if (getsigamount (stoi(newNumber), decimal, newNumber + ".") == sigamount) {
                    newNumber = newNumber + ".";
                    break;
                }
            }
            
            if (getsigamount (stoi(newNumber), decimal, newNumber) != sigamount) {
                ss << fixed << setprecision (sigamount - 1) << sign * stod(newNumber) / pow (10.0, getdigits(stoi(newNumber)) - 1) << "e" << getdigits(stoi(newNumber)) - 1 << endl;     
            } else {
                ss << newNumber << endl;
            }
        } else {
        	int decimal_precision = getsigamount (whole_number, decimal, input) - sigamount;

        	if (getsigamount(whole_number, 0, to_string(whole_number) + ".") == sigamount && getsigamount(whole_number, 0, to_string(whole_number)) != sigamount) {

        		ss << whole_number << '.' << endl;
        		
        	} else if (getsigamount(whole_number, 0, to_string(whole_number) + ".") > sigamount) {
        		ss << setsigamount (whole_number, 0, to_string(10 * int(whole_number/10)), sigamount);
        	} else {
        		ss << fixed << setprecision (decimal_precision) << number << endl;
        	}
        }
	}

	return ss.str();
}

vector<string> splitinput (string input) {
	vector<string> split_input;

    size_t prev = 0, pos;
    while ((pos = input.find_first_of("+-/*()", prev)) != string::npos)
    {
        if (pos > prev) {
            split_input.push_back(input.substr(prev, pos-prev));
        } 

        prev = pos+1;
    }
    if (prev < input.length()) {
        split_input.push_back(input.substr(prev, string::npos));
    }

    return split_input;
}

// EVALUATION
// TODO: Evaluate Scientific Notation

const char * expressionToParse = "6.5-2.5*10/5+2*5";

istringstream parse(expressionToParse) ;

char peek()
{
    return static_cast<char>(parse.peek()) ;
}

char get()
{
    return static_cast<char>(parse.get()) ;                    
}

double expression();

double number()
{
    double result ;
    parse >> result ;                     
    return result;
}

double factor()
{
    if ((peek() >= '0' && peek() <= '9') || peek() == '.')
        return number();
    else if (peek() == '(')
    {
        get(); // '('
        double result = expression();
        get(); // ')'
        return result;
    }
    else if (peek() == '-')
    {
        get();
        return -factor();
    }
    return 0; // error
}

double term()
{
    double result = factor();
    while (peek() == '*' || peek() == '/') {
        int sigfigs = min(getsigamount((int)result, result-(int)result, to_string(result)), getsigamount ((int)factor(), factor()-(int)factor(), to_string(factor())));
        if (get() == '*') {
            result *= factor();
        } else {
            result /= factor();
        }
        result = stod (setsigamount ((int)result, result-(int)result, to_string(result), sigfigs));
    }
    return result;
}

double expression()
{
    double result = term();
    while (peek() == '+' || peek() == '-') {
        double scale = 1 / pow (10, min(getsigamount(0, result - (int)result, to_string(result)), getsigamount(0, term() - (int)term(), to_string(term()))));
        if (get() == '+')
            result += term();           
        else
            result -= term();
        result = floor(result / scale + 0.5) * scale;
    }
    return result;
}

int main () {
	string input_string;

	cout << "Enter a number or expression." << endl;

	getline (cin, input_string);


	// Remove unneeded whitespaces ("9. 0 0 1" becomes "9.001")
	input_string.erase( remove( input_string.begin(), input_string.end(), ' ' ), input_string.end() );

	int whole_number;
	double decimal = 0;

	vector<string> split = splitinput (input_string);

	for (int i = 0; i < split.size (); i ++) {
		try {
			whole_number = stoi(split[i]);

			try {
				whole_number = (int)stod(split[i]);

				if ((double)whole_number != stod(split[i])) {
					if (whole_number != 0) {
							decimal = fmod (stod(split[i]), (double)whole_number);
					} else {
						decimal = stod(split[i]);
					}
				}
			} catch (invalid_argument) {
				whole_number = stoi (split[i]);
				decimal = 0;
			}
		} catch (invalid_argument) {
			continue;
		}

		cout << "number: " << split[i] << endl;
		cout << "current sigfigs: " << getsigamount (whole_number, decimal, split[i]) << endl;
		cout << "with 3 sigfigs: " << setsigamount(whole_number, decimal, split[i], 3) << endl;
	}

	expressionToParse = input_string.c_str();
	parse.str (input_string);

	cout << expression () << endl;
}
