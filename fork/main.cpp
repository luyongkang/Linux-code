#include <unistd.h>
#include <iostream>
#include <string>
using namespace std;
int globvar = 6;
string str = "a write to stdout";

int main()
{
    int var;
    pid_t pid;

    var = 88;
    if (write(STDOUT_FILENO, str.c_str(), str.size()) != str.size())
    {
        cerr << "write error" << endl;
    }

    cout << "before fork" << endl;

    if ((pid = fork()) < 0)
    {
        cout << "fork error" << endl;
    }
    else if (pid == 0)
    {
        var++;
        globvar++;
    }
    else
    {
        sleep(2);
    }

    cout << "pid=" << getpid() << "  globvar=" << globvar << "  var=" << var << endl;
    cout << "ppid=" << getppid() << endl;
    return 0;
}