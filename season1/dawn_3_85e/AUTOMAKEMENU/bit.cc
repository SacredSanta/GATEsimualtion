#include <iostream.h>

int main(void)
{
	int i = 0, a;
	cout << i << endl;

	i = i | 0x1;
	cout << i << endl;

	i = i | 0x2;
	cout << i << endl;

//	i = i | 0x4;
	cout << i << endl;

	i = i | 0x8;
	cout << i << endl;

	i = i | 0x10;
	cout << i << endl;

	if ( i & 0x1) {
		cout << " on " << endl;
	} else {
		cout << " off " << endl;
	}
	
	a = i & 0x1;
	cout << a  << endl;
	a = i & 0x2;
	cout << a << endl;
	a = i & 0x4;
	cout << a << endl;
	a = i & 0x8;
	cout << a << endl;
	a = i & 0x10;
	cout << a << endl;
	return 1;
}
