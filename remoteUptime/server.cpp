//服务器代码，提供系统正常运行时间
#include<signal.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/resource.h>
#include<string.h>
#include<fcntl.h>
#include<iostream>
#include<netdb.h>
#include<errno.h>
#include<syslog.h>
#include<sys/socket.h>
#include<unistd.h>
#define BUFLEN 128
#define QLEN 10

using namespace std;

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

int initserver(int, const struct sockaddr *, socklen_t, int);

void serve(int sockfd);

int set_cloexec(int fd)	//对fd设置执行时关闭标志，执行时关闭标志是执行exec另一个程序时，该fd会自动关闭
{
	int val;
	if((val=fcntl(fd,F_GETFD,0))<0)
	{
		return -1;
	}

	val |= FD_CLOEXEC;

	return (fcntl(fd, F_SETFD, val)); 
}

void daemonize(const char *cmd); //初始化一个守护进程

int main(int argc,char* argv[])
{
	addrinfo *ailist, *aip;
	addrinfo hint;
	int sockfd, err, n;
	char *host;

	if(argc!=1)
	{
		cerr << "usage: remoteUptime" << endl;
	}
	if((n=sysconf(_SC_HOST_NAME_MAX))<0)
	{
		n = HOST_NAME_MAX;
	}
	if((host=new char[n])==nullptr)	
	{
		cerr << "new error" << endl;
	}
	if(gethostname(host,n)<0)
	{
		cerr << "gethostname error" << endl;
	}
	daemonize("remoteUptime");
	memset(&hint, 0, sizeof(hint));
	hint.ai_flags = AI_CANONNAME;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_canonname = nullptr;
	hint.ai_addr = nullptr;
	hint.ai_next = nullptr;

	if((err=getaddrinfo(host,"remoteUptime",&hint,&ailist))!=0)
	{
		syslog(LOG_ERR, "remoteUptime:getaddrinfo error:%s", gai_strerror(err));
		return;
	}

	for (aip = ailist; aip != nullptr;aip=aip->ai_next)
	{
		if(sockfd=initserver(SOCK_STREAM,aip->ai_addr,aip->ai_addrlen,QLEN)>=0)
		{
			serve(sockfd);
			return;
		}
	}
	return;
}

int initserver(int type, const struct sockaddr *addr, socklen_t alen,int qlen)
{
	int fd;
	int err = 0;

	if((fd=socket(addr->sa_family,type,0))<0)
	{
		cerr << "socket error" << endl;
		return -1;
	}

	if (bind(fd, addr, alen)<0)
	{
		err = errno;
		close(fd);
		errno = err;
		return -1;
	}

	if(type==SOCK_STREAM||type==SOCK_SEQPACKET)
	{
		if(listen(fd,qlen)<0)
		{
			err = errno;
			close(fd);
			errno = err;
			return -1;
		}

		return fd;
	}
}

void serve(int sockfd)
{
	int clfd;
	FILE *fp;
	char buf[BUFLEN];

	set_cloexec(sockfd);
	for (;;)
	{
		if((clfd=accept(sockfd,nullptr,nullptr))<0)
		{
			syslog(LOG_ERR, "remoteUptime: accept error: %s", strerror(errno));
			return;
		}
		set_cloexec(clfd);
		if((fp=popen("/usr/bin/uptime","r"))==nullptr)
		{
			cout << buf << "error: " << strerror(errno) << endl;
			send(clfd, buf, strlen(buf), 0);
		}
		else
		{
			while(fgets(buf,BUFLEN,fp)!=nullptr)
			{
				send(clfd, buf, strlen(buf), 0);
			}
			pclose(fp);
		}
		close(clfd);
	}
}

void daemonize(const char* cmd )
{
	int i, fd0, fd1, fd2;
	pid_t pid;
	rlimit rl;
	struct sigaction sa;

	umask(0);

	if(getrlimit(RLIMIT_NOFILE,&rl)<0)
	{
		cerr << cmd << ": can't get file limit" << endl;
		return;
	}

	if((pid=fork())<0)
	{
		cerr << cmd << ": can't fork" << endl;
		return;
	}
	else if(pid!=0)
	{
		return;
	}
	setsid();

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if(sigaction(SIGHUP,&sa,NULL)<0)
	{
		cerr << cmd << ": can't ignore SIGHUP" << endl;
		return;
	}
	if((pid=fork())<0)
	{
		cerr << cmd << ": can't fork" << endl;
	}
	else if(pid!=0)
	{
		return;
	}

	if(chdir("/")<0)
	{
		cerr << cmd << ": can't change directory to /" << endl;
	}

	if(rl.rlim_max==RLIM_INFINITY)
		rl.rlim_max = 1024;
	for (i = 0; i < rl.rlim_max;i++)
	{
		close(i);
	}

	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	openlog(cmd, LOG_CONS, LOG_DAEMON);
	if(fd0!=0||fd1!=1||fd2!=2)
	{
		syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
		return;
	}
}