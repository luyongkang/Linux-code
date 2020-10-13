#include <iostream>
#include <stack>
#include <dirent.h>
#include <sys/stat.h>
#include <string>
#include <cstring>
using namespace std;

const char *self = ".";
const char *up = "..";

int main(int args, char *argv[])
{

    if (args != 2)
    {
        cerr << "parm error" << endl;
        return 0;
    }
    struct stat buf;
    if (stat(argv[1], &buf) < 0)
    {
        cerr << argv[1] << "stat error" << endl;
        return 0;
    }

    if (!S_ISDIR(buf.st_mode))
    {
        cerr << argv[1] << " is not a dir" << endl;
        return 0;
    }

    DIR *p_dir;
    struct dirent *p_entry;

    stack<DIR *> s;
    stack<string> paths;
    string path(argv[1]);
    if ((p_dir = opendir(argv[1])) == nullptr)
    {
        cerr << "opendir error" << endl;
        return 0;
    }
    s.push(p_dir);
    paths.push(string(argv[1]));
    while (!s.empty())
    {
        p_dir = s.top();
        path = paths.top();
        while ((p_entry = readdir(p_dir)) != nullptr)
        {
            cout << string(s.size() * 4, ' ') << p_entry->d_name << endl;
            if (p_entry->d_type == DT_DIR && (strcmp(p_entry->d_name, self) != 0) && (strcmp(p_entry->d_name, up) != 0))
            {
                path = path + '/' + p_entry->d_name;
                if ((p_dir = opendir(path.c_str())) == nullptr)
                {
                    cerr << "opendir error" << endl;
                    return 0;
                }
                s.push(p_dir);
                paths.push(path);
                break;
            }
        }

        if (p_entry == nullptr)
        {
            s.pop();
            paths.pop();
            closedir(p_dir);
        }
    }

    return 0;
}