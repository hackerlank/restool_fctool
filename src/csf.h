#ifndef __CSF_H__
#define __CSF_H__

#include "define.h"


#pragma pack(1)
typedef struct _CsfBone
{
    char name[255]; 

    vec3 pos;  
    quat rot;

    vec3 invpos;
    quat invrot;

    int32 parent;
    vector<int32> children;

    Matrix4f* mat;
} CsfBone;
#pragma pack()


class Csf
{
public:
	Csf(const char* file);
	~Csf();

	void save();

    Matrix4f* getMatById(int id);
public:
	char 	 _path[255];
	ifstream _file;

	vector<CsfBone> _list;
};



#endif

