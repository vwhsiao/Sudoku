#include <iostream>
#include "Sudoku.h"
#include "FileManager.h"
#include <string>

std::vector<int> generateFromFile(std::string filename)
{
	const std::string inputFilesPath = "..\\_InputFiles\\";
	FileManager file = FileManager();
	return file.readFile(inputFilesPath + filename);
}

void altMain()
{
	bool initTest = false;
	bool rngSize4 = false;
	bool rngSize6 = true;
	bool rngSize9 = false;
	bool rngSize12 = false;

	if (initTest)
	{
		Sudoku test = Sudoku(generateFromFile("test.txt"));
		test.print();
		test.printByColumns();
		test.printByBoxes();
	}

	if (rngSize4)
	{
		Sudoku s4 = Sudoku(4);
		s4.buildByRng();
		s4.print();
	}
	
	if (rngSize6)
	{
		Sudoku s6 = Sudoku(6);
		s6.buildByRng();
		s6.print();
	}

	if (rngSize9)
	{
		Sudoku s9 = Sudoku(9);
		s9.buildByRng();
		s9.print();
	}

	if (rngSize12)
	{
		Sudoku s12 = Sudoku(12);
		s12.buildByRng();
		s12.print();
	}

	std::cin.get();
}

int main(int argc, char* argv[])
{
	if (argv[1] == nullptr)
	{
		altMain();
		return 0;
	}

	//yes, this is hard coded cause supposedly the arguments are always in the same order. 
	FileManager file = FileManager();
	Sudoku s = Sudoku(file.readFile(argv[1]));

	file.writeTo(argv[2], s.returnOutput());
	s.print();

	std::cin.get();
	return 0;
}