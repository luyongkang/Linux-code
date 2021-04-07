#include<stddef.h>
#include<iostream>
#include<sys/socket.h>
#include<sys/un.h>

using namespace std;

int main()
{
	int fd, size;
	sockaddr_un un;

	un.sun_family = AF_UNIX;
	strcpy(un.sun_path, "foo.socket");

	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(fd<0)
	{
		cerr << "socket err" << endl;
		exit(1);
	}

	size = offsetof(sockaddr_un, sun_path) + strlen(un.sun_path);

	cout << size << endl;
	cout << sizeof(un) << endl;
	if (bind(fd, (sockaddr *)&un, sizeof(un)) < 0)
	{
		cerr << "bind fail" << endl;
		exit(1);
	}
	cout << "UNIX domain socket bound" << endl;
	return 0;
}