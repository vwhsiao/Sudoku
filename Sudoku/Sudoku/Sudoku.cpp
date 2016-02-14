#include "Sudoku.h"
LogItem::LogItem(LogState state, std::string optional)
{
	LogItem::state = state;
	LogItem::currentTime = clock();
	
}
void Sudoku::init(int size, int boxW, int boxH)
{
	Sudoku::initDebugLog();

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
	listOfLogItems = std::vector<LogItem>();
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

Sudoku::Sudoku(std::vector<int> reqs)
{
	listOfLogItems = std::vector<LogItem>();
	addToLog(LogState::TOTAL_START);
	addToLog(LogState::PREPROCESSING_START);
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
	addToLog(LogState::TOTAL_START);
	addToLog(LogState::PREPROCESSING_START);


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
	
	std::cout << options[0] << std::endl;
	
	if (options[0]!=" ")
	{
		for (int i = 0; i < options.size(); i++)
		{
			if (options[i] == "GEN")
			{
				
				buildByRng();
				generateProblem(numToFill);

			}
			else if (options[i] == "BT")
			{
			
				BTSearch = true;	
			}
			else if (options[i] == "FC")
			{
				
				FCSearch = true;
				BTSearch = false;
			}
		}
	}
	addToLog(LogState::PREPROCESSING_DONE);

	if (BTSearch)
	{
		
		if (reqs.size() > 0)
			Sudoku::fillSudokuByInput(reqs);
		addToLog(LogState::SEARCH_START);
		BTSolveStart();
		status = "success";
		addToLog(LogState::SEARCH_DONE);

	}
	else if (FCSearch)
	{

		if (reqs.size() > 0)
			Sudoku::fillSudokuByInput(reqs);
		addToLog(LogState::SEARCH_START);
		FCSolveStart();
		status = "success";
		addToLog(LogState::SEARCH_DONE);

	}		
	addToLog(LogState::SOLUTION_TIME);
	addToLog(LogState::STATUS);
	addToLog(LogState::SOLUTION);
	addToLog(LogState::COUNT_NODES);
	addToLog(LogState::COUNT_DEADENDS);


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

		foundValues.push_back(rowSquare->getValue());
		foundValues.push_back(colSquare->getValue());
		foundValues.push_back(boxSquare->getValue());
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
			bool rowSqFound = rowSquare->getValue() == remainder[rem];
			bool colSqFound = colSquare->getValue() == remainder[rem];
			bool boxSqFound = boxSquare->getValue() == remainder[rem];
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
			int value = sudoku[cellIndex];
			listOfRows[r][c]->setValue(value);
			if (value != 0)
				removeFromDomains(r, c, listOfRows[r][c]->boxNum, value);
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
		if ((listOfRows[row][col]->getValue() == 0) && (col < Sudoku::size))
		{
			// if there are values to use, randomly pick one
			remainingNums = Sudoku::remainingValuesPossible(row, col);
			if (remainingNums.size() > 0) 
			{
				distribution = std::uniform_int_distribution<int>(0, remainingNums.size() - 1);
				int value = remainingNums[distribution(generator)];
				listOfRows[row][col]->setValue(value);
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
	Sudoku* newSudoku = new Sudoku(std::vector<int>{ size, boxH, boxW });

	distribution = std::uniform_int_distribution<int>(0, size-1);
	while (numToFill > 0)
	{
		int row = distribution(generator);
		int col = distribution(generator);
		
		int value = newSudoku->listOfRows[row][col]->getValue();
		if (value == 0)
		{
			newSudoku->listOfRows[row][col]->setValue(listOfRows[row][col]->getValue());
			numToFill--;
		}
	}
	std::cout << "Problem Generator" << std::endl;
	newSudoku->print();

	newSudoku->returnSolution();
	delete newSudoku;
}

//reset
void Sudoku::resetRow(int rowNum)
{
	
	int valueToReset;
	for (int i = 0; i < size; i++)
	{
		Square squareToReset = Square(rowNum-1,i,0,boxH,boxW);
		valueToReset = listOfRows[rowNum - 1][i]->getValue();
		
		listOfRows[rowNum - 1][i]->resetValue();
		//std::cout << "resetting box " << squareToReset.boxNum << std::endl;
		for (int m = 0; m < listOfBoxes[squareToReset.boxNum].size(); m++)
		{
			if (listOfBoxes[squareToReset.boxNum][m]->row == rowNum - 1)
			{
				//std::cout << "resetting this value to 0: " << listOfBoxes[squareToReset.boxNum][m].value << std::endl;;
				listOfBoxes[squareToReset.boxNum][m]->resetValue();
			}
		}
	}
	restarted = true;
	deadends++;

}

void Sudoku::resetSudoku()
{
	Sudoku::clear();
	Sudoku::buildSquaresAndLists();
	deadends++; 
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
			int value = listOfBoxes[i][m]->getValue();
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
			int value = listOfColumns[i][m]->getValue();
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
			int value = listOfRows[i][m]->getValue();
			std::cout << convertValue(value) << " ";
		}
		std::cout << "|" << std::endl;
	}
	std::cout << "--------------------------" << std::endl;
	std::cout << std::endl << "=====================================" << std::endl << std::endl;
}

void Sudoku::addToLog(LogState logState, std::string optional)
{
	
	listOfLogItems.push_back(LogItem(logState, optional));
	
}


std::string Sudoku::generateLog()
{
	float prep_dn_time = 0, prep_st_time = 0, srch_dn_time = 0, srch_st_time = 0;

	std::string log = "";
	for (int i = 0; i < listOfLogItems.size(); i++)
	{
		LogItem item = listOfLogItems[i];
		float time = calculateTime(item.currentTime);
		switch (item.state)
		{
		case LogState::TOTAL_START:
			log += "TOTAL_START=" + std::to_string(time);
			break;
		case LogState::PREPROCESSING_START:
			log += "PREPROCESSING_START=" + std::to_string(time);
			prep_st_time = time;
			break;
		case LogState::PREPROCESSING_DONE:
			log += "PREPROCESSING_DONE=" + std::to_string(time);
			prep_dn_time = time;
			break;
		case LogState::SEARCH_START:
			log += "SEARCH_START=" + std::to_string(time);
			srch_st_time = time;
			break;
		case LogState::SEARCH_DONE:
			log += "SEARCH_DONE=" + std::to_string(time);
			srch_dn_time = time;
			break;
		case LogState::SOLUTION_TIME:
			log += "SOLUTION_TIME=" + std::to_string(time);
			if (time > Sudoku::time)
			{
				status = "timeout";
			}
			break;
		case LogState::STATUS:
			
			log += "STATUS=" + status;
			break;
		case LogState::SOLUTION:
			log += "SOLUTION=" + std::to_string((prep_dn_time - prep_st_time) + (srch_dn_time - srch_st_time));
			if ((status == "success")||(status == "timeout"))
				log += returnSolution();
			else
				log += returnNoSolution();
			break;
		case LogState::COUNT_NODES:
			log += "COUNT_NODES="+std::to_string(countNodes);
			break;
		case LogState::COUNT_DEADENDS:
			
			log += "COUNT_DEADENDS=" + std::to_string(deadends);
			break;
		}
		log += "\n";
	}
	return log;
}

float Sudoku::calculateTime(clock_t deltaTime)
{
	return (float)deltaTime/CLOCKS_PER_SEC;
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
			int value = listOfRows[i][m]->getValue();
			output += std::to_string(value);
			output += " ";
		}
		output += "\n";
	}
	//std::cout << output << std::endl;
	return output;
}

std::string Sudoku::returnSolution()
{
	std::string output;
	output += "(";
	for (int i = 0; i < size; i++)
	{
		for (int m = 0; m < size; m++)
		{
			int value = listOfRows[i][m]->getValue();
			output += std::to_string(value);
			output += ",";
		}
	}
	output.pop_back();
	output += ")";
	return output;
}

std::string Sudoku::returnNoSolution()
{
	std::string output;
	output += "(";
	for (int i = 0; i < size; i++)
	{
		for (int m = 0; m < size; m++)
			output += "0,";
	}
	output.pop_back();
	output += ")";
	return output;
}

void Sudoku::BTSolveStart()
{
	BTSolve(0, 0);
}

bool Sudoku::BTSolve(int row, int col)
{
	//if column is at the end (col is ++ at the start of every recursive call, so if col is the same as size) and if row is in the last row
	if ((col == size) && (row == (size - 1)))
		return true;

	if (col == size)
	{
		col = 0;
		row++;
	}
	if (listOfRows[row][col]->getValue() == 0)
	{
		std::vector<int> remainingNums = remainingValuesPossible(row, col);
		if (remainingNums.size() > 0)
		{
			distribution = std::uniform_int_distribution<int>(0, remainingNums.size() - 1);
			while (true)
			{
				int index = distribution(generator);
				int value = remainingNums[index];
				listOfRows[row][col]->setValue(value);
				countNodes++;
				if (BTSolve(row, col + 1))
				{
					return true;
				}
				else
				{
					listOfRows[row][col]->resetValue();
					remainingNums.erase(remainingNums.begin() + index);
					if (remainingNums.size() > 0)
					{
						distribution = std::uniform_int_distribution<int>(0, remainingNums.size() - 1);
					}
					else
					{
						deadends++;
						return false;
					}
				}
			}
		}
		else
		{
			deadends++;
			return false;
		}
	}
	else
	{

		if (BTSolve(row, col + 1))
			return true;
		return false;
	}
	
}


void Sudoku::FCSolveStart()
{
	for (int row = 0; row < Sudoku::size; row++)
	{
		for (int col = 0; col < Sudoku::size; col++)
		{
			if (listOfRows[row][col]->getValue() != 0)
			{
				std::cout <<"removing: "<< listOfRows[row][col]->getValue() << std::endl;

				removeFromDomains(row, col, listOfRows[row][col]->boxNum, listOfRows[row][col]->getValue());
				
				listOfBoxes[listOfRows[row][col]->boxNum][row]->printDomain();
				/*std::cout << "listOfRows[" << row << "][" << col << "] domain: ";
				for (int i = 0; i < listOfRows[row][col]->getDomain().size(); i++)
				{
					std::cout << listOfRows[row][col]->getDomain()[i];
				}
				std::cout << std::endl;*/

				/*std::cout << "listOfCol[" << col << "][" << row << "] domain: ";
				for (int i = 0; i < listOfColumns[col][row]->getDomain().size(); i++)
				{
					std::cout << listOfColumns[col][row]->getDomain()[i];
				}
				std::cout << std::endl;

				std::cout << "listOfBoxes[" << listOfRows[row][col]->boxNum << "] domain: ";
				for (int i = 0; i < listOfBoxes[listOfRows[row][col]->boxNum].size(); i++)
				{
					std::cout << listOfBoxes[listOfRows[row][col]->boxNum][i]->getDomain()[i];
				}
				std::cout << std::endl;*/
			}
		}
		
	}

	for (int i = 0; i < listOfBoxes[0].size(); i++)
	{
		for (int m = 0; m < listOfBoxes[0][i]->getDomain().size(); m++)
		{
			std::cout << listOfBoxes[0][i]->getDomain()[m];
		}
		std::cout << std::endl;
	}



	FCSolve(0, 0);
}

bool Sudoku::FCSolve(int row, int col)
{
	//if column is at the end (col is ++ at the start of every recursive call, so if col is the same as size) and if row is in the last row
	if ((col == size) && (row == (size - 1)))
		return true;

	if (col == size)
	{
		col = 0;
		row++;
	}
	if (listOfRows[row][col]->getValue() == 0)
	{
		std::vector<int> remainingNums = listOfRows[row][col]->getDomain();
		if (remainingNums.size() > 0)
		{
			distribution = std::uniform_int_distribution<int>(0, remainingNums.size() - 1);
			while (true)
			{
				int index = distribution(generator);
				int value = remainingNums[index];
				
				listOfRows[row][col]->setValue(value);

				removeFromDomains(row, col, listOfRows[row][col]->boxNum, value);

				countNodes++;
				if (FCSolve(row, col + 1))
				{
					return true;
				}
				else
				{
					int value = listOfRows[row][col]->getValue();
					
					addToDomains(row, col, listOfRows[row][col]->boxNum, value);
					listOfRows[row][col]->resetValue();
					
					remainingNums.erase(remainingNums.begin() + index);
					if (remainingNums.size() > 0)
					{
						distribution = std::uniform_int_distribution<int>(0, remainingNums.size() - 1);
					}
					else
					{
						deadends++;
						return false;
					}
				}
			}
		}
		else
		{
			deadends++;
			return false;
		}
	}
	else
	{

		if (FCSolve(row, col + 1))
			return true;
		return false;
	}
}

void Sudoku::removeFromDomains(int row, int col, int boxNum, int value)
{
	for (int i = 0; i < Sudoku::size; i++)
	{
		//std::cout << "row :" << row << " col: " << col << "'s domain values before: ";
		//for (int m = 0; m < listOfRows[row][i]->getDomain().size(); m++)
		//{
		//	std::cout << listOfRows[row][i]->getDomain()[m]<<" ";
		//}
		//std::cout << std::endl;
		
		listOfRows[row][i]->removeFromDomain(value);
		listOfColumns[col][i]->removeFromDomain(value);
		listOfBoxes[boxNum][i]->removeFromDomain(value);

		//std::cout << "row :" << row << " col: " << col << "'s domain values after: ";
		//for (int m = 0; m < listOfRows[row][i]->getDomain().size(); m++)
		//{
		//	std::cout << listOfRows[row][i]->getDomain()[m]<<" ";
		//}
		//std::cout << std::endl;
	}
	
}

void Sudoku::addToDomains(int row, int col, int boxNum, int value)
{
	for (int i = 0; i < Sudoku::size; i++)
	{
		listOfRows[row][i]->addToDomain(value);
		listOfColumns[col][i]->addToDomain(value);
		listOfBoxes[boxNum][i]->addToDomain(value);
	}
}

void Sudoku::initDebugLog()
{
	debugFile = FileManager();
	debugLogContents = "";
}

void Sudoku::debugLog(std::string text)
{
	debugLogContents += text;
	debugFile.writeTo("debugLog.txt", debugLogContents);
}