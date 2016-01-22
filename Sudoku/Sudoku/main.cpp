#include <iostream>
#include "Sudoku.h"

int main()
{
	Sudoku s = Sudoku(9);
	Sudoku p = Sudoku(12);
	s.build();
	s.print();

	p.build();
	p.print();
	std::cin.get();
	return 0;

}