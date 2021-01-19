#include<iostream>

using namespace std;

union A
{
	int i;
	char c;
};

int main(int argc, char *argv[])
{
	A test;
	test.i = 0x12345678;
	test.c = 0x12;

	if(test.i==0x12345678)
	{
		cout << "it's big endian" << endl;
	}
	else
	{
		cout << "it's small endian" << endl;
	}

	return 0;
};