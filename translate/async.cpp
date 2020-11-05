#include<iostream>
#include<ctype.h>
#include<fcntl.h>
#include<aio.h>
#include<errno.h>

using namespace std;

char translate(char c);

char translate(char c)
{
	if(isalpha(c))
	{
		if(c>='n')
		{
			c -= 13;
		}
		else if(c>='a')
		{
			c += 13;
		}
		else if(c>='N')
		{
			c -= 13;
		}
		else
		{
			c += 13;
		}
	}
}
