// FileSystem2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Filesystem.h"
#include <iostream> 
#include <windows.h>
#include <vector>
#include <string>
#include <map>
#include <stdlib.h>
using namespace System;
using namespace System::IO;
using namespace std;


filesystem* filesys;
map <wstring, void*(filesystem::*)(vector<wstring>)> methods;
wstring path;

void associate() {
	//map<wstring, void*(filesystem::*)(vector<wstring>)>::iterator iter = methods.begin();
	//void*(filesystem::*pf)(vector<wstring>) = &filesystem::mkdir;
	methods.insert(pair<wstring, void*(filesystem::*)(vector<wstring>)>(L"mkdir", &filesystem::mkdir));//filesys->mkdir(path)));
	methods.insert(pair<wstring, void*(filesystem::*)(vector<wstring>)>(L"touch", &filesystem::touch));
	methods.insert(pair<wstring, void*(filesystem::*)(vector<wstring>)>(L"mv", &filesystem::mv));
	methods.insert(pair<wstring, void*(filesystem::*)(vector<wstring>)>(L"cp", &filesystem::cp));
	methods.insert(pair<wstring, void*(filesystem::*)(vector<wstring>)>(L"rm", &filesystem::rm));
	methods.insert(pair<wstring, void*(filesystem::*)(vector<wstring>)>(L"edit", &filesystem::edit));
	methods.insert(pair<wstring, void*(filesystem::*)(vector<wstring>)>(L"rmdir", &filesystem::rmdir));
	methods.insert(pair<wstring, void*(filesystem::*)(vector<wstring>)>(L"cpdir", &filesystem::cpdir));
	methods.insert(pair<wstring, void*(filesystem::*)(vector<wstring>)>(L"mvdir", &filesystem::mvdir));
}



void execute(wstring command) {
	wchar_t *context = NULL;
	wchar_t *p = new wchar_t[(command.length() + 1)*sizeof(wchar_t)];
	wcscpy_s(p,(command.length()+1)*sizeof(wchar_t), command.c_str());
	wchar_t *first_tok = wcstok_s(p, L" ", &context);//wcstok(p, L" ");
	map<wstring, void*(filesystem::*)(vector<wstring>)>::iterator iter;
	iter = methods.find(first_tok);
	vector<wstring> params;
	p = wcstok_s(NULL, L" ", &context);
	while (p != NULL) {
		params.push_back(p);
		p = wcstok_s(NULL, L" ", &context);
	}
	//vector<wstring> params = { L"papka" };
	(*filesys.*(iter->second))(params);
	delete[] p;
}

/*class A {
public:
	A(A &obj) : st(obj.st) {
		//st = obj.st;
		printf("c1\n");
	}
	A() : st("abcd") {
		printf("c2\n");
		//st = "abcd";
	}
	
	/*~A() {
		printf("destructor of A is called");
	}*/
/*	string st;
	
};

class B {
public:
	B() {}
	int i = 4;
};*/


int _tmain(int argc, _TCHAR* argv[])
{
	wstring path = L"c:/myFilesystem";
	filesys = new filesystem(path);
	associate();
	if (CreateDirectory(path.c_str(), NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{
		// CopyFile(...)
	}

	/*A one;
	A two = one;
	two.st = "efgh";
	printf("one----%s\n", one.st);
	printf("two----%s\n", two.st);
	//удалить и посмотреть, что будет
	A *c = new A();
	delete c; */

	filesys = new filesystem(path);
	wstring command;
	while (true) {
		wcout << ">>";
		getline(wcin, command);
		if (wcscmp(command.c_str(), L"exit"))
			execute(command);
		else
			break;
		
	}
	/*vector<wstring> params;
	params.push_back(path);
	filesys->rmdir(params);*/ //неверно, т.к. этот путь отсутствует в фс

	delete filesys;
	cin.ignore();

	return 0;
}

