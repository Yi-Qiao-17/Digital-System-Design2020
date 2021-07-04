#include<iostream>
#include<string>
#include<cmath>
using namespace std;
int r = 20;
void recursive_mode0(int value);
void recursive_mode1(string value);

int main() {
	int mode, intput_value;
	string input_value_string;
	cout <<"My Student ID is 1081420."<<endl<< "r is 20." << endl;
	cout << "Mode 0: convert a decimal number to a number in base 20. " << endl << "Mode 1: convert a number in base 20 to a decimal number." << endl;
	cout << "Which mode? " << endl;
	while (cin >> mode) {
		if (mode == 0 || mode == 1);
		else {
			cout << "The mode_input is error please enter agin." << endl;
			continue;
		}
		cout << "Value? " << endl;
		if (mode == 0) {
			cin >> intput_value;//THE CHECK : MODE 0 INPUT IS INT
			recursive_mode0(intput_value);//calculate the converted value
		}
		else {
			cin >> input_value_string;//THE CHECK : MODE 1 INPUT IS STRING
			recursive_mode1(input_value_string);
		}
		cout <<endl <<"Mode 0: convert a decimal number to a number in base 20. " << endl << "Mode 1: convert a number in base 20 to a decimal number." << endl;
		cout << "Which mode? " << endl;
	}
}

void recursive_mode0(int value) {
	if (value == 0) cout<<"0";
	else if(value>=r) {
		int number = value / r;
		recursive_mode0(number);
		int num = value % r;
		char c;
		if (num >= 10) {
			c = 'A' + num - 10;
			cout << c;
		}
		else cout << num;
	}
	else {
		cout << "Result : ";
		char c;
		if (value >= 10) {
			c = 'A' + value - 10;
			cout << c;
		}
		else cout << value;
	}
}
void recursive_mode1(string value) {
	int output = 0, i = 0;
	for (i = value.size() - 1; i >= 0; i--) {
		char c = value[i];
		int number;
		//string-to-int
		if (c >= '0' && c <= '9')number = c - '0';
		else {
			int j = 0;
			while (true) {
				if (c == 'A' + j)break;
				j++;
			}
			number = 10 + j;
		}
		output += (number * pow(r, value.size()-1-i));
	}
	cout << "Result : " << output;
}