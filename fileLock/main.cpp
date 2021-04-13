//使用文件在不同进程间实现锁功能
#include<cstdio>
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

