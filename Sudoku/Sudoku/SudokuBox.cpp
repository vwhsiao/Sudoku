#include "SudokuBox.h"

SudokuBox::SudokuBox(int boxW, int boxH)
{
	for (int i = 0; i < boxW*boxH; i++)
	{
		SudokuBox::box.push_back(new Square());
	}
}

SudokuBox::~SudokuBox()
{
}

void SudokuBox::addSquare(Square* s)
{
	//s->
}

bool SudokuBox::addValue(int value)
{
	if (!SudokuBox::exists(value))
	{
		
		SudokuBox::box.push_back(value);
		if (box[0] == 0)
		{
			box.erase(box.begin());
		}
		return true;
	}
	return false;
}

bool SudokuBox::isFull()
{
	//return true if full, return false if not
	return (SudokuBox::box.size() == SudokuBox::size);
	
}




bool SudokuBox::exists(int value)
{

	for (int i = 0; i < box.size(); i++)
	{
		if (box[i] == value)
		{
			return true;
		}
		
	}
	return false;
}