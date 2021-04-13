//使用文件在不同进程间实现锁功能
#include<sys/mman.h>
#include<pthread.h>
#include<cstdio>
#include<iostream>
#include<cstring>
#include<cstdlib>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
static int lockfd = -1;
static struct flock lock_it, unlock_it;

void myLockInit(char* pathname)
{
	char lockFile[1024];

	strncpy(lockFile, pathname, sizeof(lockFile));

	lockfd = mkstemp(lockFile);//用mkstemp创建的文件是临时文件，即使服务器崩溃最后该文件也会被删除

	if(unlink(lockFile)<0)	//unlink在有进程打开该文件时，是不会删除该文件的
	{
		perror(strerror(errno));
		exit(1);
	}

	lock_it.l_type = F_WRLCK;
	lock_it.l_whence = SEEK_SET;
	lock_it.l_start = 0;
	lock_it.l_len = 0;

	unlock_it.l_type = F_UNLCK;
	unlock_it.l_whence = SEEK_SET;
	unlock_it.l_start = 0;
	unlock_it.l_len = 0;

}


void myLockWait()//上锁文件
{
	int rc;
	while((rc=fcntl(lockfd,F_SETLKW,&lock_it))<0)
	{
		if(errno==EINTR)
			continue;
		else
		{
			perror(strerror(errno));
			exit(1);
		}
	}
}


void myLockRelease()//解锁文件
{
	int rc;
	if((rc=fcntl(lockfd,F_SETLKW,&unlock_it))<0)
	{
		perror(strerror(errno));
		exit(1);
	}
}



//使用线程上锁实现不同进程间的锁
static pthread_mutex_t *mptr;

void myLockInit2(char* pathname )
{
	int fd;
	pthread_mutexattr_t mattr;

	fd = open("dev/zero", O_RDWR, 0);

	mptr = static_cast<pthread_mutex_t*>(mmap(0, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));

	close(fd);//mptr已经指向了内存空间，关闭描述符不影响内存空间的使用

	pthread_mutexattr_init(&mattr);
	pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);//想要在进程间使用互斥锁必须要设置进程间共享标志
	pthread_mutex_init(mptr, &mattr);
}


void myLockWait2()
{
	if(pthread_mutex_lock(mptr)<0)
	{
		std::cerr << "pthread_mutex_lock error" << std::endl;
		exit(1);
	}
}

void myLockRelease()
{
	if(pthread_mutex_unlock(mptr)<0)
	{
		std::cerr << "pthread_mutex_unlock error" << std::endl;
		exit(1);
	}
}
