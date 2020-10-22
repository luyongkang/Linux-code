#include<iostream>
#include<dirent.h>

using namespace std;

int main(int args,char* argv[])
{
    DIR* dp;
    dirent* dirp;
    if(args!=2)
        cerr<<"usage:ls directory_name"<<endl;

    if((dp = opendir(argv[1]))==nullptr)
    {
        cerr<<"can't open "<<argv[1]<<endl;
    }
    else
    {
        while((dirp=readdir(dp))!=nullptr)
        {
            cout<<dirp->d_name<<endl;
        }
    }

    system("date");
    return 0;
    
}
