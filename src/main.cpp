#include <iostream>
#include <cmath>
#include "util.h"
#include "rvp.h"

using namespace std;

#define TestClass Rvp
#define TestFiles "../data.rvp"



void work(const char *path)
{
    cout << "[Read]" << path << endl;
	TestClass tt(path);
}

int main(int argc, char* argv[])
{
    if(argc > 1)
    {
	    for(int i = 1; i < argc; i ++)
	    	work(argv[i]);
    }
    else
    {
	    work(TestFiles);
    }

    return 0;
}


