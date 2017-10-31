#include <iostream> // std::cout, std::cin, std::getline
#include <string> // std::string
#include <math.h> // fmod (), pow ()
#include <cmath> // abs
#include <algorithm> // string::erase (), string::remove ()
#include <sstream>
#include <vector>
#include <iomanip>
#include <utility> // std::pair, std::make_pair

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
		if (input.find_first_of('e') != string::npos) {
			double number = stod (input.substr(0, input.find_first_of('e')));

			return getsigamount ((int)number, number-(int)number, input.substr(0, input.find_first_of('e')));
		}

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

int getdecimalplaces (double number) {
	int places = 0;
	
	double num = abs(number);
	num = num - (int)num;

	while (num != 0) {
		num *= 10;
		places ++;
		num = num - (int)num;
	}

	return places;
}

string setsigamount (int whn, double dci, string inpt, int sigamount) {
	int whole_number = whn;
	double decimal = dci;
	double number = whole_number + decimal;
	string input = inpt;
	stringstream ss;
    ss << input;

	if (input.find_first_of('e') != string::npos) {
		double number = stod(input);
		whole_number = (int)number;
		decimal = number - whole_number;
		ss.str("");
		ss << stod(input);
		input = ss.str();
		//number = stod(input);
	}

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
            ss << fixed << setprecision (sigamount - getdigits(whole_number)) << number << endl;			
		} else {
			ss << fixed << setprecision (sigamount) << number << endl;
		}
	} else if (currentsigamount  > sigamount) {
        ss.str("");
        
        int sign = int( number / abs(number));
        stringstream newNumber;

        // decrease sigfigs

        if (input.find('.') == string::npos) {
            //newNumber << ((int)abs(number));

            double num = int((abs(number) + pow(10, getdigits((int)number) - sigamount) / 2)/ pow(10, getdigits((int)number) - sigamount)) * pow(10, getdigits((int)number) - sigamount);
            //cout << to_string ((int)num) << endl;

            if (getsigamount ((int)num, 0, to_string((int)num)) < sigamount) {
            	ss << setsigamount (num, 0, to_string((int)num), sigamount);
            } else {
            	ss << num << endl;
            }
        } else {
        	int decimal_precision = sigamount - getsigamount(whole_number, 0, to_string(whole_number) + ".");

        	if (getsigamount(whole_number, 0, to_string(whole_number) + ".") == sigamount) {

        		if (getsigamount(whole_number, 0, to_string(whole_number)) != sigamount) {
        			ss << (int)round(number) << '.' << endl;
				} else {
					ss << (int)round(number) << endl;
				}
        		
        	} else if (getsigamount(whole_number, 0, to_string(whole_number) + ".") > sigamount) {
        		ss << setsigamount (whole_number, 0, to_string((int)round(number)), sigamount);
        	} else if (decimal_precision >= 0) {
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
// TODO: Proper sigfigs for addition and subtraction 

const char * expressionToParse = "6.5-2.5*10/5+2*5";

istringstream parse(expressionToParse) ;

size_t strposition, prevpos = 0;

char peek() {
    return static_cast<char>(parse.peek());
}

char get() {
    
    return static_cast<char>(parse.get());
}

pair<string, double> expression();

pair<string, double> number()
{
    double result ;
    
    strposition = parse.str().find_first_of("+-/*()", prevpos);

    string resultstr = parse.str().substr(prevpos, strposition-prevpos);
   

    if (strposition == parse.str().length()-1) {
        prevpos = string::npos;
    } else {
        prevpos = strposition+1;
    }
    
    parse >> result ; 
    cout << "NUMBER: " << resultstr << endl;

    return make_pair(resultstr, result);
}

pair<string, double> factor()
{
    if ((peek() >= '0' && peek() <= '9') || peek() == '.') {
        return number();
    }
    else if (peek() == '(')
    {
        get(); // '('
        pair<string, double> result = expression();
        cout << "EXPRESSION IN FACTOR: " << result.first << endl;
        get(); // ')'
        return result;
    }
    else if (peek() == '-')
    {
        get();
        pair<string, double> fac = factor ();
        string retstring = get<string>(fac);
        if (get<string>(fac)[0] == '-') {
            retstring = get<string>(fac).substr (1, string::npos);
        } else {
            retstring = "-" + get<string>(fac);
        }

        return make_pair (retstring, get<double>(fac) * -1);
    }
    return make_pair ("ERROR", 0); // error
}

pair<string, double> term()
{
    pair<string, double> result = factor();
    pair<string, double> fac = result;
    int sigfigs = 0;
    while (peek() == '*' || peek() == '/') {

        if (get() == '*') {
            fac = factor();
            sigfigs = min (getsigamount((int)result.second, result.second - (int)result.second, result.first), getsigamount(fac.second, fac.second-(int)fac.second, fac.first));
            result.second *= fac.second;
            result.first = setsigamount((int)result.second, result.second-(int)result.second, to_string(result.second), sigfigs);
            //result = make_pair (setsigamount((int)result.second, result.second-(int)result.second, to_string(result.second), sigfigs), stod(setsigamount((int)result.second, result.second-(int)result.second, to_string(result.second), sigfigs)));
            cout << "RESULT STRING: " << result.first << endl;
        } else {
        	fac = factor();
            sigfigs = min (getsigamount((int)result.second, result.second-(int)result.second, result.first), getsigamount(fac.second, fac.second-(int)fac.second, fac.first));
            result.second /= fac.second;
            result.first = setsigamount((int)result.second, result.second-(int)result.second, to_string(result.second), sigfigs);
            //result = make_pair (setsigamount((int)result.second, result.second-(int)result.second, to_string(result.second), sigfigs), stod(setsigamount((int)result.second, result.second-(int)result.second, to_string(result.second), sigfigs)));
            cout << "RESULT STRING: " << result.first << endl;
        }
    }

    result = make_pair (setsigamount((int)result.second, result.second-(int)result.second, to_string(result.second), sigfigs), stod(setsigamount((int)result.second, result.second-(int)result.second, to_string(result.second), sigfigs)));
    
    cout << "FINAL TERM: " << result.first << endl;

    return result;
}

pair<string, double> expression()
{
    pair<string, double> result = term();
    cout << "TERM IN EXPRESSION: " << result.first << endl;
    while (peek() == '+' || peek() == '-') {
        if (get() == '+')
            result.second += term().second;
        else
            result.second -= term().second;
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
		cout << "with 1 sigfig: " << setsigamount(whole_number, decimal, split[i], 1) << endl;
	}

	expressionToParse = input_string.c_str();
	parse.str (input_string);

    pair <string, double> answer = expression();

	cout << "Calculated:\nSTRING: " << answer.first + "\nDECIMAL: " << answer.second << endl;
}
