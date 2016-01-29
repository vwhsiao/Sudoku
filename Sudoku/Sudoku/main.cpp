#include <iostream>
#include "Sudoku.h"


int main()
{
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