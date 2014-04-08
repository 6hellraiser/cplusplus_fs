#include "stdafx.h"
#ifndef FILE_H
#define FILE_H
#include "File.h"
#endif
#include <string>

/*file::file(size_t _size, time_t _time_created, time_t _time_changed, wstring _path) {
	size = _size;
	time_created = _time_created;
	time_changed = _time_changed;
	path = _path;
}*/

file::file() {}

file::~file() {

}

size_t file::getSize() {
	return size;
}

void file::setSize(size_t size) {
	file::size = size;
}

wstring file::getPath() {
	return path;
}

void file::setPAth(wstring path) {
	file::path = path;
}

time_t file::getTime_Created() {
	return time_created;
}

void file::setTime_Created(time_t time) {
	time_created = time;
}

time_t file::getTime_Changed() {
	return time_changed;
}

void file::setTime_Changed(time_t time) {
	time_changed = time;
}