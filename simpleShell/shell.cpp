#include<iostream>
#include<sys/wait.h>
#include<string.h>
#include<unistd.h>

static void sig_int(int);
using namespace std;
int main()
{
    char buf[100];
    pid_t pid;
    int status;
    if(signal(SIGINT,sig_int)==SIG_ERR)
        cerr<<"signal error"  <<endl;
    cout<<"&& ";
    while(fgets(buf,100,stdin)!=nullptr)
    {
        if(buf[strlen(buf)-1]=='\n')
        {
            buf[strlen(buf)-1]=0;
        }

        if((pid=fork())<0)
            cerr<<"fork error"<<endl;
        else if(pid==0)
        {
            execlp(buf,buf,static_cast<char*>(0));
            cerr<<"couldn't execute: "<<buf<<endl;
            return 127;
        }


        if((pid==waitpid(pid,&status,0))<0)
            cerr<<"waitpid error"<<endl;

        cout<<"&& ";
    }

    return 0
}

void sig_int(int signo)
{
    cout<<"interrupt"<<signo<<endl;
    return;
}