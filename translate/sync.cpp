#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<ctype.h>

using namespace std;

char translate(char c);

char buf[4096];

int main(int argc,char* argv[])
{
	if(argc!=3)
	{
		cerr << "command usage: debug inputfile outputfile" << endl;
		return 0;
	}

	int inputfd, outputfd;
	if((inputfd=open(argv[1],O_RDONLY))<0)
	{
		cerr << "open inputfile " << argv[1] << " error" << endl;
		return 0;
	}

	if((outputfd=open(argv[2],O_WRONLY))<0)
	{
		cerr << "open outputfile " << argv[2] << " error" << endl;
		return 0;
	}

	int n;
	while((n=read(inputfd,buf,4096))>0)
	{
		for (int i = 0; i < n;++i)
		{
			buf[i] = translate(buf[i]);
		}
		if(write(outputfd,buf,n)!=n)
		{
			cerr << "write failed" << endl;
		}
	}

	fsync(outputfd);
	close(inputfd);
	close(outputfd);
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