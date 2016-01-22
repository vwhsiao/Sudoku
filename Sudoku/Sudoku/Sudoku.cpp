#include "Sudoku.h"




Sudoku::Sudoku(int width)
{
	for (int i = 1; i <= width; i++)
	{
		Sudoku::domain.push_back(i);
	}

	
	//if (number == 9)
	//{
	//	boxH = 3;
	//	boxW = 3;
	//}
	//else if (number == 12)
	//{
	//	boxH = 3;
	//	boxW = 4;
	//}

	listOfRows = std::vector<std::vector<int>>();
	generator = std::default_random_engine(rd());
	
	size = width;
	
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
		listOfColumns.push_back(col);
	}


	for (int i = 1 ; i < Sudoku::size+1; i++)
	{
		
		buildRow(i,0);
		
		
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

	for (int i = 0; i < listOfRows[rowNum].size(); i++)
	{

		for (int m = 0; m < remainder.size(); m++)
		{

			if (listOfRows[rowNum][i] == remainder[m])
			{
				
				remainder.erase(remainder.begin() + m);
			}
		}
	}
	
	//column check
	for (int i = 0; i < size; i++)
	{
		
		for (int m = 0; m < remainder.size(); m++)
		{
			
			if (listOfRows[i][colNum] == remainder[m])
			{
				
				remainder.erase(remainder.begin() + m);
			}
		}
	}

	return remainder;
}

void Sudoku::buildRow(int n, int col)
{
	if (col <= size-1)
	{
		int num = n - 1;
		std::vector<int> remainingNums;
		
		std::vector<int> curRow = listOfRows[num];

		if ((curRow[col] == 0) && (col < Sudoku::size))
		{
			int row = num;
			
			remainingNums = Sudoku::remainingValuesPossible(row, col);

			if (remainingNums.size()>0) 
			{
				
				distribution = std::uniform_int_distribution<int>(0, remainingNums.size() - 1);
				listOfRows[row][col] = remainingNums[distribution(generator)];
				buildRow(n, col + 1);
			}
			else
			{
				resetRow(n);
				buildRow(n, 0);
			}
		}
		else
		{
			
			return;
		}
	}
	else
	{
		return;
	}
}

void Sudoku::resetRow(int rowNum)
{
	for (int i = 0; i < size; i++)
	{

		listOfRows[rowNum-1][i] = 0;
	}
	restarted = true;
	
}


void Sudoku::print()
{
	
	for (int i = 0; i < size; i++)
	{
		for (int m = 0; m < size; m++)
		{
			std::cout << listOfRows[i][m] << " ";
		}
	std::cout<<	std::endl;
	}
	std::cout << "----------------------------------------" << std::endl;
}
