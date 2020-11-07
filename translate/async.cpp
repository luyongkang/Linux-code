#include<iostream>
#include<ctype.h>
#include<fcntl.h>
#include<aio.h>
#include<errno.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<unistd.h>

using namespace std;
#define BSZ 4096
#define NBUF 8

enum rwop
{
	UNUSED = 0,
	READ_PENDING = 1,
	WRITE_PENDING = 2

};

struct buf 
{
	enum rwop op;
	int last;
	struct aiocb aiocb;
	unsigned char data[BSZ];
};

struct buf bufs[NBUF];

char translate(char c);

int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		cerr << "commands usage: asyncexe inputfile outputfile" << endl;
		return 0;
	}
	int ifd, ofd;
	if((ifd=open(argv[1],O_RDONLY))<0)
	{
		cerr << "can't open file:" << argv[1] << endl;
		return 0;
	}
	if((ofd=open(argv[2],O_RDWR|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH))<0)
	{
		cerr << "can't open file:" << argv[2] << endl;
		return 0;
	}

	struct stat sbuf;
	if(fstat(ifd,&sbuf)<0)
	{
		cerr << "fstat error" << endl;
		return 0;
	}

	const struct aiocb *aiolist[NBUF];
	for (int i = 0; i < NBUF; i++)
	{
		bufs[i].op = UNUSED;
		bufs[i].aiocb.aio_buf = bufs[i].data;
		bufs[i].aiocb.aio_sigevent.sigev_notify = SIGEV_NONE;
		aiolist[i] = nullptr;
	}

	int err;
	int n;
	int numop = 0;
	int i = 0;
	off_t off = 0;
	while (true)
	{
		for (i = 0; i < NBUF;i++)
		{
			switch(bufs[i].op)
			{
				case UNUSED:
					if (off < sbuf.st_size)
					{
						bufs[i].op = READ_PENDING;
						bufs[i].aiocb.aio_fildes = ifd;
						bufs[i].aiocb.aio_offset = off;
						off += BSZ;
						if(off>=sbuf.st_size)
						{
							bufs[i].last = 1;
						}
						bufs[i].aiocb.aio_nbytes = BSZ;
						if(aio_read(&bufs[i].aiocb)<0)
						{
							cerr << "aio_read error" << endl;
						}
						aiolist[i] = &bufs[i].aiocb;
						numop++;
					}
					break;
				case READ_PENDING:
					if((err=aio_error(&bufs[i].aiocb))==EINPROGRESS)
					{
						continue;
					}
					if (err != 0)
					{
						if(err==-1)
						{
							cerr << "aio_error failed" << endl;
						}
						else
						{
							cerr << "read failed" << endl;
							return 0;
						}
					}

					if((n=aio_return(&bufs[i].aiocb))<0)
					{
						cerr << "aio_return failed" << endl;
					}
					if(n!=BSZ&&!bufs[i].last)
					{
						cerr << "short read " << n << "/" << BSZ << endl;
						return 0;
					}
					for (int j = 0; j < n;j++)
					{
						bufs[i].data[j] = translate(bufs[i].data[j]);
					}
					bufs[i].op = WRITE_PENDING;
					bufs[i].aiocb.aio_fildes = ofd;
					bufs[i].aiocb.aio_nbytes = n;
					if(aio_write(&bufs[i].aiocb)<0)
					{
						cerr << "aio_write failed" << endl;
					}
					break;

				case WRITE_PENDING:
					if((err=aio_error(&bufs[i].aiocb))==EINPROGRESS)
					{
						continue;
					}
					if(err!=0)
					{
						if(err==-1)
						{
							cerr << "aio_error failed" << endl;
						}
						else
						{
							cerr << "wirte failed" << endl;
							return 0;
						}
					}

					if((n=aio_return(&bufs[i].aiocb))<0)
					{
						cerr << "aio_return failed" << endl;
					}
					if(n!=bufs[i].aiocb.aio_nbytes)
					{
						cerr << "short write " << n << "/" << BSZ << endl;
						return 0;
						
					}
					aiolist[i] = nullptr;
					bufs[i].op = UNUSED;
					numop--;
					break;
				
			}
		}
		if(numop==0)
		{
			if(off>=sbuf.st_size)
			{
				break;
			}
			else
			{
				if(aio_suspend(aiolist,NBUF,nullptr)<0)
				{
					cerr << "aio_suspend failed" << endl;
				}
			}
		}
	}
	bufs[0].aiocb.aio_fildes = ofd;
	if(aio_fsync(O_SYNC,&bufs[0].aiocb)<0)
	{
		cerr << "aio_fsync failed" << endl;
	}

	return 0;
}
char translate(char c)
{
	if(isalpha(c))
	{
		if(c>='n')
		{
			c -= 13;
		}
		else if(c>='a')
		{
			c += 13;
		}
		else if(c>='N')
		{
			c -= 13;
		}
		else
		{
			c += 13;
		}
	}

	return c;
}
