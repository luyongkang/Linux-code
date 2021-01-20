#include<iostream>
#include<sys/socket.h>
#include<unistd.h>

#define MAXSLEEP 128
int connect_retry(int domain, int type, int protocol, const struct sockaddr *addr, socklen_t alen)
{
	int numsec, fd;

	for (numsec = 1; numsec <= MAXSLEEP;numsec<<=1)
	{
		if((fd=socket(domain,type,protocol))<0)
		{
			return -1;//函数发生错误`
		}
		
		if(connect(fd,addr,alen)==0)
		{
			//connect success;
			return fd;
		}

		close(fd); 
		if(numsec<=MAXSLEEP/2)
		{
			sleep(numsec);
		}
	}

	return -1;
}