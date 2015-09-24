#ifndef __CAF_H__
#define __CAF_H__

#include "define.h"


#pragma pack(1)

typedef struct _KeyData
{
    vec3  pos;
    quat  rot;
    vec3  scale;
} KeyData;


typedef struct _KeyAnim
{
    float time;
    vec3  pos;
    quat  rot;
} KeyAnim;

typedef struct _CafAnim
{
    uint32 boneid;
    vector<KeyAnim> list;
} CafAnim;
#pragma pack()


class Caf
{
public:
	Caf(const char* file);
	~Caf();

	void save();

     KeyData getKeyData(int boneid, float time);
public:
	char 	 _path[255];
	ifstream _file;

    float           _time;
	vector<CafAnim> _list;
};



#endif

