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
	if (file.is_open())
	{
		std::cout << "file is open" << std::endl;
	}
	std::vector<int> contents;
	char i;
	while (file.get(i))
	{

		//std::cout << contents.size() << std::endl;
		if (!isspace(i))
		{
			contents.push_back(i - 48);
		}
		
		if (file.eof())
		{
			break;
		}
		
	}
	for (int i = 0; i < contents.size(); i++)
	{
		std::cout << contents[i]<<" ";
	}
	std::cout << std::endl;
	file.close();
	return contents;
}