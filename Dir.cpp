#include "stdafx.h"
#include <string>
#include "Dir.h"



dir::dir(size_t size, wstring path, time_t time_created, time_t time_changed) : file() {
	dir::setSize(size);
	dir::setPAth(path);
	dir::setTime_Created(time_created);
	dir::setTime_Changed(time_changed);
}

dir::dir() {}
