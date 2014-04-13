// FileSystem2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Filesystem.h"
#include <iostream> 
#include <windows.h>
#include <vector>
#include <string>
#include <fstream>
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
	methods.insert(pair<wstring, void*(filesystem::*)(vector<wstring>)>(L"pwd", &filesystem::pwd));
	methods.insert(pair<wstring, void*(filesystem::*)(vector<wstring>)>(L"ls", &filesystem::ls));
	methods.insert(pair<wstring, void*(filesystem::*)(vector<wstring>)>(L"cd", &filesystem::cd));
	methods.insert(pair<wstring, void*(filesystem::*)(vector<wstring>)>(L"ln", &filesystem::ln));
}



void execute(wstring command) {
	wchar_t *context = NULL;
	wchar_t *p = new wchar_t[(command.length() + 1)*sizeof(wchar_t)];
	wcscpy_s(p,(command.length()+1)*sizeof(wchar_t), command.c_str());
	wchar_t *first_tok = wcstok_s(p, L" ", &context);//wcstok(p, L" ");

	map<wstring, void*(filesystem::*)(vector<wstring>)>::iterator iter;
	iter = methods.find(first_tok);
	if (iter != methods.end()){
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
	else {
		wprintf(L"%s %s\n",first_tok,L"is not valid command"); /////////////////////////////////check
	}
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

template<typename TIArch, typename TOArch, typename TClass>
void TestArch(const std::string & file, int flags, const TClass & cont)
{

	{ // —ериализуем
		std::ofstream ofs(file.c_str(), std::ios::out | flags);
		TOArch oa(ofs);
		// make_nvp создаЄт пару им€-значение, котора€ отразитс€ в XML
		// если не используем XML архив, то можно пару не создавать
		oa << boost::serialization::make_nvp("Test_Object", cont);
	}

	TClass newg;
	{ // ƒесериализуем
		std::ifstream ifs(file.c_str(), std::ios::in | flags);
		TIArch ia(ifs);
		ia >> boost::serialization::make_nvp("Test_Object", newg);
	}

	{ // ≈ще раз сериализуем, чтобы потом сравнить результаты двух сериализаций
		// и убедитьс€, что десериализации€ прошла корректно
		std::ofstream ofs((file + ".tmp").c_str(), std::ios::out | flags);
		TOArch oa(ofs);
		oa << boost::serialization::make_nvp("Test_Object", cont);
	}
}

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
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 2));
		wcout << filesys->getPwd().c_str();
		SetConsoleTextAttribute(hConsole, (WORD)((0 << 4) | 15));
		cout << ">>";
		getline(wcin, command);
		if (wcscmp(command.c_str(), L"exit"))
			execute(command);
		else
			break;
		
	}
	vector<wstring> params;
	params.push_back(L"");
	filesys->mkdir(params);
	params.erase(params.begin());
	params.push_back(path + L"/");
	filesys->rmdir(params);
	
	//delete filesys; ибо незачем

	return 0;
}

