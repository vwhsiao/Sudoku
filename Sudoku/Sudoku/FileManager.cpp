#include "FileManager.h"

#include <iostream>
#include <fstream>
#include <vector>


//Constructor
FileManager::FileManager()
{
}

//Deconstructor
FileManager::~FileManager()
{
}

//readFile opens and reads from a file to return a vector of ints from inside the input file
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

//writeTo takes in a target filename and the string to write to it 
//and then creates or opens the file and writes to it
void FileManager::writeTo(std::string filename, std::string output)
{
	std::ofstream file;
	file.open(filename);

	file << output;
	file.close();
}