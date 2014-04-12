#include "stdafx.h"
#include <string>
#include <iostream>
#ifndef FILE_H
#define FILE_H
#include "File.h"
#endif
#ifndef DIR_H
#define DIR_H
#include "Dir.h"
#endif
#include <ctime>
#include "Filesystem.h"
#include <vector>
#include <fstream>
#include <algorithm> 
#include <windows.h>
#include <shlwapi.h>
#pragma comment (lib, "shlwapi")
using namespace std;

wstring root;

filesystem::filesystem(wstring _current_path){
	current_path = _current_path;
	root = _current_path;
	for_pwd = L"/";
}

wstring filesystem::getCurrenpPath() {
	return current_path;
}

void filesystem::setCurrentPath(wstring path) {
	current_path = path;
}

wstring filesystem::getPwd() {
	return for_pwd;
}

void filesystem::setPwd(wstring _path) {
	for_pwd = _path;
}

wstring filesystem::createPath(wstring param) {
	wstring path = current_path;
	if (param[0] != '/') {
		path += L"/";
	}
	path += param;
	return path;
}

void* filesystem::mkdir(vector<wstring> params) {
	//создать по полному пути
	//создать в данной директории
	//создать в данной и еще куча вложенных. хотя это, кажется, то же самое, что и предыдущее

	wstring temp = createPath(params[0]);
	dir* d = new dir();
	d->setSize(0);
	d->setTime_Changed(time(0));
	d->setTime_Created(time(0));
	d->setPAth(temp);
	CreateDirectory(temp.c_str(), NULL);
	//fs.push_front(f);
	fs.push_back(d);
	//dir *ret = &d;
	return nullptr;
}

void* filesystem::touch(vector<wstring> params) {
	wstring temp = createPath(params[0]);
	file* f = new file();
	f->setSize(0);
	f->setTime_Changed(time(0));
	f->setTime_Created(time(0));
	f->setPAth(temp);
	ofstream ofs(temp);
	ofs.close();
	fs.push_back(f);
	file *ret = f;
	return ret;
}

int findbyname(vector<file*> fs, wstring name) {
	for (int i = 0; i < fs.size(); i++) {
		if (!wcscmp(fs[i]->getPath().c_str(), name.c_str())) {
			return i;
		}
	}
	printf("file or dir with such name does not exist"); //throw exception
	return fs.size() + 1;
}

void* filesystem::mv(vector<wstring> params) { //params[0] - old, params[1] - new
	wstring path_old = createPath(params[0]);
	wstring path_new = createPath(params[1]);
	ifstream src(path_old);
	ofstream dst(path_new);
	dst << src.rdbuf();
	src.close();
	dst.close();
	DeleteFile(path_old.c_str());
	fs[findbyname(fs, path_old)]->setPAth(path_new);
	file* ret = fs[findbyname(fs, path_new)];
	return &ret;
}

void* filesystem::cp(vector<wstring> params) {
	wstring path_old = createPath(params[0]);
	wstring path_new = createPath(params[1]);
	path_new += params[0];
	ifstream src(path_old);
	ofstream dst(path_new);
	dst << src.rdbuf();
	src.close();
	dst.close();
	file* f = new file();
	int pos = findbyname(fs, path_old);
	f->setSize(fs[pos]->getSize());
	f->setPAth(path_new);
	f->setTime_Changed(time(0));
	f->setTime_Created(time(0));
	fs.push_back(f);
	return &f;
}

void* filesystem::rm(vector<wstring> params) {
	wstring path = createPath(params[0]);
	int pos = findbyname(fs, path);
	vector<file*>::iterator iter = fs.begin();
	fs.erase(iter + pos);
	DeleteFile(path.c_str());
	return nullptr;
}

void* filesystem::edit(vector<wstring> params) {
	wstring path = createPath(params[0]);
	ShellExecute(0, NULL, _T("notepad.exe"), path.c_str(), NULL, SW_SHOWDEFAULT);
	int pos = findbyname(fs, path);
	file* ret = fs[pos];
	return &ret;
}


void* filesystem::rmdir(vector<wstring> params) {
	wstring path = createPath(params[0]);
	vector<wstring> indices;
	vector<file*> children;
	wstring name;
	for (int i = 0; i < fs.size(); i++) {
		name = fs[i]->getPath();
		if (wcsstr(name.c_str(), path.c_str()) != nullptr) {
			children.push_back(fs[i]);
			indices.push_back(name);
		}
	}
	for (int j = 0; j < children.size(); j++) {
		name = children[j]->getPath();
		printf("%s\n", typeid(*children[j]).name());
		if (dynamic_cast<dir*>(children[j]) == nullptr) {
			DeleteFile(name.c_str());
		}
	}
	for (int j = 0; j < children.size(); j++) {
		name = children[j]->getPath();
		if (dynamic_cast<dir*>(children[j]) != nullptr) {
			RemoveDirectory(name.c_str());
		}
	}
	RemoveDirectory(path.c_str());
	vector<file*>::iterator iter;
	for (int i = 0; i < indices.size(); i++) {
		iter = fs.begin();
		int pos = findbyname(fs, indices[i]);
		fs.erase(iter + pos);
	}
	return nullptr;
}

int findlastentry(wstring word, wchar_t symbol) {
	for (int i = word.length() - 1; i != 0; i--) {
		if (word[i] == symbol)
			return i;
	}
	throw ("There is no entry of such symbol");
	return 6;
}


void* filesystem::cpdir(vector<wstring> params) {
	//пройтись по фс, если есть вхождение, то включить в children
	//вырезать имена, оставляя имя папки
	wstring old_path = createPath(params[0]);
	wstring new_path = createPath(params[1]);

	vector<file*> children;
	vector<wstring> temp_names;
	wstring trimmed_name;
	wstring name;
	for (int i = 0; i < fs.size(); i++) {
		name = fs[i]->getPath();
		if (wcsstr(name.c_str(), old_path.c_str()) != nullptr) {
			children.push_back(fs[i]);
			//trimmed_name = name.substr(old_path-позиция посленего слеша в old_path); т.е. найти последнее вхождение слеша в олд_пас
			trimmed_name = name.substr(findlastentry(old_path, '/')); //проверить, мать вашу!!
			temp_names.push_back(trimmed_name);
		}
	}
	//создать их физически
	for (int i = 0; i < children.size(); i++) {
		if (dynamic_cast<dir*>(children[i]) != nullptr) {
			CreateDirectory((new_path + temp_names[i]).c_str(), NULL);
		}
		else {
			ifstream src(children[i]->getPath());
			ofstream dst((new_path + temp_names[i]).c_str());
			dst << src.rdbuf();
			src.close();
			dst.close();
		}
	}
	//занести новые файлы в фс
	

	return nullptr;
}

int count_slashes(wstring path) {
	int count = 0;
	for (int i= 0; i < path.length(); i++) {
		i = path.find(L"/", i);
		if (i != string::npos) {
			++count;
		}
		else {
			break;
		}
	}
	return count;
}

/*bool operator <(file &one, file &two) {
	return count_slashes(one.getPath()) < count_slashes(two.getPath());
}*/

bool compare(wstring &one, wstring &two) {
	return count_slashes(one) < count_slashes(two);
}

void* filesystem::mvdir(vector<wstring> params) {
	wstring old_path = createPath(params[0]);
	wstring new_path = createPath(params[1]);
	vector<file*> children;
	vector<wstring> temp_names;
	wstring name;
	wstring trimmed_name;
	for (int i = 0; i < fs.size(); i++) {
		name = fs[i]->getPath();
		if (wcsstr(name.c_str(), old_path.c_str()) != nullptr) {
			children.push_back(fs[i]); //? один и тот же ли порядок 
			trimmed_name = name.substr(old_path.length()); //вынести наружу: сейчас не могу, голова уже не соображает
			temp_names.push_back(trimmed_name);
		}
	}
	sort(temp_names.begin(), temp_names.end(), compare);
	for (int i = 0; i < children.size(); i++) {
		if (dynamic_cast<dir*>(children[i]) != nullptr) {
			CreateDirectory((new_path + temp_names[i]).c_str(), NULL);
		}
		else {
			ifstream src(children[i]->getPath());
			ofstream dst((new_path + temp_names[i]).c_str());
			dst << src.rdbuf();
			src.close();
			dst.close();
		}
	}
	//удаление папки из старого места
	//vector<wstring> param;
	//param.push_back(old_path);
	//rmdir(param); //не работает!!!
	
	//замена старых имён на новые 
	for (int i = 0; i < children.size(); i++) {
		int pos_change = findbyname(fs, children[i]->getPath());
		fs[pos_change]->setPAth(new_path + temp_names[i]);
	}
	
	return nullptr;
}

void* filesystem::pwd(vector<wstring> params) {
	wprintf(L"%s\n", getPwd().c_str());
	return nullptr;
}

void* filesystem::ls(vector<wstring> params) {
	bool params_empty = params.empty();
	wstring path;
	if (!params_empty)
		path = createPath(params[0]);
	else
		path = current_path;
	vector<file*> children;
	wstring name;
	wstring trimmed_name;
	vector<wstring> temp_names;

	size_t slashes_in_path = count_slashes(path);
	for (int i = 0; i < fs.size(); i++) {
		name = fs[i]->getPath();
		if (wcsstr(name.c_str(), path.c_str()) != nullptr && (count_slashes(name) == slashes_in_path + 1)) {
			children.push_back(fs[i]); //? один и тот же ли порядок 
			trimmed_name = name.substr(path.length() + 1); //вынести наружу: сейчас не могу, голова уже не соображает
			temp_names.push_back(trimmed_name);
		}
	}
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int j = 0; j < children.size(); j++) {
		if (dynamic_cast<dir*>(children[j]) != nullptr) {
			SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 3));
		}
		else {
			SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 7));
		}
		wprintf(L"%s\n", temp_names[j].c_str());
	}
	/*size_t counter = 0;
	const size_t MAX_IN_ROW = 4;
	for (int j = 0; j < temp_names.size(); j++) {
		if (counter != MAX_IN_ROW) {
			wprintf(L"%s   ", temp_names[j].c_str());
			counter++;
		}
		else {
			counter = 0;
			wprintf(L"\n%s   ", temp_names[j].c_str());
		}
	}
	wprintf(L"\n");*/
	return nullptr;
}

void* filesystem::cd(vector<wstring> params) {
	//путь из корня
	//путь из текущей папки
	//..
	//../..
	wstring command = params[0];
	if (!wcscmp(command.c_str(), L"..") && wcscmp(current_path.c_str(), root.c_str())) {
		int i = findlastentry(current_path, '/');
		wstring temp = current_path.substr(0, i);
		current_path = temp;
		for_pwd = current_path.substr(root.length());
		for_pwd += L"/";
	}
	else if (!wcscmp(command.c_str(), L"../..")) {
		current_path = root;
		for_pwd = L"/";
	}
	else {
		wstring new_path = createPath(params[0]);
		current_path = new_path;
		for_pwd = current_path.substr(root.length());//=current path without root
	}

	return nullptr;
}