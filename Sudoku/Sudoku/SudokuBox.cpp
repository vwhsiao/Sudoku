#include "SudokuBox.h"

SudokuBox::SudokuBox(int width, int height)
{
	
	SudokuBox::width = width;
	SudokuBox::height = height;
	SudokuBox::sudokuSize = sudokuSize;

	std::cout << SudokuBox::height << std::endl;
	for (int i = 0; i < SudokuBox::height; i++)
	{
		SudokuBox::box.push_back(std::vector<int>());
		
		for (int m = 0; m < SudokuBox::width; m++)
		{
			SudokuBox::box[i].push_back(0);
		}
	}
	//SudokuBox::print();
}

SudokuBox::~SudokuBox()
{
}

void SudokuBox::print()
{
	for (int i = 0; i < box.size(); i++)
	{
		for (int m = 0; m < box[i].size(); m++)
		{
			std::cout << SudokuBox::box[i][m] << " ";
		}
		std::cout << std::endl;
	}
}

bool SudokuBox::exists(int value)
{
	for (int i = 0; i < box.size(); i++)
	{
		for (int m = 0; m < box[i].size(); m++)
		{
			if (box[i][m] == value)
			{
				return true;
			}
		}
	}
	return false;
}