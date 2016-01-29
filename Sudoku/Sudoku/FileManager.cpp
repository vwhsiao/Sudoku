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
	while (!file.eof())
	{
		int i;
		std::cin >> i;
		contents.push_back(i);
	}

	file.close();
	return contents;
}