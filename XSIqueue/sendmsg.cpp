#include<memory.h>
#include<iostream>
#include<sys/msg.h>
#include<unistd.h>

#define MAXMSZ 512

using namespace std;
struct mymesg
{
	long mtype;
	char mtext[MAXMSZ];
};

int main(int argc,char* argv[])
{
	key_t key;
	long qid;
	size_t nbytes;
	mymesg m;

	if(argc!=3)
	{
		cout << "usage: sendmsg KEY message" << endl;
		exit(1);
	}
	key = strtol(argv[1],nullptr,0);
	if((qid=msgget(key,0))<0)
	{
		cerr << "can't open queue key " << argv[1] << endl;
		exit(1);
	}

	memset(&m, 0, sizeof(m));
	strncpy(m.mtext, argv[2], MAXMSZ - 1);
	nbytes = strlen(m.mtext);
	m.mtype = 1;
	if(msgsnd(qid,&m,nbytes,0)<0)
	{
		cerr << "can't send message" << endl;
	}
	exit(0);
}