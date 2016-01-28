#include <vector>
#include <iostream>

#ifndef SUDOKUBOX
#define SUDOKUBOX

class SudokuBox
{
public:
	SudokuBox(int boxW, int boxH);
	~SudokuBox();
	
	bool exists(int value);
	bool isFull();

	bool addValue(int value);


private:
	int size;
	std::vector<int> box;
};

#endif