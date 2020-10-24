#include"sleep.h"
#include<iostream>
#include<unistd.h>
#include<csignal>
static void sig_alrm(int signo)
{
	std::cout<<"call alrm handler"<<std::endl;
}

unsigned int mySleep(unsigned int seconds)
{
	std::cout<<"sleep begin"<<std::endl;
	void (*temp)(int);
	if((temp=signal(SIGALRM,sig_alrm))==SIG_ERR)
	{
		std::cerr<<"signal err"<<std::endl;
		return seconds;
	}
	alarm(seconds);
	pause();
	if(signal(SIGALRM,temp)==SIG_ERR)
	{
		std::cerr<<"signal err"<<std::endl;
		return seconds;
	}
	std::cout<<"sleep over,sleep "<<seconds-alarm(0)<<" seconds"<<std::endl;
	return alarm(0);
}