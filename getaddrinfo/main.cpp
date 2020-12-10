#include<iostream>
#include<arpa/inet.h>
#include<netdb.h>

#if defined(SOLARIS)
#include<netinet/in.h>
#endif

#if defined(BSD)
#include<sys/socket.h>
#include<netinet/in.h>
#endif

using namespace std;

void print_family(struct addrinfo* aip)
{
	cout << " family " << endl;
	switch(aip->ai_family)
	{
		case AF_INET:
			cout << "inet" << endl;
			break;
		case AF_INET6:
			cout << "inet6" << endl;
			break;
		case AF_UNIX:
			cout << "unix" << endl;
			break;
		case AF_UNSPEC:
			cout << "unspecified" << endl;
			break;
		default:
			cout << "unknow" << endl;
	}
}

void print_type(struct addrinfo *aip)
{
	cout << " type " << endl;
	switch (aip->ai_socktype)
	{
		case SOCK_STREAM:
			cout << "stream" << endl;
			break;
		case SOCK_DGRAM:
			cout << "datagram" << endl;
			break;
		case SOCK_SEQPACKET:
			cout << "seqpacket" << endl;
			break;
		case SOCK_RAW:
			cout << "raw" << endl;
			break;
		default:
			cout << "unknow " << aip->ai_socktype << endl;
	}
}

void print_protocol(struct addrinfo* aip)
{
	cout << " protocol " << endl;
	switch(aip->ai_protocol)
	{
		case 0:
			cout << "default" << endl;
			break;
		case IPPROTO_AH:
			cout << "TCP" << endl;
			break;
		case IPPROTO_UDP:
			cout << "UDP" << endl;
			break;
		case IPPROTO_RAW:
			cout << "raw" << endl;
			break;
		default:
			cout << "unknow " << aip->ai_protocol << endl;
	}
}

void print_flags(struct addrinfo* aip)
{
	cout << " flags " << endl;
	if(aip->ai_flags==0)
	{
		cout << 0 << endl;
	}
	else
	{
		if(aip->ai_flags&AI_PASSIVE)
		{
			cout << " passive" << endl;
		}
		if(aip->ai_flags&AI_CANONNAME)
		{
			cout << " numhost" << endl;
		}
		if(aip->ai_flags&AI_NUMERICSERV)
		{
			cout << " numserv" << endl;
		}
		if(aip->ai_flags&AI_V4MAPPED)
		{
			cout << " v4mapped" << endl;
		}
		if(aip->ai_flags&AI_ALL)
		{
			cout << " all" << endl;
		}
	}
	
}


int main(int argc,char* argv[])
{
	struct addrinfo *ailist, *aip;
	struct addrinfo hint;
	struct sockaddr_in *sinp;
	const char *addr;
	int err;
	char abuf[INET_ADDRSTRLEN];

	if(argc!=3)
	{
		cerr << "usage: " << argv[0] << " nodename service" << endl;
		return 0;
	}

	hint.ai_family = 0;
	hint.ai_flags = AI_CANONNAME;
	hint.ai_socktype = 0;
	hint.ai_protocol = 0;
	hint.ai_addrlen = 0;
	hint.ai_canonname = 0;
	hint.ai_addr = nullptr;
	hint.ai_next = nullptr;

	if((err = getaddrinfo(argv[1],argv[2],&hint,&ailist))!=0)
	{
		cerr << "getaddrinfo error: " << gai_strerror(err) << endl;
		return 0;
	}

	for (aip = ailist; aip != nullptr;aip = aip->ai_next)
	{
		print_flags(aip);
		print_family(aip);
		print_type(aip);
		print_protocol(aip);
		cout << (aip->ai_canonname ? aip->ai_canonname : "-")<< "   ---host" << endl;

		if(aip->ai_family==AF_INET)
		{
			sinp = (struct sockaddr_in *)aip->ai_addr;
			addr = inet_ntop(AF_INET, &sinp->sin_addr, abuf, INET_ADDRSTRLEN);
			cout << " address is " << (addr ? addr : "unknown") << endl;
			cout << " port is " << ntohs(sinp->sin_port) << endl;
		}
	}

	return 0;
}