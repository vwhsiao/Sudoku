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
	Sudoku test = Sudoku(generateFromFile("test.txt"));
	test.print();
	test.printByColumns();
	test.printByBoxes();

	/*
	Sudoku s4 = Sudoku(4);
	Sudoku s6 = Sudoku(6);
	Sudoku s9 = Sudoku(9);
	Sudoku s12 = Sudoku(12);


	s4.build();
	s4.print();

	s6.build();
	s6.print();

	s9.build();
	s9.print();

	s12.build();
	s12.print();
	*/

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