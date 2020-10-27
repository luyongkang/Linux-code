#include<iostream>
#include<ctime>
#include<pthread.h>
#include<unistd.h>
#include<cstring>
using namespace std;
int main()
{
	int err;
	timespec tout;
	tm *tmp;
	char buf[64];
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_lock(&lock);
	cout << "mutex is locked" << endl;
	clock_gettime(CLOCK_REALTIME, &tout);
	tmp = localtime(&tout.tv_sec);
	strftime(buf, sizeof(buf), "%r", tmp);
	cout << "current time is " << buf << endl;
	tout.tv_sec += 10;
	err = pthread_mutex_timedlock(&lock, &tout);
	clock_gettime(CLOCK_REALTIME, &tout);
	tmp = localtime(&tout.tv_sec);
	strftime(buf, sizeof(buf), "%r", tmp);
	cout << "the time is now " << buf << endl;
	if(err==0)
	{
		cerr << "mutex locked again" << endl;
	}
	else
	{
		cout << "can't lock mutex again:" << strerror(err) << endl;
	}
	pthread_mutex_destroy(&lock);
	return 0;
}