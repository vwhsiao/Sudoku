#include "Sudoku.h"

Sudoku::Sudoku(int width)
{
	for (int i = 1; i <= width; i++)
	{
		Sudoku::domain.push_back(i);
	}

	
	if (width == 9)
	{
		boxH = 3;
		boxW = 3;
		
	}
	else if (width == 12)
	{
		boxH = 3;
		boxW = 4;
	}

	//listOfBoxes.push_back(std::vector<SudokuBox>());
	//SudokuBox x = SudokuBox(boxW, boxH);
	//listOfBoxes[0].push_back(x);





	listOfRows = std::vector<std::vector<Square>>();
	generator = std::default_random_engine(rd());
	
	size = width;
	listOfBoxes = std::vector<std::vector<Square>>();

	


	for (int i = 0; i < Sudoku::size; i++)
	{
		std::vector<Square> col = std::vector<Square>();
		listOfBoxes.push_back(std::vector<Square>());
		for (int m = 0; m < Sudoku::size; m++)
		{
			Square s = Square(i, m, 0, boxH, boxW);
			col.push_back(s);
			
		}
		Square temp = Square(i, 0, 0, boxH, boxW);
		listOfBoxes[temp.boxNum].push_back(temp);
		listOfRows.push_back(col);
		
		listOfColumns.push_back(col);
	}
	
}

Sudoku::~Sudoku()
{
}

/*builds sudoku*/
void Sudoku::build()
{



	for (int i = 1 ; i < Sudoku::size+1; i++)
	{
		
		buildRow(i,0);
		print();
		
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

			if (listOfRows[rowNum][i].value == remainder[m])
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
			
			if (listOfRows[i][colNum].value == remainder[m])
			{
				
				remainder.erase(remainder.begin() + m);
			}
		}
	}

	//box check
	int boxNum = listOfRows[rowNum][colNum].boxNum;

	for (int i = 0; i < listOfBoxes[boxNum].size(); i++)
	{
		for (int m = 0; m < remainder.size(); m++)
		{
			if (listOfBoxes[boxNum][i].value == remainder[m])
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
		
		std::vector<Square> curRow = listOfRows[num];

		if ((curRow[col].value == 0) && (col < Sudoku::size))
		{
			int row = num;
			
			remainingNums = Sudoku::remainingValuesPossible(row, col);

			if (remainingNums.size()>0) 
			{
				
				distribution = std::uniform_int_distribution<int>(0, remainingNums.size() - 1);
				int value = remainingNums[distribution(generator)];
				listOfRows[row][col] = Square(num, col, value, boxH, boxW );
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

//reset
void Sudoku::resetRow(int rowNum)

{
	
	for (int i = 0; i < size; i++)
	{

		listOfRows[rowNum-1][i].value = 0;
	}
	restarted = true;
	
}


void Sudoku::print()
{
	
	for (int i = 0; i < size; i++)
	{
		for (int m = 0; m < size; m++)
		{
			std::cout << listOfRows[i][m].value << " ";
		}
	std::cout<<	std::endl;
	}
	std::cout << "----------------------------------------" << std::endl;
}



