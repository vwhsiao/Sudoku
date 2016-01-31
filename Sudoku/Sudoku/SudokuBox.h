#ifndef SUDOKUBOX
#define SUDOKUBOX

#include <vector>
#include <iostream>
#include "Square.h"

class SudokuBox
{
public:
	SudokuBox(int boxW, int boxH);
	~SudokuBox();
	
	void addSquare(Square* s);

	bool exists(int value);
	bool isFull();

	bool addValue(int value);


private:
	int size;
	std::vector<Square*> box;
};

#endif