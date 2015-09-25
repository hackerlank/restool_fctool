#include <zlib.h>
#include "util.h"
#include "csf.h"


Csf::Csf(const char* file)
{
	strcpy(_path, file);
	_file.open(file, ios::in|ios::binary);

    char head[8];
	_file.seekg(0, ios::beg);
	_file.read((char *)head, 8);

    uint32 total;
	_file.read((char *)&total, 4);

    for(int i = 0; i < total; i++)
    {
        CsfBone b;
        memset((char*)&b, 0, sizeof(b));

        uint32 n;
	    _file.read((char *)&n, 4);
	    _file.read((char *)b.name, n);

	    _file.read((char *)&b.pos, sizeof(vec3));
	    _file.read((char *)&b.rot, sizeof(quat));

	    _file.read((char *)&b.invpos, sizeof(vec3));
	    _file.read((char *)&b.invrot, sizeof(quat));

	    _file.read((char *)&b.parent, 4);
	    _file.read((char *)&n, 4);
        for(int j = n; j > 0; j--)
        {
	        _file.read((char *)&n, 4);
            b.children.push_back(n);
        }
        _list.push_back(b);

        cout << i << " " << b.name << " " << b.parent << " " << b.children.size() << endl;
    }
}


Csf::~Csf()
{
	_file.close();
}


void Csf::save()
{
    char path[255];
    strcpy(path, "aries/");
    strcpy(path + 6, _path);
    strcpy(path + strlen(path) - 4, ".bone");

    cout << "[SAVE]" << path << endl;
    Util::mkdir(path);
    ofstream fout(path, ios::out|ios::binary);

    uint8 bn = (uint8)_list.size();
    fout.write((char *)&bn, 1);

    for(int i = 0; i < _list.size(); i++)
    {
        CsfBone b = _list[i];

        int8 id = (int8)i;
        fout.write((char *)&id, 1);

        int8 bp = (int8)b.parent;
        fout.write((char *)&bp, 1);

        uint8 bs = strlen(b.name);
        fout.write((char *)&bs, 1);
        fout.write((char *)b.name, bs);


	    Matrix4f m4 = Matrix4f::TransMat(b.invpos.x, b.invpos.y, b.invpos.z);
	    Matrix4f m5(Quaternion(b.invrot.x, b.invrot.y, b.invrot.z, b.invrot.w));
        Matrix4f m6 = m5 * m4;

        //cout << "====================================" << endl;
        //m6.print();
        //getMatById(i)->inverted().print();

        fout.write((char *)m6.x, 4 * 16);
    }

    fout.close();
}


Matrix4f* Csf::getMatById(int id)
{
    CsfBone b = _list[id];
    if(b.mat)
        return b.mat;


    Matrix4f m1 = Matrix4f::TransMat(b.pos.x, b.pos.y, b.pos.z);
	Matrix4f m2(Quaternion(b.rot.x, b.rot.y, b.rot.z, b.rot.w));
    b.mat = new Matrix4f();
    (*(b.mat)) = m2 * m1;

    if(b.parent > -1)
    {
        (*b.mat) = (*b.mat) * (*getMatById(b.parent));
        //(*b.mat) = (*getMatById(b.parent)) * (*b.mat);
    }
    return b.mat;
}











