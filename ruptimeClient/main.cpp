#include<iostream>
#include<netdb.h>
#include<errno.h>
#include<sys/socket.h>
#include<memory.h>
#include<unistd.h>
#define BUFLEN 128

using namespace std;

int connect_retry(int, int, int, const struct sockaddr *);

void print_uptime(int sockfd);

int main(int argc,char* argv[])
{
	int sockfd, err;
	addrinfo *aip, *ailist;
	addrinfo hint;

	if(argc!=2)
	{
		cerr << "usage: ruptime hostname" << endl;
		exit(1);
	}

	memset(&hint, 0, sizeof(hint));
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_canonname = nullptr;
	hint.ai_addr = nullptr;
	hint.ai_next = nullptr;
	err=getaddrinfo(argv[1], "ruptime", &hint, &ailist);
	if(err!=0)
	{
		cerr << "getaddrinfo error:" << gai_strerror(err);
		exit(1);
	}

	for (aip = ailist; aip != nullptr;aip=aip->ai_next)
	{
		sockfd = connect_retry(aip->ai_family, SOCK_STREAM, 0, aip->ai_addr, aip->ai_addrlen);
		if(sockfd<0)
		{
			err = errno;
		}
		else
		{
			print_uptime(sockfd);
			exit(0);
		}
	}

	cerr << "can't connect to " << argv[1] << endl;
	exit(1);
}

void print_uptime(int sockfd)
{
	int n;
	char buf[128];

	while((n=recv(sockfd,buf,128,0))>0)
	{
		write(STDOUT_FILENO, buf, n);
	}
	if(n<0)
	{
		cerr << "recv error" << endl;
	}
}

int connect_retry(int domain,int type,int protocol,sockaddr* addr,socklen_t alen)
{
	int numsec, sockfd;

	for (numsec = 1; numsec <= 128; numsec = numsec<<1)
	{
		if((sockfd=socket(domain,type,protocol))<0)
		{
			return -1;
		}
		if(connect(sockfd,addr,alen)==0)
		{
			return sockfd;
		}
		close(sockfd);

		if(numsec<=64)
		{
			sleep(numsec);
		}
	}
	return -1;
}