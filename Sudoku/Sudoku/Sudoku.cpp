#include "Sudoku.h"
LogItem::LogItem(LogState state, std::string optional)
{
	LogItem::state = state;
	LogItem::currentTime = clock();
	
}
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
	timeStart = clock();
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

		addToLog(LogState::SEARCH_DONE);

	}
	else if (FCSearch)
	{

		if (reqs.size() > 0)
			Sudoku::fillSudokuByInput(reqs);
		addToLog(LogState::SEARCH_START);
		FCSolveStart();

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
			{
				removeFromDomains(listOfRows[r][c]);
				listOfRows[r][c]->given = true;
			}
		}
	}
	debugLog(getSudokuPrint("Given Sudoku"));
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
			if (solution)
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
	solution = BTSolve(0, 0);
	if (solution)
	{
		status = "success";
		return;
	}
	else if (!solution)
	{
		if (isTimeUp())
		{
			status = "timeout";
			return;
		}
		status = "success";
		return;
	}
	else
	{
		status = "error";
	}
	
	
}

bool Sudoku::BTSolve(int row, int col)
{
	if (isTimeUp())
	{
		return false;
	}
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
				if (isTimeUp())
				{
					return false;
				}
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

bool Sudoku::isTimeUp()
{
	clock_t nowTime = clock();
	float timePassed = (float)(nowTime - timeStart) / CLOCKS_PER_SEC;
	
	return (timePassed > Sudoku::time);
	}

void Sudoku::FCSolveStart()
{
	for (int row = 0; row < Sudoku::size; row++)
	{
		for (int col = 0; col < Sudoku::size; col++)
		{
			if (listOfRows[row][col]->getValue() != 0)
			{
				removeFromDomains(listOfRows[row][col]);
			}
		}
	}

	for (int row = 0; row < Sudoku::size; row++)
	{
		for (int col = 0; col < Sudoku::size; col++)
		{
			listOfRows[row][col]->initialDomain = listOfRows[row][col]->getDomain();
			listOfRows[row][col]->storeDomain();
		}
	}

	std::cout << "\n\nStart solving...\n\n" << std::endl;

	solution = FCSolve(0, 0);
	if (solution)
	{
		status = "success";
		return;
	}
	else
	{
		if (isTimeUp())
		{
			status = "timeout";
			return;
		}
		status = "success";
		return;
	}
	status = "error";
	

		
}


bool Sudoku::FCSolve(int row, int col, Square* prevHost)
{
	if (isTimeUp())
	{
		return false;
	}
	//if column is at the end (col is ++ at the start of every recursive call, so if col is the same as size) and if row is in the last row
	bool isAtEndOfColumn = (col == size);
	bool isAtLastRow = (row == (size - 1));

	// the last cell has been finished
	if (isAtEndOfColumn && isAtLastRow)
		return true;

	// move on to the next row and reset the column to front
	if (isAtEndOfColumn)
	{
		row++;
		col = 0;
	}

	Square* currSquare = listOfRows[row][col];

	// if square is assigned already
	if (currSquare->getValue() != 0)
	{
		//debugLog("This square is GIVEN. Moving on.");
		// move on to the next value
		currSquare->domainLocked = true;
		bool isNextSquareSafe = FCSolve(row, col + 1, prevHost);
		if (isNextSquareSafe)
			return true;
		currSquare->domainLocked = false;
		return false;
	}

	debugLog("thickborder");
	debugLog(currSquare->getHostString(), "");
	debugLog("smallborder");

	if (prevHost != nullptr)
		addToNeighborInfos(prevHost, currSquare);

	// otherwise, if value is not assigned...

	currSquare->storeDomain();

	// value is not assigned and domain isn't empty
	distribution = std::uniform_int_distribution<int>(0, currSquare->getDomain().size() - 1);
	
	while (true)
	{
		if (isTimeUp())
		{
			return false;
		}
		// keep going through values in the domain
		int index = distribution(generator);
		int value = currSquare->getDomain()[index];
		
		debugLog("\nAssigning new value in domain...");
		debugLog("Host info: " + currSquare->getDomainString(), "");
		debugLog("Assigning value: --------------> (" + std::to_string(value) + ")");

		countNodes++;
		currSquare->domainLocked = false;
		bool isForwardCheckingSafe = assignValue(currSquare, value);

		buildNeighborInfos(currSquare);
		/*if (!isAtLastRow && isAtEndOfColumn)
		{
			addToNeighborInfos(currSquare, listOfRows[row + 1][0]);
		}*/

		if (isForwardCheckingSafe)
		{
			debugLog("\nForward checking test PASSED, moving on to next square.\n");
			currSquare->domainLocked = true;
			bool isNextSquareSafe = FCSolve(row, col + 1, currSquare);
			currSquare->domainLocked = false;

			if (isNextSquareSafe)
			{
				debugLog("\nSquare is safe, returning true!\n");
				currSquare->neighborInfos.clear();
				return true;
			}
		
			// the next square backtracks, so we should try a new value
			debugLog(currSquare->getHostString());
			debugLog("smallborder");
			debugLog("\nBacktrack just happened, reassign new value from domain?\n");
			debugLog("Host info: " + currSquare->getDomainString());

			//applyNeighborInfos(currSquare);
		}
		else
		{
			debugLog("\nForward checking test FAILED, reassign new value from domain?\n");
			debugLog("Host Info: " + currSquare->getDomainString());

			//addToDomains(currSquare);
			//currSquare->removeFromDomain();
		}

		currSquare->domainLocked = false;
		applyNeighborInfos(currSquare);
		currSquare->neighborInfos.clear();
		
		// time to check the domain to see if there are anymore values
		bool domainIsNotEmpty = (currSquare->getDomain().size() != 0);
		if (domainIsNotEmpty)
		{
			debugLog("\nPrepping for the next value.\n");
			debugLog("thinborder");
			distribution = std::uniform_int_distribution<int>(0, currSquare->getDomain().size() - 1);
			continue;
		}

		// where backtracking happens
		debugLog("\nNo more values to try from domain, backtracking from:");
		debugLog(currSquare->getHostString());

		deadends++;
		currSquare->domainLocked = false;
		cancelValue(currSquare);
		//currSquare->restoreDomain();
		currSquare->neighborInfos.clear();
		debugLog("backborder");
		return false;	
	}
}

void Sudoku::addToNeighborInfos(Square* self, Square* neighbor)
{
	int row = neighbor->row;
	int col = neighbor->col;
	int boxNum = neighbor->boxNum;
	int value = neighbor->getValue();
	std::vector<int> domain = neighbor->getDomain();
	std::vector<int> storedDomain = neighbor->getStoredDomain();

	self->neighborInfos.push_back(Square(row, col, boxNum, value, domain, storedDomain));
}

void Sudoku::buildNeighborInfos(Square* square)
{
	debugLog("Building/storing neighbors info:\n", "");
	debugLog(square->getHostString(), "\n\n");

	int row = square->row;
	int col = square->col;
	int boxNum = square->boxNum;
	square->neighborInfos.clear();

	Square* s = square;
	for (int i = 0; i < size; i++)
	{
		s = listOfRows[row][i];
		if (!s->given && !s->domainLocked)
			addToNeighborInfos(square, s);

		s = listOfColumns[col][i];
		if (!s->given && !s->domainLocked)
			addToNeighborInfos(square, s);

		s = listOfBoxes[boxNum][i];
		if (!s->given && !s->domainLocked)
			addToNeighborInfos(square, s);
	}
	debugLog(listOfRows[row][col]->getNeighborInfosString());
}

void Sudoku::applyNeighborInfos(Square* square)
{
	int row = square->row;
	int col = square->col;
	int boxNum = square->boxNum;
	int value = square->getValue();

	std::vector<Square> neighbors = square->neighborInfos;
	for (int i = 0; i < neighbors.size(); i++)
	{
		int r = neighbors[i].row;
		int c = neighbors[i].col;

		std::vector<int> d = neighbors[i].getDomain();
		std::vector<int> sd = neighbors[i].getStoredDomain();


		/*if (r == row && c == col)
		{
			listOfRows[r][c]->storedDomain = sd;
			continue;
		}*/

		listOfRows[r][c]->restoreDomains(d, sd);
	}
	debugLog("\nDomains restored, ", "");
	debugLogActualNeighborDomains(row, col, boxNum, false);
}

void Sudoku::cancelValue(Square* square)
{
	int row = square->row;
	int col = square->col;
	int boxNum = square->boxNum;
	int value = square->getValue();

	debugLog("\n\n== Backtracking ==\n\n", "");
	debugLog("(Before)\nHost info: " + square->getDomainString(), "");

	addToDomains(square);
	square->resetValue();
}

bool Sudoku::assignValue(Square* square, int _value)
{
	int row = square->row;
	int col = square->col;
	int boxNum = square->boxNum;

	debugLog("\n\n== Assignment ==\n\n(Before)\nHost Info: " + square->getDomainString(), "");

	addToDomains(square);
	square->removeFromDomain();
	square->setValue(_value);

	bool result = removeFromDomains(square);

	debugLog("\n(After)\nHost Info: " + square->getDomainString());
	debugLog(getSudokuPrint("\nAssigned", square->row, square->col));
	
	return result;
}

bool Sudoku::removeFromDomains(Square* square)
{
	int row = square->row;
	int col = square->col;
	int boxNum = square->boxNum;
	int value = square->getValue();

	Square* s = square;
	for (int i = 0; i < Sudoku::size; i++)
	{
		s = listOfRows[row][i];
		if (!removeFromDomainAndCheckSize(s, row, col, value))
			return false;
		
		s = listOfColumns[col][i];
		if (!removeFromDomainAndCheckSize(s, row, col, value))
			return false;

		s = listOfBoxes[boxNum][i];
		if (!removeFromDomainAndCheckSize(s, row, col, value))
			return false;
	}
	return true;
}

bool Sudoku::removeFromDomainAndCheckSize(Square* s, int row, int col, int value)
{
	if (s->row == row && s->col == col)
		return true;
	
	s->removeFromDomain(value);
	if (s->getDomain().size() == 0 && s->getValue() == 0)
	{
		debugLog("\nOops, someone's got empty domain during FC: " + s->getDomainString());
		return false;
	}
	return true;
}

void Sudoku::addToDomains(Square* square)
{
	int row = square->row;
	int col = square->col;
	int boxNum = square->boxNum;
	int value = square->getValue();

	Square* s;
	for (int i = 0; i < Sudoku::size; i++)
	{
		s = listOfRows[row][i];
		s->addToDomain(value);
		
		s = listOfColumns[col][i];
		s->addToDomain(value);
		
		s = listOfBoxes[boxNum][i];
		s->addToDomain(value);
	}
}

void Sudoku::debugLog(std::string text, std::string end)
{
	if (text == "out")
	{
		debugFile.writeTo("debugLog.txt", debugLogContents);
		return;
	}

	if (text == "thickborder")
		text = "\n======================================================================================\n";
	else if (text == "backborder")
		text = "\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n";
	else if (text == "thinborder")
		text = "\n-------------------------------------------------------------------\n";
	else if (text == "smallborder")
		text = "\n---------------------------------------\n";
	debugLogContents += text + end;
}

void Sudoku::debugLogActualNeighborDomains(int row, int col, int boxNum, bool showLastResult)
{
	debugLog("Actual neighboring domains:");
	for (int i = 0; i < size; i++)
	{
		debugLog(listOfRows[row][i]->getDomainString(showLastResult), "");
		debugLog(listOfColumns[col][i]->getDomainString(showLastResult), "");
		debugLog(listOfBoxes[boxNum][i]->getDomainString(showLastResult), "");
	}
}

std::string Sudoku::getSudokuPrint(std::string title, int row, int col)
{
	std::string text = title;
	int save = -1;
	if (row != -1 && col != -1)
		text += " value: " + std::to_string(listOfRows[row][col]->getValue());
	text += "\n";

	for (int r = 0; r < size; r++)
	{
		if (r % Sudoku::boxH == 0)
			text += "--------------------------\n";
		for (int c = 0; c < size; c++)
		{
			if (c % Sudoku::boxW == 0)
				text += "| ";
			int value = listOfRows[r][c]->getValue();
			if (r == row && c == col)
				text += "* ";
			else
				text += convertValue(value) + " ";
		}
		text += "|\n";
	}
	text += "--------------------------\n";
	return text;
}
