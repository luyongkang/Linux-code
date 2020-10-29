#include<iostream>
#include<pthread.h>
#include<unistd.h>
using namespace std;
void *thr_fn(void *);
int main()
{
	pthread_attr_t attr;
	pthread_attr_t attr2;
	pthread_t tid;
	int err = 0;
	int detach = 0;

	err = pthread_attr_init(&attr);
	err = pthread_attr_init(&attr2);
	if (err != 0)
	{
		cout << "pthread_attr_init error" << endl;
	}
	err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	err = pthread_create(&tid, &attr, thr_fn, (void*)9);
	if(err!=0)
	{
		cerr << "pthread_create error" << endl;
	}

	err = pthread_attr_getdetachstate(&attr, &detach);
	if(detach==PTHREAD_CREATE_DETACHED)
		cout << "PTHREAD_CREATE_DETACHED" << endl;
	else
	{
		cout << "PTHREAD_CREATE_JOINABLE" << endl;
	}
	void *ret;
	size_t size;
	err = pthread_attr_getstack(&attr, &ret, &size);
	cout << "stackaddress" << ret << endl;
	cout << "stacksize" << size << endl;
	err = pthread_attr_getstack(&attr2, &ret, &size);
	cout << "stackaddress2" << ret << endl;
	cout << "stacksize2" << size << endl;
	err = pthread_attr_getguardsize(&attr, &size);
	cout << "guardsize" << size << endl;
	err = pthread_attr_destroy(&attr);
	err = pthread_attr_destroy(&attr2);
	err = pthread_join(tid, &ret);
	if(err!=0)
		cerr << "pthread_join err" << endl;
	cout << "return :" << (long)ret << endl;
	return 0;
}

void* thr_fn(void* arg )
{
	sleep(5);
	for (int i = 0; i < 10;i++)
	{
		cout << "i=" << i << endl;
	}
		return (void *)9;
}