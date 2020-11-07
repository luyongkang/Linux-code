#include<iostream>
#include<sys/wait.h>
using namespace std;

int main()
{
	char line[1024];
	FILE *fpin;

	if((fpin=popen("/home/luyongkang/VSCodeProject/Linux-code/popen-example/myuclc","r"))==nullptr)
	{
		cerr << "popen error" << endl;
	}
	while(true)
	{
		fputs("prompt> ", stdout);
		fflush(stdout);
		if(fgets(line,1024,fpin)==nullptr)
		{
			break;
		}
		if(fputs(line,stdout)==EOF)
		{
			cerr << "fputs error to pipe" << endl;
		}
	}
	if(pclose(fpin)==-1)
	{
		cerr << "pclose error" << endl;
	}

	putchar('\n');
	return 0;
}