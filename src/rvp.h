#ifndef __PKG_H__
#define __PKG_H__

#include "define.h"


#pragma pack(1)
typedef struct _RvpFile
{
    char   name[255]; 
    uint8  flag;
	uint32 offset;
    uint32 size1;
    uint32 size2;
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

	vector<RvpFile> _list;
};



#endif

