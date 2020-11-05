#include<fcntl.h>
#include<sys/mman.h>
#include<iostream>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<cstring>
using namespace std;

#define COPYINCR (1024 * 1024 * 1024)


int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		cerr << "command usage:copy inputfile outputfile" << endl;
	}

	int inputfd, outputfd;
	if((inputfd=open(argv[1],O_RDONLY))<0)
	{
		cerr << "open inputfile:" << argv[1] << " error" << endl;
		return 0;
	}

	if((outputfd=open(argv[2],O_RDWR|O_CREAT|O_TRUNC,S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH))<0)
	{
		cerr << "open outputfile:" << argv[2] << " error" << endl;
		return 0;
	}
	struct stat buf;
	if(fstat(inputfd,&buf)<0)
	{
		cerr << "get stat error" << endl;
		return 0;
	}
	if(ftruncate(outputfd,buf.st_size)<0)
	{
		cerr << "set outputfile size error" << endl;
		return 0;
	}

	size_t copysz;
	void *src, *dst;
	off_t index =0;
	while(index<buf.st_size)
	{
		if((buf.st_size-index)<COPYINCR)
		{
			copysz = buf.st_size - index;
		}
		else
		{
			copysz = COPYINCR;
		}
		src = mmap(0, copysz, PROT_READ, MAP_SHARED, inputfd, index);
		if(src==MAP_FAILED)
		{
			cerr << "mmap failed" << endl;
			return 0;
		}
		dst = mmap(0, copysz, PROT_WRITE, MAP_SHARED, outputfd, index);
		if(dst==MAP_FAILED)
		{
			cerr << "mmap failed" << endl;
			return 0;
		}
		memcpy(dst, src, copysz);
		munmap(src, copysz);
		munmap(dst, copysz);
		index += copysz;
	}

	close(inputfd);
	close(outputfd);
	return 0;
}