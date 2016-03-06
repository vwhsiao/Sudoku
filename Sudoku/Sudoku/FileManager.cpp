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
	
	std::string word;
	while (file >> word)
	{
		int i;
		try
		{
			i = std::stoi(word);
		}
		catch (std::invalid_argument e)
		{
			i = word[0] - 48 - 7;
		}
		contents.push_back(i);
	}
	
	file.close();
	return contents;
}

void FileManager::writeTo(std::string filename, std::string output)
{
	std::ofstream file;
	file.open(filename);

	file << output;
	file.close();
}