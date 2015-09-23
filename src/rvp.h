#ifndef __PKG_H__
#define __PKG_H__

#include "define.h"


#pragma pack(4)
typedef struct _RvpHead
{
    char   filexx[7]; 
	uint32 offset;
} RvpHead;

typedef struct _RvpFile
{
    char   name[7]; 
    uint8  flag;
    uint32 size1;
    uint32 size2;
	uint32 offset;
} RvpFile;
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
	vector<RvpFile> _list;
};



#endif

