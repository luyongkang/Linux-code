#include<iostream>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
using namespace std;

int main()
{
	int fd[2];
	pid_t pid;
	char line[1024];

	if(pipe(fd)<0)
	{
		cerr << "pipe error" << endl;
		return 0;
	}
	if((pid=fork())<0)
	{
		cerr << "fork error" << endl;
		return 0;
	}
	else if(pid>0) //parent
	{
		close(fd[0]);
		write(fd[1], "hello worldfsljeekfj", 10);
	}
	else
	{
		close(fd[1]);
		read(fd[0], line, 1024);
		cout << line << endl;
		return 0;
	}
	int *returnvalue = new int();
	waitpid(pid, returnvalue, 0);
	cout << "son end" << endl;
	cout << "son returnvalue:" << (*returnvalue) << endl;
	delete returnvalue;

	return 0;
}