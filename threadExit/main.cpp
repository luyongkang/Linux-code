#include<iostream>
#include<pthread.h>
#include<unistd.h>
#include<string>
using namespace std;
void *thr1_func(void *);
void *thr2_func(void *);
void printInfo(const string& name);
int main()
{
	pthread_t thr1, thr2;
	int err;
	err = pthread_create(&thr1, nullptr, thr1_func, nullptr);
	if(err!=0)
	{
		cerr << "pthread_create err" << endl;
	}

	err = pthread_create(&thr2, nullptr, thr2_func, nullptr);
	if(err!=0)
	{
		cerr << "pthread_create err" << endl;
	}

	printInfo("main");

	void *thr1_return;
	void *thr2_return;
	err = pthread_join(thr1, &thr1_return);
	if(err!=0)
	{
		cerr << "pthread_join err" << endl;
	}
	cout << "thread1 return " << (long)thr1_return << endl;
	err = pthread_join(thr2, &thr2_return);
	if (err != 0)
	{
		cerr << "pthread_join err" << endl;
	}
	cout << "thread2 return " << (long)thr2_return << endl;
}

void printInfo(const string& name)
{
	cout << name << " thread: pid=" << getpid() << ", tid=" << pthread_self() << endl;
	return;
}

void* thr1_func(void* arg )
{
	cout << "thr1 begin:" << endl;
	printInfo("thr1");
	cout << "thr1 end" << endl;
	return (void *)0;
}

void* thr2_func(void* arg )
{
	cout << "thr2 begin:" << endl;
	printInfo("thr2");
	cout << "thr2 end" << endl;
	pthread_exit((void *)2);
}