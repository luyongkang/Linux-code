#include<iostream>
#include<ctype.h>
using namespace std;
int main()
{
	int c;
	while((c=getchar())!=EOF)
	{
		if(isupper(c))
		{
			c = tolower(c);
		}
		else if(islower(c))
		{
			c = toupper(c);
		}
		if(putchar(c)==EOF)
		{
			cerr << "output error" << endl;
		}
		if(c=='\n')
		{
			fflush(stdout);
		}
	}
	return 0;
}