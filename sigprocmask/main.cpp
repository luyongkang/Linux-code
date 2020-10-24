#include<iostream>
#include<csignal>
#include<unistd.h>


static void sig_quit(int);
int main()
{
	sigset_t newmask,oldmask,pendmask;

	if(signal(SIGQUIT,sig_quit)==SIG_ERR)
	{
		std::cerr<<"signal err"<<std::endl;
	}

	sigemptyset(&newmask);
	sigaddset(&newmask,SIGQUIT);
	if(sigprocmask(SIG_BLOCK,&newmask,&oldmask)<0)
	{
		std::cerr<<"sigprocmask err"<<std::endl;
	}

	sleep(5);

	if(sigpending(&pendmask)<0)
	{
		std::cerr<<"sigpending err"<<std::endl;
	}

	if(sigismember(&pendmask,SIGQUIT))
	{
		std::cerr<<"sigquit pending"<<std::endl;
	}

	if(sigprocmask(SIG_SETMASK,&oldmask,nullptr)<0)
	{
		std::cerr<<"sigprocmask err"<<std::endl;
	}

	std::cout<<"sigquit unblocked"<<std::endl;

	sleep(5);

	pause();
	return 0;
	
}

static void sig_quit(int signo)
{
	std::cout<<"catch sigquit"<<std::endl;
	return ;
}