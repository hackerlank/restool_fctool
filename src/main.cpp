#include <iostream>
#include <cmath>
#include "util.h"
#include "rvp.h"
#include "csf.h"

using namespace std;

//#define TestClass Rvp
//#define TestFiles "../data.rvp"

#define TestClass Csf
#define TestFiles "res/bao00.csf"



void work(const char *path)
{
    cout << "[Read]" << path << endl;
	TestClass tt(path);
    tt.save();
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


