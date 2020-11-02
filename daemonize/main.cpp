#include<syslog.h>
#include<fcntl.h>
#include<sys/resource.h>
#include<unistd.h>
#include<iostream>
#include<csignal>
#include<cstdlib>
#include<sys/types.h>
#include<sys/stat.h>
using namespace std;
void daemonize(const char *cmd);
int main()
{
	daemonize("daemonize");
	sleep(1000);
	return 0;
}

void daemonize(const char*cmd )
{
	int i, fd0, fd1, fd2;
	pid_t pid;
	rlimit r1;
	struct sigaction sa;

	umask(0);

	if(getrlimit(RLIMIT_NOFILE,&r1)<0)
	{
		cerr << cmd << ": can't get file limit" << endl;
	}

	if((pid=fork())<0)
	{
		cerr << cmd << ": can't fork" << endl;
	}
	else if(pid>0)
	{
		return;
	}

	setsid();

	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if(sigaction(SIGHUP,&sa,nullptr)<0)
		cerr << cmd << ": can't ignore SIGHUP" << endl;
	
	if((pid=fork())<0)
	{
		cerr << cmd << ": can't fork" << endl;
	}
	else if(pid>0)
	{
		exit(0);
	}

	if(chdir("/")<0)
	{
		cerr << cmd << ": can't change directory to /" << endl;
	}

	if(r1.rlim_max==RLIM_INFINITY)
		r1.rlim_max = 1024;
	for (i = 0; i < r1.rlim_max;++i)
	{
		close(i);
	}

	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	openlog(cmd, LOG_CONS, LOG_DAEMON);
	if(fd0!=0||fd1!=0||fd2!=0)	
	{
		syslog(LOG_ERR, "unexpected file descriptors %d %d %d",fd0,fd1,fd2);
		exit(0);
	}
}