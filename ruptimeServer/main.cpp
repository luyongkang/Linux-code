#include<netdb.h>
#include<errno.h>
#include<syslog.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<iostream>
#include<unistd.h>
#include<memory.h>
#include<sys/resource.h>
#include<sys/types.h>
#include<sys/stat.h>

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

using namespace std;

void daemonize(const char *cmd);
int initserver(int type, const sockaddr *addr, socklen_t alen, int qlen);
void serve(int sockfd);
int set_cloexec(int fd);


int main(int argc, char *argv[])
{
	addrinfo *ailist, *aip;
	addrinfo hint;
	int sockfd, err, n;
	char *host;

	if(argc!=1)
	{
		cerr << "usage: ruptimed" << endl;
		return 0;
	}
	if((n=sysconf(_SC_HOST_NAME_MAX))<0)
	{
		n = HOST_NAME_MAX;
	}
	host = new char[n];
	gethostname(host, n);

	cout << "gethostname" << endl;

	daemonize("ruptimed");
	cout << "daemonize" << endl;

	memset(&hint, 0, sizeof(hint));
	hint.ai_flags = AI_CANONIDN;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_canonname = nullptr;
	hint.ai_addr = nullptr;
	hint.ai_next = nullptr;
	err=getaddrinfo(host, "ruptime", &hint, &ailist);
	if(err!=0)
	{
		cerr << "ruptimed: getaddrinfo error " << gai_strerror(err) << endl;
		exit(1);
	}
	if(ailist==nullptr)
	{
		cout << "err" << endl;
	}
	for (aip = ailist; aip != nullptr; aip = aip->ai_next)
	{
		sockfd = initserver(SOCK_STREAM, aip->ai_addr, aip->ai_addrlen, 10); //sockfd是服务端监听的套接字描述符
		cout << aip->ai_addr->sa_data << endl;
		serve(sockfd);
		exit(0);
	}

	exit(1);
}

int initserver(int type,const sockaddr* addr,socklen_t alen,int qlen)
{
	int fd;
	int err = 0;

	fd = socket(addr->sa_family, type, 0);
	if(fd<0)
	{
		cout << "initserver: socket create fail" << endl;
		exit(0);
	}
	bind(fd, addr, alen);
	if(type==SOCK_STREAM||type==SOCK_SEQPACKET)
	{
		listen(fd, qlen);
	}

	return fd;
}

void daemonize(const char* cmd )
{
	int i, fd0, fd1, fd2;
	pid_t pid;
	rlimit rl;
	struct sigaction sa;

	umask(0);//给予该进程全权限

	getrlimit(RLIMIT_NOFILE, &rl);

	if((pid=fork())<0)
	{
		cerr << cmd << " can't fork" << endl;
		exit(0);
	}
	else if(pid!=0)
	{
		//this is parent
		exit(0);
	}

	//next it's children
	setsid();

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if(sigaction(SIGHUP,&sa,NULL)<0)
	{
		cout << cmd << " can't ignore SIGHUP" << endl;
	}

	if((pid=fork())<0)
	{
		cerr << cmd << " can't fork" << endl;
		exit(0);
	}
	else if(pid!=0)
	{
		//this is parent
		exit(0);
	}

	//next it's children
	if(chdir("/")<0)
	{
		cerr << " can't change directory to /" << endl;
		exit(0);
	}

/*
	if(rl.rlim_max==RLIM_INFINITY)
	{
		rl.rlim_max = 1024;
	}
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
		exit(1);
	}
*/
	return;
}

void serve(int sockfd)
{
	int clfd;
	FILE *fp;
	char buf[128];

	set_cloexec(sockfd);

	while(true)
	{
		clfd = accept(sockfd, nullptr, nullptr);
		if(clfd<0)
		{
			syslog(LOG_ERR, "ruptimed: accept error %s", strerror(errno));
			exit(1);
		}
		set_cloexec(clfd);
		fp = popen("/usr/bin/uptime", "r");
		if(fp==nullptr)
		{
			memcpy(buf, strerror(errno), strlen(strerror(errno)));
			buf[strlen(strerror(errno))] = '\0';
			send(clfd, buf, strlen(buf), 0);
		}
		else
		{
			while(fgets(buf,128,fp)!=nullptr)
			{
				send(clfd, buf, strlen(buf), 0);
			}
			pclose(fp);
		}
		close(clfd);
	}
}

int set_cloexec(int fd)//设置执行exec另一个程序时，关闭该文件描述符
{
	int val;

	if((val=fcntl(fd,F_GETFD,0))<0)
	{
		return -1;
	}

	val = val | FD_CLOEXEC;

	return (fcntl(fd, F_SETFD, val));
}