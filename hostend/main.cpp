#include<iostream>
#include<netdb.h>

using namespace std;
int main()
{
	hostent *phost;
	while((phost=gethostent())!=nullptr)
	{
		cout << phost->h_name << endl;
		cout << phost->h_addrtype << endl;
		cout << "this gethostent" << endl;
	}
	endhostent();
	netent *pnet;
	while((pnet=getnetent())!=nullptr)
	{
		cout << pnet->n_name << endl;
		cout << pnet->n_net << endl;
		cout << pnet->n_addrtype << endl;
		cout << "this getnetent" << endl;
	}

	endnetent();

	return 0;
}