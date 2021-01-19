#include<iostream>
#include<netdb.h>

using namespace std;

int main()
{
	protoent *p;
	while((p=getprotoent())!=nullptr)
	{
		cout << "p_name:" << p->p_name << endl;
		cout << "p_proto:" << p->p_proto << endl;
		cout << "--------------------------" << endl;
	}

	return 0;
}