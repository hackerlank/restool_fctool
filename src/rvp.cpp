#include "util.h"
#include "rvp.h"


Rvp::Rvp(const char* file)
{
	strcpy(_path, file);
	_file.open(filename, ios::in|ios::binary);

	//read head
	memset(&_head, 0, sizeof(_head));
	_file.seekg(0);
	_file.read((char *)&_head, sizeof(_head));

    PkgFile* files = new PkgFile[_head.filecount];

    char* p = (char *)files;
    int   n =  sizeof(PkgFile) * _head.filecount;
	_file.seekg(_head.tbloffset);
    _file.read(p, n);

    if(_head.encrypt == '1')
    {
        char* key = _head.version;
        char  max = strlen(_head.version) - 1;
        for(int i = 0; i < n; i++)
            for(int j = max; j >= 0; j--)
                p[i] ^= key[j];
    }

    for(int i = 0; i < _head.filecount; i++)
    {
        _list.push_back(files[i]);
    }
    delete[] files;
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
        PkgFile v = _list[i];
        if(v.weakuse == '0')
        {
            cout << "[WEAK]" << v.path << endl;
            continue;
        }

        strcpy(path, "out/");
        strcpy(path + 4, v.path);
        for(int t = 0; t < strlen(path); t++)
            if(path[t] == '\\')
                path[t] = '/';

        cout << "[SAVE]" << path << endl;

        if(v.zipsize == 0 && v.ziptype == 0)
        {
            Util::mkdir(path);
            ofstream fout(path, ios::out|ios::binary);
            char* buf = new char[v.rawsize];

	        _file.seekg(v.offset);
            _file.read(buf, v.rawsize);
            fout.write(buf, v.rawsize);
            fout.close();

            delete[] buf;
        }
        else
        {
            cout << "[ERROR]ZIP=============================================\n" << endl;
        }
    }
}







