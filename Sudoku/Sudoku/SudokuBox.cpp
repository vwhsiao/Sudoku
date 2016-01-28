#include "SudokuBox.h"

SudokuBox::SudokuBox(int boxW, int boxH)
{
	
	SudokuBox::size = size;
	SudokuBox::box.push_back(0);
}

SudokuBox::~SudokuBox()
{
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