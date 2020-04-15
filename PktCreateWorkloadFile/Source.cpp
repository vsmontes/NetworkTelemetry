

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main()
{
	stringstream myfile;

	int cols;
	int lines;
	long long data = 12345678987654321;

	cout << "cols: ";
	cin >> cols;
	cout << "lines: ";
	cin >> lines;

	for (int j = 0; j < cols; j++)
	{
		myfile << "HEADER_" << (j + 1) << ';';
	}
	myfile << endl;

	for (int i = 0; i < lines; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			myfile << data + (rand() % data) << ';';
		}
		myfile << endl;
	}

	return 0;
}