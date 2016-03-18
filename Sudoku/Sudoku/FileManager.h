#ifndef FILEMANAGER
#define FILEMANAGER

#include <string>
#include <vector>

class FileManager
{
public:
	//Constructors and Deconstructors
	//This class is really meant to just make it easier to open and write
	//to files. So the class itself doesn't hold any information
	FileManager();
	~FileManager();

	//readFile opens and reads from a file to return a vector of ints from inside the input file
	std::vector<int> readFile(std::string filename);

	//writeTo takes in a target filename and the string to write to it 
	//and then creates or opens the file and writes to it
	void writeTo(std::string filename, std::string output);
private:

};

#endif