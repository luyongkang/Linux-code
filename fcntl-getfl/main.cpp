#include <cstdlib>
#include <iostream>
#include <fcntl.h>

using namespace std;

int main(int argc, char *argv[])
{
    int val;
    if (argc < 2)
        cerr << "usage:debug <descriptor#>" << endl;
    if ((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0)
        cerr << "fcntl error for fd" << argv[1] << endl;

    switch (val & O_ACCMODE)
    {
    case O_RDONLY:
        cout << "read only";
        break;
    case O_WRONLY:
        cout << "write only";
        break;
    case O_RDWR:
        cout << "read&write";
        break;
    default:
        cerr << "unknow access mode" << endl;
    }

    if (val & O_APPEND)
        cout << " ,append";
    if (val & O_NONBLOCK)
        cout << " ,nonblocking";
    if (val & O_SYNC)
        cout << " ,synchronous writes";

#if !defined(_POSIX_C_SOURCE) && defined(O_FSYNC) && (O_FSYNC != O_SYNC)
    if (val & O_FSYNC)
        cout << " ,synchronous writes";
#endif
    cout << endl;
    return 0;
}
