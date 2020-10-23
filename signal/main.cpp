#include<iostream>
#include<csignal>
#include<unistd.h>
#include<cstdio>
using namespace std;
static void sig_usr(int);
int main()
{
	if(signal(SIGUSR1,sig_usr)==SIG_ERR)
	{
		cerr<<"can't catch sigusr1"<<endl;
	}

	if(signal(SIGUSR2,sig_usr)==SIG_ERR)
	{
		cerr<<"can't catch sigusr2"<<endl;
	}

	while(true)
	{
		pause();
	}


}

static void sig_usr(int signo)
{
	if(signo==SIGUSR1)
	{
		cout<<"receive sigusr1"<<endl;
	}
	else if(signo==SIGUSR2)
	{
		cout<<"receive sigusr2"<<endl;
	}

	else
	{
		cout<<"receive signal "<<signo<<endl;
		abort();
	}

	
}