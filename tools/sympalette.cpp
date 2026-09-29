#include <iostream>
using namespace std;

int count_digits(int n) {
    if (n == 0) return 1;
    int count = 0;
    while (n != 0) {
        n /= 10;
        ++count;
    }
    return count;
}

int main() {
	int from, to, width;
	char w_mode = 'N';
	cout << "Enable wide (wchar_t) mode? [y/n]: ";
	cin >> w_mode;
	cout << endl;
	cout << "Enter row WIDTH (in symbols): ";
	cin >> width;
	cout << endl;
	cout << "Enter FROM and TO (inclusive) range: ";
	cin >> from >> to;
	cout << endl;
	int col_width = count_digits(to) + 1;
	for (int i = 0; i < col_width; i++)
		cout << ' ';
	for (int i = 0; i < width; i++)
		cout << i % 10;
	cout << endl;
	for (int i = 0; i < width + col_width; i++)
		cout << '_';
	cout << endl;
	if (w_mode == 'y' || w_mode == 'Y') {
		for (int i = from; i <= to;) {
			int dig_len = count_digits(i);
			for (int j = 0; j < col_width - dig_len - 1; j++)
				wcout << L' ';
			wcout << i << L'|';
			for (int w = 0; w < width && i <= to; w++)
				wcout << (wchar_t)i++;
			wcout << endl;
		}
		wcout << endl;
	}
	else if (w_mode == 'n' || w_mode == 'N') {
		for (int i = from; i <= to;) {
			int dig_len = count_digits(i);
			for (int j = 0; j < col_width - dig_len - 1; j++)
				cout << ' ';
			cout << i << '|';
			for (int w = 0; w < width && i <= to; w++)
				cout << (char)i++;
			cout << endl;
		}
		cout << endl;
	}
	else {
		cout << "invalid input, aborting" << endl;
		return 1;
	}
	return 0;
}
