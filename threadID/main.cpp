#include<iostream>
#include<pthread.h>
#include<unistd.h>
using namespace std;
pthread_t ntid;

void *thr_fn(void *);
int main()
{
	if(pthread_create(&ntid,nullptr,thr_fn,nullptr)!=0)
	{
		cerr << "pthread create failed" << endl;
	}
	sleep(5);
	cout << "this is mainThread: pid=" << getpid() << ",tid=" << pthread_self() << endl;
	return 0;
}

void* thr_fn(void* arg)
{
	cout << "this is newThread: pid=" << getpid() << ",tid=" << pthread_self() << endl;
	return nullptr;
}
