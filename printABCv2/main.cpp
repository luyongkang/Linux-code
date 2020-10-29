#include<iostream>
#include<pthread.h>
#include<unistd.h>
#include<string>
using namespace std;

void *thr_fn1(void *);
void *thr_fn2(void *);
void *thr_fn3(void *);

pthread_cond_t exeThr1;
pthread_cond_t exeThr2;
pthread_cond_t exeThr3;

int thr1 = 0;//0表示A等待打印，1表示A已经打印
int thr2 = 0;
int thr3 = 0;

pthread_mutex_t cond;

int main()
{
	pthread_t tid1, tid2, tid3;
	pthread_cond_init(&exeThr1, nullptr);
	pthread_cond_init(&exeThr2, nullptr);
	pthread_cond_init(&exeThr3, nullptr);
	pthread_mutex_init(&cond, nullptr);
	pthread_mutex_lock(&cond);
	thr1 = 0;
	thr2 = 1;
	thr3 = 1;
	pthread_mutex_unlock(&cond);

	pthread_create(&tid1, nullptr, thr_fn1, nullptr);
	pthread_create(&tid2, nullptr, thr_fn2, nullptr);
	pthread_create(&tid3, nullptr, thr_fn3, nullptr);

	pthread_join(tid1, nullptr);
	pthread_join(tid2, nullptr);
	pthread_join(tid3, nullptr);
	cout << endl;

	pthread_cond_destroy(&exeThr1);
	pthread_cond_destroy(&exeThr2);
	pthread_cond_destroy(&exeThr3);
	pthread_mutex_destroy(&cond);
	return 0;
}

void* thr_fn1(void* arg )
{
	for (int i = 0; i < 10;i++)
	{
		pthread_mutex_lock(&cond);
		while (thr1 == 1)
			pthread_cond_wait(&exeThr1, &cond);
		cout << "A";
		thr1 = 1;
		thr2 = 0;
		pthread_cond_signal(&exeThr2);
		pthread_mutex_unlock(&cond);
	}

	return (void *)0;
}

void* thr_fn2(void* arg )
{
	for (int i = 0; i < 10;i++)
	{
		pthread_mutex_lock(&cond);
		while(thr2==1)
		{
			pthread_cond_wait(&exeThr2, &cond);
		}
		cout << "B";
		thr2 = 1;
		thr3 = 0;
		pthread_cond_signal(&exeThr3);
		pthread_mutex_unlock(&cond);
	}

	return (void *)0;
}

void* thr_fn3(void* invalid_argument)
{
	for (int i = 0; i < 10;i++)
	{
		pthread_mutex_lock(&cond);
		while(thr3==1)
			pthread_cond_wait(&exeThr3, &cond);
		cout << "C";
		thr3 = 1;
		thr1 = 0;
		pthread_cond_signal(&exeThr1);
		pthread_mutex_unlock(&cond);
	}

	return (void *)0;
}