#include <iostream>
#include "Sudoku.h"

int main()
{
	Sudoku s = Sudoku(9);
	s.build();
	s.print();
	std::cin.get();
	return 0;

}