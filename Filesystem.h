#include "stdafx.h"
#include <string>
#include <vector>
#ifndef DIR_H
#define DIR_H
#include "Dir.h"
#endif
#ifndef FILE_H
#define FILE_H
#include "File.h"
#endif
#include <vector>
using namespace std;

class filesystem {
private:
	wstring current_path; //полный путь
	vector<file*> fs;
	wstring createPath(wstring param);
	
public:
	filesystem(wstring _current_path);
	wstring getCurrenpPath();
	void setCurrentPath(wstring _path);

	void* mkdir(vector<wstring> params);
	void* touch(vector<wstring> params);
	void* mvdir(vector<wstring> params); //rename; add to map
	void* mv(vector<wstring> params); //rename
	void* cpdir(vector<wstring> params);
	void* cp(vector<wstring> params);
	void* edit(vector<wstring> params);
	void* rmdir(vector<wstring> params);
	void* rm(vector<wstring> params);
};