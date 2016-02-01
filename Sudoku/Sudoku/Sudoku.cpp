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
	generator = std::default_random_engine(rd());
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
	listOfLogItems = std::vector<LogItem>();
	listOfLogItems.push_back(LogItem(Sudoku::LogState::TOTAL_START, clock()));
	listOfLogItems.push_back(LogItem(Sudoku::LogState::PREPROCESSING_START, clock()));
	//put LogItem thing here for TOTAL_START
	int size = reqs[0];
	int boxW = reqs[1];
	int boxH = reqs[2];
	reqs.erase(reqs.begin(), reqs.begin() + 3);

	Sudoku::init(size, boxW, boxH);
	generator = std::default_random_engine(rd());
	if (reqs.size() > 0)

		Sudoku::fillSudokuByInput(reqs);
}


Sudoku::Sudoku(std::vector<int> reqs, float time, std::vector<std::string>options)
{
	listOfLogItems = std::vector<LogItem>();
	listOfLogItems.push_back(LogItem(Sudoku::LogState::TOTAL_START, clock()));
	listOfLogItems.push_back(LogItem(Sudoku::LogState::PREPROCESSING_START, clock()));


	int numToFill, size, boxW, boxH;
	
	if (reqs.size() == 4)
	{
		numToFill = reqs[0];
		size = reqs[1];
		boxW = reqs[2];
		boxH = reqs[3];
	}
	else
	{
		
		size = reqs[0];
		
		boxW = reqs[1];
		boxH = reqs[2];
		reqs.erase(reqs.begin(), reqs.begin() + 3);
	}
	Sudoku::time = time;
	
	

	Sudoku::init(size, boxW, boxH);
	
	if (options[0]!=" ")
	{
		for (int i = 0; i < options.size()-1; i++)
		{
			if (options[i] == "GEN")
			{
				
				buildByRng();
				//put LogItem thing here for SEARCH_START
				generateProblem(numToFill);
			}
			if (options[i] == "BT")
			{
				BTSearch = true;	
			}
		}
	}
	if (BTSearch)
	{
		if (reqs.size() > 0)
			//put LogItem thing here for SEARCH_START
			Sudoku::fillSudokuByInput(reqs);
		solveStart();
	}

}



Sudoku::~Sudoku()
{
	Sudoku::clear();
}

/*builds sudoku by rng*/
void Sudoku::buildByRng()
{
	
	for (int i = 0 ; i < Sudoku::size; i++)
	{
		for (int m = 0; m < Sudoku::size; m++)
		{
			fillSquareByRng(i, m);
			while (restarted)
			{
				
				i = 0;
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

std::vector<int> Sudoku::remainingValuesPossible2(int rowNum, int colNum)
{
	//set up domain of all values from 1 to however big the sudoku is
	std::vector<int> remainder = Sudoku::domain;

	Square* thisSquare = listOfRows[rowNum][colNum];
	for (int i = 0; i < Sudoku::size; i++)
	{
		Square* rowSquare = listOfRows[rowNum][i];
		Square* colSquare = listOfColumns[colNum][i];
		Square* boxSquare = listOfBoxes[thisSquare->boxNum][i];

		// Delete remaining values if found
		for (int rem = 0; rem < remainder.size(); rem++)
		{
			bool rowSqFound = rowSquare->value == remainder[rem];
			bool colSqFound = colSquare->value == remainder[rem];
			bool boxSqFound = boxSquare->value == remainder[rem];
			if ((rowSqFound) || (colSqFound) || (boxSqFound))
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

void Sudoku::fillSudokuByInput(std::vector<int> sudoku)
{
	
	for (int r = 0; r < Sudoku::size; r++)
	{
		for (int c = 0; c < Sudoku::size; c++)
		{
	
			int cellIndex = r*size + c;
	
			listOfRows[r][c]->value = sudoku[cellIndex];
	
		}
	}
	print();
}

void Sudoku::fillSquareByRng(int row, int col)
{
	if (col < size)
	{
		std::vector<int> remainingNums;
		// if square is empty
		if ((listOfRows[row][col]->value == 0) && (col < Sudoku::size))
		{
			// if there are values to use, randomly pick one
			remainingNums = Sudoku::remainingValuesPossible(row, col);
			if (remainingNums.size() > 0) 
			{
				distribution = std::uniform_int_distribution<int>(0, remainingNums.size() - 1);
				int value = remainingNums[distribution(generator)];
				listOfRows[row][col]->value = value;
			}
			else
			{
				int times = 20;
				while (times > 0)
				{
					resetRow(row);
					remainingNums = Sudoku::remainingValuesPossible(row, col);
					int resetting = row;
					while (remainingNums.size() == 0)
					{
						resetRow(resetting--);
						remainingNums = Sudoku::remainingValuesPossible(resetting, col);
					}
					times--;
				}
				resetSudoku();
			}
		}
	}
}

void Sudoku::generateProblem(int numToFill)
{
	Sudoku newSudoku = Sudoku(std::vector<int>{ size, boxH, boxW });

	distribution = std::uniform_int_distribution<int>(0, size-1);
	while (numToFill > 0)
	{
		int row = distribution(generator);
		int col = distribution(generator);
		
		int value = newSudoku.listOfRows[row][col]->value;
		if (value == 0)
		{
			newSudoku.listOfRows[row][col]->value = listOfRows[row][col]->value;
			numToFill--;
		}
	}
	std::cout << "Problem Generator" << std::endl;
	newSudoku.print();
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
	Sudoku::clear();
	Sudoku::buildSquaresAndLists();

	restarted = true;
}

void Sudoku::clear()
{
	Square* s;
	for (int r = 0; r < listOfRows.size(); r++)
	{
		for (int c = 0; c < listOfRows.size(); c++)
		{
			//std::cout << r << ", " << c << std::endl;
			s = listOfRows[r][c];
			if (s != nullptr)
				delete s;
		}
	}
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
			std::cout << convertValue(value) << " ";
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
			std::cout << convertValue(value) << " ";
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
			std::cout << convertValue(value) << " ";
		}
		std::cout << "|" << std::endl;
	}
	std::cout << "--------------------------" << std::endl;
	std::cout << std::endl << "=====================================" << std::endl << std::endl;
}

std::string Sudoku::convertValue(int v)
{
	std::string value = std::to_string(v);
	if (v >= 10)
	{
		v -= 10;
		value = (char)('A' + v);
	}
	return value;
}

std::string Sudoku::returnSudoku()
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

void Sudoku::solveStart()
{
	solve(0, 0);
}

bool Sudoku::solve(int row, int col)
{
	//if column is at the end (col is ++ at the start of every recursive call, so if col is the same as size) and if row is in the last row
	if ((col == size) && (row == (size - 1)))
	{
		return true;
	}

	if (col == size)
	{
		col = 0;
		row++;
	}
	if (listOfRows[row][col]->value == 0)
	{
		std::vector<int> remainingNums = remainingValuesPossible(row, col);
		if (remainingNums.size() > 0)
		{
			distribution = std::uniform_int_distribution<int>(0, remainingNums.size() - 1);
			while (true)
			{
				int index = distribution(generator);
				int value = remainingNums[index];
				listOfRows[row][col]->value = value;
				if (solve(row, col + 1))
				{
					return true;
				}
				else
				{
					listOfRows[row][col]->value = 0;
					remainingNums.erase(remainingNums.begin() + index);
					if (remainingNums.size() > 0)
					{
						distribution = std::uniform_int_distribution<int>(0, remainingNums.size() - 1);
					}
					else
					{
						return false;
					}

				}
			}


		}
		else
		{
			return false;
		}
	}
	else
	{
		if (solve(row, col + 1))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
}



