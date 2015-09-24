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
    strcpy(path, _path);
    strcpy(path + strlen(_path) - 4, ".bone");

    cout << "[SAVE]" << path << endl;
    Util::mkdir(path);
    ofstream fout(path, ios::out|ios::binary);

    uint8 bn = (uint8)_list.size();
    fout.write((char *)&bn, 1);

    for(int i = 0; i < _list.size(); i++)
    {
        CsfBone b = _list[i];
        int8 bp = (int8)b.parent;
        fout.write((char *)&bp, 1);

        uint16 bs = strlen(b.name);
        fout.write((char *)&bs, 2);
        fout.write((char *)b.name, bs);


        //cout << b.pos.x << " " << b.pos.y << " " << b.pos.z << endl;
        //cout << b.invpos.x << " " << b.invpos.y << " " << b.invpos.z << endl;
        //cout << b.rot.x << " " << b.rot.y << " " << b.rot.z << " " << b.rot.w << endl;
        //cout << b.invrot.x << " " << b.invrot.y << " " << b.invrot.z << " " << b.invrot.w << endl;

        cout << "====================================" << endl;
	    //Matrix4f m1 = Matrix4f::TransMat(b.pos.x, b.pos.y, b.pos.z);
	    //Matrix4f m2(Quaternion(b.rot.x, b.rot.y, b.rot.z, b.rot.w));
        //Matrix4f m3 = m1 * m2;
        //m3.print();
        //m3.inverted().print();

        getMatById(i)->print();
        //getMatByIdInv(i)->print();

	    Matrix4f m4 = Matrix4f::TransMat(b.invpos.x, b.invpos.y, b.invpos.z);
	    Matrix4f m5(Quaternion(b.invrot.x, b.invrot.y, b.invrot.z, b.invrot.w));
        Matrix4f m6 = m5 * m4;
        m6.inverted().print();
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


Matrix4f* Csf::getMatByIdInv(int id)
{
    CsfBone b = _list[id];
    if(b.invmat)
        return b.invmat;


    Matrix4f m1 = Matrix4f::TransMat(b.invpos.x, b.invpos.y, b.invpos.z);
	Matrix4f m2(Quaternion(b.invrot.x, b.invrot.y, b.invrot.z, b.invrot.w));
    b.invmat = new Matrix4f();
    (*(b.invmat)) = m1 * m2;

    if(b.parent > -1)
    {
        //(*b.invmat) = (*b.invmat) * (*getMatById(b.parent));
        (*b.invmat) = (*getMatByIdInv(b.parent)) * (*b.invmat);
    }
    return b.invmat;
}









