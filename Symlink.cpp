#include "stdafx.h"
#include <string>
#ifndef FILE_H
#define FILE_H
#include "File.h"
#endif
#include "Symlink.h"

symlink::symlink() : file() {}

file *symlink::getLinkedfile()
{
	return linked_file;
}

void symlink::setLinkedfile(file* f) {
	linked_file = f;
}