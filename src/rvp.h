#ifndef __PKG_H__
#define __PKG_H__

#include "define.h"


#pragma pack(4)
typedef struct _RvpHead
{
    char   filexx[7]; 
	uint32 offset;
} RvpHead;

#pragma pack()


class Rvp
{
public:
	Rvp(const char* file);
	~Rvp();

	void save();
public:
	char 	 _path[255];
	ifstream _file;

	RvpHead         _head;
	vector<PkgFile> _list;
};



#endif

