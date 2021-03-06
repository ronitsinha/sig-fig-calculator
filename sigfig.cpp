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
	
	if (input.find('C') != string::npos || input.find('c') != string::npos) {
		// Will be recognized as code for infinite sigfigs.
		return -42;
	}

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

		string decimalsub = decimal_string.substr(decimal_string.find_first_of('.')+1, string::npos);

		if (decimalsub.length() > 0) {
			if (decimalsub.at(0)-'0' != 0) {
				sigfigs = decimalsub.length();
				return sigfigs;
			}
		} else {
			return 0;
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

int getdecimalplaces (string numberString) {
    int decimalplaces = 0;

    if (numberString.find_first_of('.') != string::npos && numberString.find_first_of('.') != numberString.length()-1) {
        decimalplaces = numberString.substr (numberString.find_first_of('.')+1, string::npos).length();
    }

    return decimalplaces;
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
                ss << fixed << setprecision (sigamount - 1) << number / pow (10.0, getdigits((int)number) - 1) << "e" << getdigits((int)number) - 1;
            } else if (hypotheticalsigamount == sigamount) {
                ss << number << ".";
            } else {
                ss << fixed << setprecision (sigamount - hypotheticalsigamount) << number;
            }	
		} else if (whole_number != 0) {
            ss << fixed << setprecision (sigamount - getdigits(whole_number)) << number;			
		} else {
			ss << fixed << setprecision (sigamount) << number;
		}
	} else if (currentsigamount  > sigamount) {
        ss.str("");
        
        int sign = int( number / abs(number));
        stringstream newNumber;

        // decrease sigfigs

        if (input.find('.') == string::npos) {

            double num = int((abs(number) + pow(10, getdigits((int)number) - sigamount) / 2)/ pow(10, getdigits((int)number) - sigamount)) * pow(10, getdigits((int)number) - sigamount);

            if (getsigamount ((int)num, 0, to_string((int)num)) < sigamount) {
            	ss << setsigamount (num, 0, to_string((int)num), sigamount);
            } else {
            	ss << num;
            }
        } else {
        	int decimal_precision = sigamount - getsigamount(whole_number, 0, to_string(whole_number) + ".");

        	if (getsigamount(whole_number, 0, to_string(whole_number) + ".") == sigamount) {

        		if (getsigamount(whole_number, 0, to_string(whole_number)) != sigamount) {
        			ss << (int)round(number) << '.';
				} else {
					ss << (int)round(number);
				}
        		
        	} else if (getsigamount(whole_number, 0, to_string(whole_number) + ".") > sigamount) {
        		ss << setsigamount (whole_number, 0, to_string((int)round(number)), sigamount);
        	} else if (decimal_precision >= 0) {
        		ss << fixed << setprecision (decimal_precision) << number;
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
// TODO: Constants with Scientific Notation
const char * expressionToParse = "6.5-2.5*10/5+2*5";

istringstream parse(expressionToParse) ;

size_t strposition, prevpos = 0;

char peek() {
    return static_cast<char>(parse.peek());
}

char get() {
    prevpos = strposition + 1;
    strposition = parse.str().find_first_of("+-/*()", prevpos);    
    return static_cast<char>(parse.get());
}

pair<string, double> expression();

pair<string, double> number()
{
    double result ;
    
    strposition = parse.str().find_first_of("+-/*()", prevpos);

    string resultstr = parse.str().substr(prevpos, strposition-prevpos);

    cout << "NUMBER: " << resultstr << endl;

    if (strposition == parse.str().length()-1) {
        prevpos = string::npos;
    } else {
        if (resultstr[resultstr.length()-1] == 'e') {
            strposition = parse.str().find_first_of("+-*/()", strposition+1);
            resultstr = parse.str().substr (prevpos, strposition-prevpos);
        }   
        prevpos = strposition + 1;
        cout << "PREVPOS: " << prevpos << endl;
    }
    
    parse >> result ;

    if (peek() == 'c' || peek() == 'C') {
        parse.get();
    }

    return make_pair(resultstr, result);
}

pair<string, double> factor()
{
    if ((peek() >= '0' && peek() <= '9') || peek() == '.' || peek() == 'C' || peek() == 'c') {
        return number();
    }
    else if (peek() == '(')
    {
        get(); // '('
        pair<string, double> result = expression();
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
    
    /* changeSigs
     * 0 = value is unchanged (no mathematical operation)
     * 1 = value is changed (operation between two nonexact values)
     * 2 = value is changed (operation between one or two exact values, meaning they have infinite sigfigs)
    */
    int changeSigs = 0;
    
    

    int sigfigs = getsigamount ((int)result.second, result.second-(int)result.second, result.first);
    
    while (peek() == '*' || peek() == '/') {

        if (get() == '*') {
            fac = factor(); 
            
            if (result.first.find('c') != string::npos || result.first.find('C') != string::npos) {
                if (fac.first.find('c') != string::npos || fac.first.find('C') != string::npos) {  

                    result.second *= fac.second;
                    result.first = to_string (result.second) + "C";

                } else {
                    sigfigs = getsigamount(fac.second, fac.second - (int)fac.second, fac.first);
                    result.second *= fac.second;
                    result.first = setsigamount ((int)result.second, result.second-(int)result.second, to_string(result.second), sigfigs);
                    changeSigs = 2;
                }   
            } else if (fac.first.find('c') != string::npos || fac.first.find('C') != string::npos) {
                sigfigs = getsigamount ((int)result.second, result.second-(int)result.second, result.first);
                result.second *= fac.second;
                result.first = setsigamount ((int)result.second, result.second-(int)result.second, to_string(result.second), sigfigs);
                changeSigs = 2;
            } else {
                sigfigs = min (getsigamount((int)result.second, result.second - (int)result.second, result.first), getsigamount(fac.second, fac.second-(int)fac.second, fac.first));
                result.second *= fac.second;
                result.first = setsigamount((int)result.second, result.second-(int)result.second, to_string(result.second), sigfigs); 
                changeSigs = 1;
            }   
        } else {
            fac = factor();
            
            if (result.first.find('c') != string::npos || result.first.find ('C') != string::npos) {
                if (fac.first.find ('c') != string::npos || fac.first.find('C') != string::npos) {
                    result.second /= fac.second;
                    result.first = to_string (result.second) + "C";
                } else {
                    sigfigs = getsigamount (fac.second, fac.second - (int)fac.second, fac.first);
                    result.second /= fac.second;
                    result.first = setsigamount ((int)result.second, result.second-(int)result.second, to_string(result.second), sigfigs);
                    changeSigs = 2;
                }
            } else if (fac.first.find('c') != string::npos || fac.first.find('C') != string::npos) {
                sigfigs = getsigamount ((int)result.second, result.second-(int)result.second, result.first);
                result.second /= fac.second;
                result.first = setsigamount ((int)result.second, result.second-(int)result.second, to_string(result.second), sigfigs);
                changeSigs = 2;               
            } else {
                sigfigs = min (getsigamount((int)result.second, result.second-(int)result.second, result.first), getsigamount(fac.second, fac.second-(int)fac.second, fac.first));
                result.second /= fac.second;
                result.first = setsigamount((int)result.second, result.second-(int)result.second, to_string(result.second), sigfigs);
                changeSigs = 1;
            } 
        }
    }

    return result;
}

// READ UP ON SIGFIGS WITH ADDITION/SUBTRACTION TO MAKE SURE OF THESE RULES (UNCERTAIN)

pair<string, double> expression()
{

    stringstream resString;
    pair<string, double> result = term();
    pair<string, double> trm = result;
    int decimalplaces = 0;
    while (peek() == '+' || peek() == '-') {
        resString.str("");
        if (get() == '+') {
            trm = term();
            
	    if (result.first.find('c') != string::npos || result.first.find('C') != string::npos) {
	    	if (trm.first.find('c') != string::npos || trm.first.find ('C') != string::npos) {
			result.second += trm.second;
			result.first = to_string (result.second) + "C";
		} else {
			decimalplaces = getdecimalplaces(trm.first.substr(0, trm.first.find('e')).substr(0, trm.first.find('E')));
			result.second += trm.second;
			resString << fixed << setprecision(decimalplaces) << result.second;
			result.first = resString.str();
			resString.str("");
			result.second = stod(result.first);
		}
	    } else if (trm.first.find('c') != string::npos || trm.first.find('C') != string::npos) {
	    	decimalplaces = getdecimalplaces(result.first.substr(0, result.first.find('e')).substr(0, result.first.find('E')));
		result.second += trm.second;
		resString << fixed << setprecision(decimalplaces) << result.second;
		result.first = resString.str();
		resString.str("");
		result.second = stod(result.first);
	    } else {
	    	decimalplaces = min(getdecimalplaces(trm.first.substr(0, trm.first.find('e')).substr(0, trm.first.find('E'))), getdecimalplaces(result.first.substr(0, result.first.find('e')).substr(0, result.first.find('E'))));
            	result.second += trm.second;
            	resString << fixed << setprecision (decimalplaces) << result.second;
            	result.first = resString.str();
            	result.second = stod(result.first);
	    }
        } else {
            trm = term();
            
	    if (result.first.find('c') != string::npos || result.first.find('C') != string::npos) {
	    	if (trm.first.find('c') != string::npos || trm.first.find ('C') != string::npos) {
			result.second -= trm.second;
			result.first = to_string (result.second) + "C";
		} else {
			decimalplaces = getdecimalplaces(trm.first.substr(0, trm.first.find('e')).substr(0, trm.first.find('E')));
			result.second -= trm.second;
			resString << fixed << setprecision(decimalplaces) << result.second;
			result.first = resString.str();
			resString.str("");
			result.second = stod(result.first);
		}
	    } else if (trm.first.find('c') != string::npos || trm.first.find('C') != string::npos) {
	        decimalplaces = getdecimalplaces(result.first.substr(0, result.first.find('e')).substr(0, result.first.find('E')));	
		result.second -= trm.second;
		resString << fixed << setprecision(decimalplaces) << result.second;
		result.first = resString.str();
		resString.str("");
		result.second = stod(result.first);
	    } else {
		decimalplaces = min(getdecimalplaces(trm.first.substr(0, trm.first.find('e')).substr(0, trm.first.find('E'))), getdecimalplaces(result.first.substr(0, result.first.find('e')).substr(0, result.first.find('E'))));
             	result.second -= trm.second;
            	resString << fixed << setprecision (decimalplaces) << result.second;
            	result.first = resString.str();
            	result.second = stod(result.first);
	    }
        }   
    }

    cout << "EXPRESSION RESULT: " << result.first << endl;

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

		if (getsigamount(whole_number,decimal,split[i]) == -42) {
			cout << "current sigfigs: infinite" << endl;	
		} else {
			cout << "current sigfigs: " << getsigamount (whole_number, decimal, split[i]) << endl;
		}

		// Remove C and c for sigfig setting (messes with it)
		split[i].erase (remove(split[i].begin(),split[i].end(), 'C'), split[i].end());
		split[i].erase (remove(split[i].begin(),split[i].end(), 'c'), split[i].end());

		cout << "with 1 sigfig: " << setsigamount(whole_number, decimal, split[i], 1) << endl;
	}

	expressionToParse = input_string.c_str();
	parse.str (input_string);

    pair <string, double> answer = expression();

	cout << "Calculated:\nSTRING: " << answer.first + "\nDECIMAL: " << answer.second << endl;
}
