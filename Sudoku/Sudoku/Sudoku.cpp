#include "Sudoku.h"




Sudoku::Sudoku(int width)
{
	for (int i = 1; i <= width; i++)
	{
		Sudoku::domain.push_back(i);
	}

	listOfRows = std::vector<std::vector<int>>();

	distribution = std::uniform_int_distribution<int> (1, width);
	size = width;
	std::cout << "made";
}

Sudoku::~Sudoku()
{
}

void Sudoku::build()
{
	for (int i = 0; i < Sudoku::size; i++)
	{
		std::vector<int> col = std::vector<int>();
		for (int i = 0; i < Sudoku::size; i++)
		{
			col.push_back(0);
		}
		listOfRows.push_back(col);
	}

	std::cout << size << std::endl;
	for (int i = 0 ; i < Sudoku::size; i++)
	{
		std::cout << "here\n";
		buildRow(size);
		
	}

}

std::vector<int> Sudoku::remainingValuesPossible(int rowNum, int colNum)
{
	//set up domain of all values from 1 to however big the sudoku is
	std::vector<int> remainder;
	for (int i = 1; i <= Sudoku::size; i++)
	{
		remainder.push_back(i);
	}

	//check row for numbers that are already there
	for (int i = 0; i < listOfRows[rowNum].size() - 1; i++)
	{
		for (int m = 0; m < remainder.size(); m++)
		{
			if (listOfRows[rowNum][i] == remainder[m])
			{
				remainder.erase(remainder.begin() + m);
			}
		}
	}

	for (int i = 0; i < listOfBoxes[colNum].size(); i++)
	{
		for (int m = 0; m < remainder.size() - 1; m++)
		{
			if (listOfRows[colNum][i] == remainder[m])
			{
				remainder.erase(remainder.begin() + m);
			}
		}
	}

	return remainder;
}

void Sudoku::buildRow(int n)
{
	int num = n - 1;
	std::vector<int> remainingNums;
	std::cout << listOfRows[num][0] << std::endl;
	if (listOfRows[num].size() < Sudoku::size)
	{
		int row = num;
		int col = listOfRows[num].size() - 1;
		
		std::cout << "before finding remaining" << std::endl;

		remainingNums = Sudoku::remainingValuesPossible(row, col);
		distribution = std::uniform_int_distribution<int>(0, col);
		listOfRows[row][col] = remainingNums[distribution(generator)];
		listOfColumns[col][row] = listOfRows[row][col];
		std::cout << "just finished row " << num << std::endl;
		buildRow(num);
	}
	else
	{
		return;
	}
}

void Sudoku::print()
{
	std::cout << "Hello World" << std::endl;
	for (int i = 0; i < size; i++)
	{
		for (int m = 0; m < size; m++)
		{
			std::cout << listOfRows[i][m] << " ";
		}
		std::cout<<std::endl;
	}
}
