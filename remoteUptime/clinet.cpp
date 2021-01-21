//本程序可以从服务其获取其正常运行时间以及占用
#include<unistd.h>
#include<iostream>
#include<netdb.h>
#include<errno.h>
#include<sys/socket.h>
#include<string.h>
#define BUFLEN 128

using namespace std;
int connect_retry(int, int, int, const struct sockaddr *, socklen_t);

void print_uptime(int sockfd);

int main(int argc,char* argv[])
{
	struct addrinfo *ailist, *aip;
	struct addrinfo hint;
	int sockfd, err;

	if(argc!=2)
	{
		cerr << "usage: remoteUptime hostname" << endl;
	}
	memset(&hint, 0, sizeof(hint));
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_canonname = nullptr;
	hint.ai_addr = nullptr;
	hint.ai_next = nullptr;

	if((err=getaddrinfo(argv[1],"remoteUptime",&hint,&ailist)!=0))
	{
		cerr << "getaddrinfo error: " << gai_strerror(err) << endl;
	}

	for (aip = ailist; aip != nullptr;aip=aip->ai_next)
	{
		if((sockfd = connect_retry(aip->ai_family,SOCK_STREAM,0,aip->ai_addr,aip->ai_addrlen))<0)
		{
			err = errno;
		}
		else
		{
			print_uptime(sockfd);
			return 0;
		}
	}

	cerr << "can't connect to " << argv[1] << endl;
	return 0;
}

int connect_retry(int domain,int type,int protocol,const struct sockaddr* addr,socklen_t alen)
{
	int fd, numsec;

	for (numsec = 1; numsec <= BUFLEN;numsec<<=1)
	{
		if((fd=socket(domain,type,protocol))<0)
		{
			cerr << "socket error" << endl;
			return 0;
		}

		if(connect(fd,addr,alen)==0)
		{
			return fd; //connect succes;
		}

		close(fd);

		if(numsec<=BUFLEN/2)
		{
			sleep(numsec);
		}
	}

	return -1;
}

void print_uptime(int sockfd)
{
	int n;
	char buf[BUFLEN];
	while((n=recv(sockfd,buf,BUFLEN,0))>0)
	{
		cout << buf << endl;
	}

	if(n<0)
	{
		cerr << "recv error" << endl;
	}
}