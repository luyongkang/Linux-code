#include <iostream>
#include <sys/stat.h>
using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "need parms" << endl;
        return 0;
    }

    struct stat buf;
    int i = 1;
    for (int i = 1; i < argc; ++i)
    {
        cout << argv[i] << " ";
        if (lstat(argv[i], &buf) < 0)
        {
            cerr << "lstat error" << endl;
            continue;
        }

        if (S_ISREG(buf.st_mode))
            cout << "is regular" << endl;
        else if (S_ISCHR(buf.st_mode))
            cout << "is character special" << endl;
        else if (S_ISDIR(buf.st_mode))
            cout << "is directory" << endl;
        else if (S_ISBLK(buf.st_mode))
            cout << "is block special" << endl;
        else if (S_ISFIFO(buf.st_mode))
            cout << "is fifo" << endl;
        else if (S_ISLNK(buf.st_mode))
            cout << "is symbolic link" << endl;
        else if (S_ISSOCK(buf.st_mode))
            cout << "is socket" << endl;
        else
        {
            cout << "is unknow mode" << endl;
        }
    }

    return 0;
}
