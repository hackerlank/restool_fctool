#include <zlib.h>
#include "util.h"
#include "rvp.h"


Rvp::Rvp(const char* file)
{
	strcpy(_path, file);
	_file.open(file, ios::in|ios::binary);

	_file.seekg(0, ios::end);
    uint32 total = _file.tellg();

    char head[7];
	_file.seekg(0, ios::beg);
	_file.read((char *)head, 7);

    uint32 offset;
	_file.read((char *)&offset, 4);
	_file.seekg(offset, ios::beg);

    while(_file.tellg() < total)
    {
        RvpFile f;
        memset((char*)&f, 0, sizeof(f));

        uint32 n;
	    _file.read((char *)&n, 4);
	    _file.read((char *)f.name, n);
	    _file.read((char *)&f.flag, 1);
	    _file.read((char *)&f.offset, 4);
	    _file.read((char *)&f.size1, 4);
	    _file.read((char *)&f.size2, 4);

        _list.push_back(f);

        //cout << f.name << " flag " << (int)f.flag << " " << f.offset << " " << f.size1 << " " << f.size2 << endl;
    }

    cout << _list.size() << endl;
}


Rvp::~Rvp()
{
	_file.close();
}


void Rvp::save()
{
    char path[255];
    for(int i = 0; i < _list.size(); i++)
    {
        RvpFile v = _list[i];
        

        strcpy(path, "out");
        strcpy(path + 3, v.name);
        for(int t = 0; t < strlen(path); t++)
            if(path[t] == '\\')
                path[t] = '/';


        char* buf = new char[v.size1];
	    _file.seekg(v.offset);
        _file.read(buf, v.size1);

        cout << "[SAVE]" << path << endl;
        Util::mkdir(path);
        ofstream fout(path, ios::out|ios::binary);
        if(v.flag == 0)
        {
            fout.write(buf, v.size1);
        }
        else
        {
            uLongf len = v.size2;
            Bytef* zip = new Bytef[len];
            int err = uncompress(zip, &len, (const Bytef*)buf, v.size1);
            if (err == Z_OK && len == v.size2)
            {
                fout.write((const char*)zip, len);
                delete[] zip;
            }
            else
            {
                delete[] zip;
                cerr << "uncompess error: " << err << '\n';
                exit(1);
            }
        }

        fout.close();

        delete[] buf;
    }
}







