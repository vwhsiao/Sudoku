#include "Square.h"

#include<iostream>
Square::Square(int row, int col, int value, int boxH, int boxW)
{
	Square::row = row;
	Square::col = col;
	Square::value = value;
	Square::boxW = boxW;
	Square::boxH = boxH;

	int boxRow = (row) / boxH;
	int boxCol = (col + 1) / boxW;

	Square::boxNum = (int) (boxRow*boxH) + ((col + 1) / boxW);
}

Square::~Square()
{
}

void Square::print()
{
	std::cout << "zero based indexing used for following:" << std::endl;
	std::cout << "row: " << row << " col: " << col << std::endl;
	std::cout << "value: " << value<<std::endl;
	std::cout << "box number: " << boxNum<<std::endl;
}