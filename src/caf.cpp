#include <zlib.h>
#include "util.h"
#include "caf.h"


Caf::Caf(const char* file)
{
	strcpy(_path, file);
	_file.open(file, ios::in|ios::binary);

    char head[8];
	_file.seekg(0, ios::beg);
	_file.read((char *)head, 8);

	_file.read((char *)&_time, 4);
    cout << "AnimTime " << _time << endl;


    uint32 total;
	_file.read((char *)&total, 4);

    for(int i = 0; i < total; i++)
    {
        CafAnim b;
        memset((char*)&b, 0, sizeof(b));

	    _file.read((char *)&b.boneid, 4);

        uint32 keys;
	    _file.read((char *)&keys, 4);

        for(int j = 0; j < keys; j++)
        {
            KeyAnim k;
	        _file.read((char *)&k.time, 4);
	        _file.read((char *)&k.pos, sizeof(vec3));
	        _file.read((char *)&k.rot, sizeof(quat));
            //cout << k.time << endl;
            b.list.push_back(k);
        }
        _list.push_back(b);
    }
}


Caf::~Caf()
{
	_file.close();
}


void Caf::save()
{
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
}



KeyData Caf::getKeyData(int boneid, float time)
{
    KeyData d;
    memset((char*)&d, 0, sizeof(d));
    d.scale.x = 1.0f;
    d.scale.y = 1.0f;
    d.scale.z = 1.0f;

    for(int i = 0; i < _list.size(); i++)
    {
        CafAnim anim = _list[i];
        if(anim.boneid == boneid)
        {
            for(int j = 0; j < anim.list.size(); j++)
            {
                KeyAnim k2 = anim.list[j];
                if(time < k2.time)
                {
                    KeyAnim k1 = anim.list[j-1];

                    float k = (time - k1.time)/(k2.time - k1.time);
                    d.pos.x = k1.pos.x * (1.0f - k) + k2.pos.x * k;
                    d.pos.y = k1.pos.y * (1.0f - k) + k2.pos.y * k;
                    d.pos.z = k1.pos.z * (1.0f - k) + k2.pos.z * k;

                    d.rot.x = k1.rot.x * (1.0f - k) + k2.rot.x * k;
                    d.rot.y = k1.rot.y * (1.0f - k) + k2.rot.y * k;
                    d.rot.z = k1.rot.z * (1.0f - k) + k2.rot.z * k;
                    d.rot.w = k1.rot.w * (1.0f - k) + k2.rot.w * k;

                    return d;
                }
            }
        }
    }

    return d;
}












