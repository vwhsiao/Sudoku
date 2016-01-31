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

int main(int argc, char* argv[])
{
	//std::vector<int> reqs = file.readFile("C:\\TGEDT\\Desktop\\Downloads\\PE1.txt");
	//std::vector<int> reqs = file.readFile("C:\\Users\\Victor\\Documents\\Sudoku\\Sudoku\\Debug\\test.txt");


	//yes, this is hard coded cause supposedly the arguments are always in the same order. 
	FileManager file = FileManager();
	Sudoku s = Sudoku(file.readFile(argv[1]));
	file.writeTo(argv[2], s.returnOutput());
	s.print();

	

	Sudoku test = Sudoku(generateFromFile("test.txt"));
	test.print();
	test.printByColumns();
	test.printByBoxes();

	Sudoku t = Sudoku(4);
	Sudoku k = Sudoku(6);
	Sudoku s = Sudoku(9);
	Sudoku p = Sudoku(12);

	
	t.build();
	t.print();

	k.build();
	k.print();
	
	
	s.build();
	s.print();

	p.build();
	p.print();


	std::cin.get();
	return 0;

}