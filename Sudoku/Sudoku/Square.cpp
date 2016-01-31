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
	int boxCol = (col) / boxW;

	Square::boxNum = (int)(boxRow*boxH) + (col / boxW);
}

Square::Square(int boxH, int boxW, int boxNum)
{
	Square::boxH = boxH;
	Square::boxW = boxW;
	Square::boxNum = boxNum;

	// Idk how to math
	Square::col = 0;
	Square::row = 0;
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