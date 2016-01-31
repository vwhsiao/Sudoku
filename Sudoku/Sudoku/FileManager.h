#ifndef FILEMANAGER
#define FILEMANAGER

#include <string>
#include <vector>

class FileManager
{
public:
	FileManager();
	~FileManager();

	std::vector<int> readFile(std::string filename);
	void writeTo(std::string filename, std::string output);
private:

};

#endif