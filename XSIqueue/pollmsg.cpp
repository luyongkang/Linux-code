#include <iostream>
#include <poll.h>
#include <pthread.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <memory.h>
#include <unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#define NQ 3
#define MAXMSZ 512
#define KEY 0x123

using namespace std;
struct threadinfo
{
	int qid; //消息队列id
	int fd;	 //unix域套接字
};

struct mymesg
{
	long mtype;
	char mtext[MAXMSZ];
};

void *helper(void *arg)
{
	int n;
	mymesg m;
	threadinfo *tip = static_cast<threadinfo *>(arg);

	while(true)
	{
		memset(&m, 0, sizeof(m));
		n = msgrcv(tip->qid, &m, MAXMSZ, 0, MSG_NOERROR);
		if(n<0)
		{
			cerr << "msgrcv error" << endl;
		}
		if(write(tip->fd,m.mtext,n)<0)
		{
			cerr << "write error" << endl;
		}
	}
}

int main()
{
	int i, n, err;
	int fd[2];
	int qid[NQ];//消息队列id
	pollfd pfd[NQ];
	threadinfo ti[NQ];//线程参数
	pthread_t tid[NQ]; //线程id;
	char buf[MAXMSZ];

	for (i = 0; i < NQ;i++)
	{
		if((qid[i]=msgget((KEY+i),IPC_CREAT|0666))<0)
		{
			cerr << "msgget error" << endl;
		}
		cout << "queue id " << i << " is " << qid[i] << endl;

		if(socketpair(AF_UNIX,SOCK_DGRAM,0,fd)<0)
		{
			cerr << "socketpair error" << endl;
		}

		pfd[i].fd = fd[1];
		pfd[i].events = POLLIN;
		ti[i].fd = fd[0];
		ti[i].qid = qid[i];

		err = pthread_create(&tid[i], nullptr, helper, &ti[i]);
		if(err!=0)
		{
			cerr << "pthread_create err" << endl;
			exit(1);
		}
	}

	while(true)
	{
		if(poll(pfd,NQ,-1)<0)
		{
			cerr << "poll err" << endl;
		}

		for (int i = 0; i < NQ;i++)
		{
			if(pfd[i].revents&POLLIN)
			{
				if((n=read(pfd[i].fd,buf,sizeof(buf)))<0)
				{
					cerr << "read err" << endl;
				}
				buf[n] = 0;//使得字符串结尾为\0
				cout << "queue id" << i << " ,message " << buf << endl;
			}
		}
	}
	exit(0);
}