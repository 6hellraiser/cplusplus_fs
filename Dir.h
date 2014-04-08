#include "stdafx.h"
#ifndef FILE_H
#define FILE_H
#include "File.h"
#endif
#include <string>
using namespace std;

class dir: public file {
public:
	dir(size_t size, wstring path, time_t time_created, time_t time_changed);
	dir();
};