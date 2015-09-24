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

            for(int k = 0; k < uvN; k++)
	            _file.read((char *)&v.uv[k], sizeof(vec2));

            uint32 tmp;
            for(int k = 0; k < 2; k++)
	            _file.read((char *)&tmp, 4);

            uint32 bindN;
	        _file.read((char *)&bindN, 4);
            for(int k = 0; k < bindN; k++)
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
    /*  
    char path[255];
    strcpy(path, "aries/");
    strcpy(path + 6, _path);
    strcpy(path + strlen(path) - 4, ".skel");

    cout << "[SAVE]" << path << endl;
    Util::mkdir(path);
    ofstream fout(path, ios::out|ios::binary);

    float dt = 1.0f/30;
    int frameN = _time / dt;
    fout.write((char *)&frameN, 4);

    int boneN = 60;
    fout.write((char *)&boneN, 4);

    float t = 0;
    for(int i = 0; i < frameN; i++, t += dt)
    {
        fout.write((char *)&dt, 4);

        for(int j = 0; j < boneN; j++)
        {
            KeyData data = getKeyData(j, t);
            fout.write((char *)&data, 10);
        }
    }

    fout.close();
    */
}














