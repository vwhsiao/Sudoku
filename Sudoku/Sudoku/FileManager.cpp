#include "FileManager.h"

#include <iostream>
#include <fstream>
#include <vector>

FileManager::FileManager()
{
}

FileManager::~FileManager()
{
}

std::vector<int> FileManager::readFile(std::string filename)
{
	std::ifstream file;
	file.open(filename);

	std::vector<int> contents;
	int i;
	while (true)
	{
		file >> i;
		//this has to be done this way because of how file input is handled. the other way causes the last item to be repeated
		if (file.eof()) 
			break;

		contents.push_back(i);
	}
	
	file.close();
	return contents;
}