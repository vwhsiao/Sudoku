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
	char i;
	while (true)
	{
		file >> i;
		//this has to be done this way because of how file input is handled. the other way causes the last item to be repeated
		if (file.eof()) 
		{
			break;
		}

		if (!isspace(i))
		{
			contents.push_back(i - 48);
		}
	}


	//for (int i = 0; i < contents.size(); i++)
	//{
	//	std::cout <<i<<"th item is: "<< contents[i]<<" ";
	//}
	//std::cout << std::endl;
	
	file.close();
	return contents;
}