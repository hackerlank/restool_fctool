#include <zlib.h>
#include "util.h"
#include "cmf.h"


Cmf::Cmf(const char* file)
{
	strcpy(_path, file);
	_file.open(file, ios::in|ios::binary);

    char head[8];
	_file.seekg(0, ios::beg);
	_file.read((char *)head, 8);

    uint32 total;
	_file.read((char *)&total, 4);

    cout << "mesh: " << total << endl;

    for(int i = 0; i < total; i++)
    {
        CmfMesh b;
        memset((char*)&b, 0, sizeof(b));

        uint32 uvFlag;
	    _file.read((char *)&uvFlag, 4);
        uint32 vertN;
	    _file.read((char *)&vertN, 4);
        uint32 faceN;
	    _file.read((char *)&faceN, 4);
        uint32 ukn1;
	    _file.read((char *)&ukn1, 4);
        uint32 ukn2;
	    _file.read((char *)&ukn2, 4);
        uint32 uvN;
	    _file.read((char *)&uvN, 4);

        //cout << "uv " << uvFlag << " uvNum " << uvN << " vertN " << vertN << endl; 

        for(int j = 0; j < vertN; j++)
        {
            CmfVert v;
	        _file.read((char *)&v.pos, sizeof(vec3));
	        _file.read((char *)&v.normal, sizeof(vec3));

            float tmp;
            for(int k = 0; k < 2; k++)
            {
	            _file.read((char *)&tmp, 4);
            }

            v.uvN = uvN;
            for(int k = 0; k < uvN; k++)
            {
	            _file.read((char *)&v.uv[k], sizeof(vec2));
                v.uv[k].y = 1.0f - v.uv[k].y;
            }

	        _file.read((char *)&v.bindN, 4);
            for(int k = 0; k < v.bindN; k++)
            {
	            _file.read((char *)&v.boneid[k], 4);
	            _file.read((char *)&v.weight[k], 4);
            }

            b.vert.push_back(v);
        }

        for(int j = 0; j < faceN; j++)
        {
            CmfFace f;
	        _file.read((char *)&f, sizeof(f));
            b.face.push_back(f);
        }
        _list.push_back(b);
    }
}


Cmf::~Cmf()
{
	_file.close();
}


void Cmf::save()
{
    for(int i = 0; i < _list.size(); i++)
    {
        char path[255];
        strcpy(path, "aries/");
        strcpy(path + 6, _path);
        if(i == 0)
        {
            strcpy(path + strlen(path) - 4, ".skin");
        }else
        {
            int p = strlen(path) - 4;
            path[p] = (char)(48 + i);
            strcpy(path + p + 1, ".skin");
        }

        cout << "[SAVE]" << path << endl;
        Util::mkdir(path);
        ofstream fout(path, ios::out|ios::binary);


        CmfMesh m = _list[i];

        uint16 vertN = (uint16)m.vert.size();
        fout.write((char *)&vertN, 2);
        for(int j = 0; j < vertN; j++)
        {
            CmfVert v = m.vert[j];
            fout.write((char *)&v.pos, sizeof(vec3));
            fout.write((char *)&v.uv[0], sizeof(vec2));

            uint8 bindN = (uint8)v.bindN;
            fout.write((char *)&bindN, 1);
            for(int k = 0; k < v.bindN; k++)
            {
                uint8 boneId = (uint8)v.boneid[k];
                fout.write((char *)&boneId, 1);
                fout.write((char *)&(v.weight[k]), 4);
            }
        }

        uint16 faceN = (uint16)m.face.size();
        fout.write((char *)&faceN, 2);
        for(int j = 0; j < faceN; j++)
        {
            for(int k = 0; k < 3; k++)
            {
                uint16 ff = (uint16)m.face[j].idx[k];
                fout.write((char *)&ff, 2);
            }
        }

        fout.close();
    }
}














