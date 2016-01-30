#include <iostream>
#include "Sudoku.h"
#include "FileManager.h"

int main()
{
	FileManager file = FileManager();
	std::vector<int> reqs = file.readFile("C:\\TGEDT\\Desktop\\Downloads\\PE1.txt");
	//std::vector<int> reqs = file.readFile("C:\\Users\\Victor\\Documents\\Sudoku\\Sudoku\\Debug\\test.txt");

	Sudoku n = Sudoku(reqs);
	n.print();

	/*Sudoku t = Sudoku(4);
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
	p.print();*/


	std::cin.get();
	return 0;

}