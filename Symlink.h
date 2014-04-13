#include "stdafx.h"
#ifndef FILE_H
#define FILE_H
#include "File.h"
#endif
#include <string>

class symlink : public file {
private:
	file* linked_file;

public:
	symlink();

	file* getLinkedfile();
	void setLinkedfile(file* f);
};