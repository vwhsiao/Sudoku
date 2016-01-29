#include "Sudoku.h"

Sudoku::Sudoku(int width)
{
	for (int i = 1; i <= width; i++)
	{
		Sudoku::domain.push_back(i);
	}


	if (width == 4)
	{
		boxH = 2;
		boxW = 2;
	}
	else if (width == 6)
	{
		boxH = 3;
		boxW = 2; 
	}
	else if (width == 9)
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
	listOfBoxes = std::vector<std::vector<Square>>();
	generator = std::default_random_engine(rd());
	
	size = width;
	

	for (int i = 0; i < Sudoku::size; i++)
	{
		listOfBoxes.push_back(std::vector<Square>());
	}

	for (int i = 0; i < Sudoku::size; i++)
	{
		std::vector<Square> col = std::vector<Square>();
		
		for (int m = 0; m < Sudoku::size; m++)
		{
			Square s = Square(i, m, 0, boxH, boxW);
			col.push_back(s);
			listOfBoxes[s.boxNum].push_back(s);
			
		}
		//Square temp = Square(i, 0, 0, boxH, boxW);
		//listOfBoxes[temp.boxNum].push_back(temp);
		listOfRows.push_back(col);
		
		listOfColumns.push_back(col);
	}

	//for (int i = 0; i < listOfBoxes.size(); i++)
	//{
	//	std::cout <<"box "<<i<<"'s size is "<< listOfBoxes[i].size() << std::endl;
	//}
	
}

Sudoku::Sudoku(std::vector<int> reqs)
{
	int squaresToFill = reqs[0];
	int size = reqs[1];
	int boxW = reqs[2];
	int boxH = reqs[3];

	for (int i = 1; i <= size; i++)
	{
		Sudoku::domain.push_back(i);
	}

	Sudoku::boxW = boxW;
	Sudoku::boxH = boxH;

	listOfRows = std::vector<std::vector<Square>>();
	listOfBoxes = std::vector<std::vector<Square>>();
	generator = std::default_random_engine(rd());

	Sudoku::size = size;

	for (int i = 0; i < Sudoku::size; i++)
	{
		listOfBoxes.push_back(std::vector<Square>());
	}

	for (int i = 0; i < Sudoku::size; i++)
	{
		std::vector<Square> col = std::vector<Square>();

		for (int m = 0; m < Sudoku::size; m++)
		{
			Square s = Square(i, m, 0, boxH, boxW);
			col.push_back(s);
			listOfBoxes[s.boxNum].push_back(s);
		}
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
	int restartedNum = 0;
	for (int i = 1 ; i < Sudoku::size+1; i++)
	{

		for (int m = 0; m < Sudoku::size; m++)
		{
			
			//int rowWeAreOn = i;
			buildRow(i, m);
			while (restarted)
			{
				//std::cout << "restarted row" << std::endl;
				/*for (int t = 0; t < Sudoku::size; t++)
				{
					restarted = false;
					buildRow(i, t);
					if (restarted)
					{
						restartedNum++;
						i-=restartedNum;
						t = 0;
						break;
					}

				}*/


				i = 1;
				m = -1;
				restarted = false;
			}


			//print();
			
		}
		restartedNum = 0;
		
		
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
	/*std::cout << "remainder items: ";
	for (int i = 0; i < remainder.size(); i++)
	{
		 std::cout<< remainder[i]<<" ";
	}
	std::cout<< std::endl;*/





	//std::cout << "box " << boxNum << ": has ";
	for (int i = 0; i < listOfBoxes[boxNum].size(); i++)
	{
		//std::cout << listOfBoxes[boxNum][i].value<<" ";
		if (listOfBoxes[boxNum][i].value != 0)
		{
			for (int m = 0; m < remainder.size(); m++)
			{
				//std::cout << "comparing item from box: " << listOfBoxes[boxNum][i].value << " with " << remainder[m] << std::endl;
				if (listOfBoxes[boxNum][i].value == remainder[m])
				{
					remainder.erase(remainder.begin() + m);
				}
			}
		}

	}
	//std::cout << std::endl;

	//std::cout << "remainder has ";
	/*for (int i = 0; i < remainder.size(); i++)
	{
		std::cout << remainder[i];
	}
	std::cout<<std::endl;*/

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
				Square assignedSquare = Square(num, col, value, boxH, boxW);

				listOfRows[row][col] = assignedSquare;
				listOfColumns[col][row] = assignedSquare;
				int lastNum = listOfBoxes[assignedSquare.boxNum].size();

				listOfBoxes[assignedSquare.boxNum].push_back(assignedSquare);
				listOfBoxes[assignedSquare.boxNum].erase(listOfBoxes[assignedSquare.boxNum].begin());
				//print();
				//listOfBoxes[assignedSquare.boxNum][lastNum -1].value=assignedSquare.value;
				//listOfBoxes[assignedSquare.boxNum].push_back(Square(num, col + 1, 0, boxH, boxW));
				
			}
			else
			{
				int times = 20;
				
				while (times > 0)
				{
					resetRow(n);
					remainingNums = Sudoku::remainingValuesPossible(row, col);
					int resetting = n;
					while (remainingNums.size() == 0)
					{
						
						resetRow(resetting--);
						remainingNums = Sudoku::remainingValuesPossible(resetting, col);
					}
					times--;
				}
				if (times == 0)
				{
					resetSudoku();
				}
				else
				{
					times = 20;
				}
				

				return;
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
	int valueToReset;
	for (int i = 0; i < size; i++)
	{
		Square squareToReset = Square(rowNum-1,i,0,boxH,boxW);
		valueToReset = listOfRows[rowNum - 1][i].value;
		
		listOfRows[rowNum-1][i].value = 0;
		listOfColumns [i][rowNum - 1].value = 0;
		//std::cout << "resetting box " << squareToReset.boxNum << std::endl;
		for (int m = 0; m < listOfBoxes[squareToReset.boxNum].size(); m++)
		{
			if (listOfBoxes[squareToReset.boxNum][m].row == rowNum-1)
			{
				//std::cout << "resetting this value to 0: " << listOfBoxes[squareToReset.boxNum][m].value << std::endl;;
				listOfBoxes[squareToReset.boxNum][m].value = 0;
			}
		}
	}
	restarted = true;
	

}


void Sudoku::resetSudoku()
{
	listOfRows.clear();
	listOfColumns.clear();
	listOfBoxes.clear();

	listOfRows = std::vector<std::vector<Square>>();
	listOfBoxes = std::vector<std::vector<Square>>();
	listOfColumns = std::vector<std::vector<Square>>();


	for (int i = 0; i < Sudoku::size; i++)
	{
		listOfBoxes.push_back(std::vector<Square>());
	}

	for (int i = 0; i < Sudoku::size; i++)
	{
		std::vector<Square> col = std::vector<Square>();

		for (int m = 0; m < Sudoku::size; m++)
		{
			Square s = Square(i, m, 0, boxH, boxW);
			col.push_back(s);
			listOfBoxes[s.boxNum].push_back(s);

		}
		//Square temp = Square(i, 0, 0, boxH, boxW);
		//listOfBoxes[temp.boxNum].push_back(temp);
		listOfRows.push_back(col);

		listOfColumns.push_back(col);
	}
	restarted = true;
}


void Sudoku::print()
{
	for (int i = 0; i < size; i++)
	{
		if (i % Sudoku::boxH == 0)
			std::cout << "--------------------------" << std::endl;
		for (int m = 0; m < size; m++)
		{
			if (m % Sudoku::boxW == 0)
				std::cout << "| ";
			int value = listOfRows[i][m].value;
			if ((value < 10) && (Sudoku::size > 10))
				std::cout << " " << value << " ";
			else
				std::cout << value << " ";
		}
		std::cout << "|" << std::endl;
	}
	std::cout << "--------------------------" << std::endl;
	std::cout << std::endl << "=====================================" << std::endl << std::endl;
}



