#ifndef __CMF_H__
#define __CMF_H__

#include "define.h"


#pragma pack(1)

typedef struct _CmfVert
{
    vec3 pos;
    vec3 normal;

    vec2 uv[4];

    uint32 boneid[4];
    float  weight[4];
} CmfVert;

typedef struct _CmfFace
{
    uint32 idx[3];
} CmfFace;

typedef struct _CmfMesh
{
    vector<CmfVert> vert;
    vector<CmfFace> face;
} CmfMesh;
#pragma pack()


class Cmf
{
public:
	Cmf(const char* file);
	~Cmf();

	void save();
public:
	char 	 _path[255];
	ifstream _file;

	vector<CmfMesh> _list;
};



#endif

