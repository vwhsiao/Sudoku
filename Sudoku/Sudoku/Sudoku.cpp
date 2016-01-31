#include "Sudoku.h"

void Sudoku::init(int size, int boxW, int boxH)
{
	for (int i = 1; i <= size; i++)
	{
		Sudoku::domain.push_back(i);
	}

	Sudoku::boxW = boxW;
	Sudoku::boxH = boxH;
	Sudoku::size = size;

	Sudoku::buildSquaresAndLists();
}

Sudoku::Sudoku(int width)
{
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

	Sudoku::init(width, boxW, boxH);
	
	generator = std::default_random_engine(rd());	
}

Sudoku::Sudoku(std::vector<int>& reqs)
{
	int size = reqs[0];
	int boxW = reqs[1];
	int boxH = reqs[2];
	reqs.erase(reqs.begin(), reqs.begin() + 3);

	Sudoku::init(size, boxW, boxH);

	Sudoku::fillSudoku(reqs);
}

//take in the sudoku requirements and the sudoku itself so we can solve it
//requirements: 9 3 3 (size, boxW, boxH)
//std::vector<std::vector<Square>> sudoku should replace the listOfRows vector, etc. 
Sudoku::Sudoku(std::vector<int>& reqs, std::vector<std::vector<Square*>> sudoku)
{

}

Sudoku::~Sudoku()
{
	Square* s;
	for (int i = 0; i < listOfAllSquares.size(); i++)
	{
		s = listOfAllSquares[i];
		if (s != nullptr)
			delete s;
	}
	for (int r = 0; r < listOfRows.size(); r++)
	{
		for (int c = 0; c < listOfRows.size(); c++)
		{
			s = listOfRows[r][c];
			if (s != nullptr)
				delete s;
		}
	}
}

/*builds sudoku*/
void Sudoku::build()
{
	for (int i = 1 ; i < Sudoku::size+1; i++)
	{
		for (int m = 0; m < Sudoku::size; m++)
		{
			buildRow(i, m);
			while (restarted)
			{
				i = 1;
				m = -1;
				restarted = false;
			}
			
		}
		print();
	}
}

std::vector<int> Sudoku::remainingValuesPossible(int rowNum, int colNum)
{
	//set up domain of all values from 1 to however big the sudoku is
	std::vector<int> remainder = Sudoku::domain;
	std::vector<int> foundValues = std::vector<int>();
	
	Square* thisSquare = listOfRows[rowNum][colNum];
	for (int i = 0; i < Sudoku::size; i++)
	{
		Square* rowSquare = listOfRows[rowNum][i];
		Square* colSquare = listOfColumns[colNum][i];
		Square* boxSquare = listOfBoxes[thisSquare->boxNum][i];

		foundValues.push_back(rowSquare->value);
		foundValues.push_back(colSquare->value);
		foundValues.push_back(boxSquare->value);

		/*
		for (int rem = 0; rem < remainder.size(); rem++)
		{
			bool rowSqFound = rowSquare->value == remainder[rem];
			bool colSqFound = colSquare->value == remainder[rem];
			bool boxSqFound = boxSquare->value == remainder[rem];
			if ((rowSqFound) || (colSqFound) || (boxSqFound))
				remainder.erase(remainder.begin() + rem);
		}*/
	}
	
	// Delete foundValues off remainder vector
	for (int f = 0; f < foundValues.size(); f++)
	{
		for (int rem = 0; rem < remainder.size(); rem++)
		{
			if (remainder[rem] == foundValues[f])
				remainder.erase(remainder.begin() + rem);
		}
	}

	return remainder;
}

void Sudoku::buildSquaresAndLists()
{
	listOfRows = std::vector<std::vector<Square*>>();
	listOfColumns = std::vector<std::vector<Square*>>();
	listOfBoxes = std::vector<std::vector<Square*>>();

	for (int i = 0; i < Sudoku::size; i++)
	{
		listOfRows.push_back(std::vector<Square*>());
		listOfColumns.push_back(std::vector<Square*>());
		listOfBoxes.push_back(std::vector<Square*>());
	}

	for (int r = 0; r < Sudoku::size; r++)
	{
		for (int c = 0; c < Sudoku::size; c++)
		{
			Square* s = new Square(r, c, 0, Sudoku::boxH, Sudoku::boxW);
			listOfAllSquares.push_back(s);
			listOfRows[r].push_back(s);
			listOfColumns[c].push_back(s);
			listOfBoxes[s->boxNum].push_back(s);
		}
	}
}

void Sudoku::fillSudoku(std::vector<int> sudoku)
{
	for (int r = 0; r < Sudoku::size; r++)
	{
		for (int c = 0; c < Sudoku::size; c++)
		{
			int cellIndex = r*size + c;
			listOfRows[r][c]->value = sudoku[cellIndex];
		}
	}
}

void Sudoku::buildRow(int n, int col)
{
	if (col <= size-1)
	{
		int num = n - 1;
		std::vector<int> remainingNums;
		
		std::vector<Square*> curRow = listOfRows[num];

		if ((curRow[col]->value == 0) && (col < Sudoku::size))
		{
			int row = num;
			
			remainingNums = Sudoku::remainingValuesPossible(row, col);

			if (remainingNums.size()>0) 
			{
				
				distribution = std::uniform_int_distribution<int>(0, remainingNums.size() - 1);
				int value = remainingNums[distribution(generator)];
				Square* assignedSquare = new Square(num, col, value, boxH, boxW);
				listOfAllSquares.push_back(assignedSquare);

				listOfRows[row][col] = assignedSquare;
				listOfColumns[col][row] = assignedSquare;
				int lastNum = listOfBoxes[assignedSquare->boxNum].size();

				listOfBoxes[assignedSquare->boxNum].push_back(assignedSquare);
				listOfBoxes[assignedSquare->boxNum].erase(listOfBoxes[assignedSquare->boxNum].begin());
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
		valueToReset = listOfRows[rowNum - 1][i]->value;
		
		listOfRows[rowNum - 1][i]->value = 0;
		//std::cout << "resetting box " << squareToReset.boxNum << std::endl;
		for (int m = 0; m < listOfBoxes[squareToReset.boxNum].size(); m++)
		{
			if (listOfBoxes[squareToReset.boxNum][m]->row == rowNum - 1)
			{
				//std::cout << "resetting this value to 0: " << listOfBoxes[squareToReset.boxNum][m].value << std::endl;;
				listOfBoxes[squareToReset.boxNum][m]->value = 0;
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

	listOfRows = std::vector<std::vector<Square*>>();
	listOfBoxes = std::vector<std::vector<Square*>>();
	listOfColumns = std::vector<std::vector<Square*>>();


	for (int i = 0; i < Sudoku::size; i++)
	{
		listOfBoxes.push_back(std::vector<Square*>());
	}

	for (int i = 0; i < Sudoku::size; i++)
	{
		std::vector<Square*> col = std::vector<Square*>();

		for (int m = 0; m < Sudoku::size; m++)
		{
			Square* s = new Square(i, m, 0, boxH, boxW);
			listOfAllSquares.push_back(s);
			col.push_back(s);
			listOfBoxes[s->boxNum].push_back(s);

		}
		//Square temp = Square(i, 0, 0, boxH, boxW);
		//listOfBoxes[temp.boxNum].push_back(temp);
		listOfRows.push_back(col);

		listOfColumns.push_back(col);
	}
	restarted = true;
}


void Sudoku::printByBoxes()
{
	std::cout << "Printing by list of Boxes" << std::endl;
	for (int i = 0; i < size; i++)
	{
		std::cout << "--------------------------" << std::endl;
		for (int m = 0; m < size; m++)
		{
			int value = listOfBoxes[i][m]->value;
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

void Sudoku::printByColumns()
{
	std::cout << "Printing by list of Columns" << std::endl;
	for (int i = 0; i < size; i++)
	{
		if (i % Sudoku::boxW == 0)
			std::cout << "--------------------------" << std::endl;
		for (int m = 0; m < size; m++)
		{
			if (m % Sudoku::boxH == 0)
				std::cout << "| ";
			int value = listOfColumns[i][m]->value;
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
			int value = listOfRows[i][m]->value;
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

std::string Sudoku::returnOutput()
{
	std::string output; 
	for (int i = 0; i < size; i++)
	{
		for (int m = 0; m < size; m++)
		{
			int value = listOfRows[i][m]->value;
			output += std::to_string(value);
			output += " ";
		}
		output += "\n";
	}
	return output;
	
}



