#include<iostream>
#include<pthread.h>

using namespace std;

void *thrA(void *);
void *thrB(void *);
void *thrC(void *);

pthread_mutex_t A = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t B = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t C = PTHREAD_MUTEX_INITIALIZER;
int main()
{
	pthread_mutex_lock(&A);
	pthread_mutex_lock(&B);
	pthread_mutex_lock(&C);
	int err = 0;
	pthread_t tidA, tidB, tidC;
	err = pthread_create(&tidA, nullptr, thrA, nullptr);
	if(err!=0)
	{
		cout << "create pthread A fail" << endl;
	}
	err = pthread_create(&tidB, nullptr, thrB, nullptr);
	if(err!=0)
	{
		cout << "create pthread B fail" << endl;
	}
	err = pthread_create(&tidC, nullptr, thrC, nullptr);
	if(err!=0)
	{
		cout << "create pthread C fail" << endl;
	}

	pthread_mutex_unlock(&A);
	err = pthread_join(tidA, nullptr);
	err = pthread_join(tidB, nullptr);
	err = pthread_join(tidC, nullptr);
	pthread_mutex_lock(&A);
	cout << endl;
	return 0;
}

void* thrA(void* arg)
{
	for (int i = 0; i < 10;i++)
	{
		pthread_mutex_lock(&A);
		cout << "A";
		pthread_mutex_unlock(&B);
	}
	return (void *)0;
}

void* thrB(void* arg )
{
	for (int i = 0; i < 10;i++)
	{
		pthread_mutex_lock(&B);
		cout << "B";
		pthread_mutex_unlock(&C);
	}
	return (void *)0;
}

void* thrC(void* arg )
{
	for (int i = 0; i < 10;i++)
	{
		pthread_mutex_lock(&C);
		cout << "C";
		pthread_mutex_unlock(&A);
	}
	return (void *)0;
}