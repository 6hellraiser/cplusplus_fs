#include "stdafx.h"
#include <windows.h>
#include <string>
using namespace std;

class file {
private:
	size_t size;
	time_t time_created;
	time_t time_changed;
	wstring path;

public:
	size_t getSize();
	void setSize(size_t newsize);

	time_t getTime_Created();
	void setTime_Created(time_t newtime);

	time_t getTime_Changed();
	void setTime_Changed(time_t newtime);

	wstring getPath();
	void setPAth(wstring newpath);

	//file(size_t size, time_t time_created, time_t time_changed, wstring path);
	file();
	virtual ~file();
};