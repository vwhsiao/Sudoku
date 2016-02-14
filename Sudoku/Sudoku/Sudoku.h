#ifndef SUDOKU
#define SUDOKU

#include <vector>
#include <random>
#include <iostream>
#include <math.h>
#include "Square.h"
#include <string>
#include <time.h>

#include "FileManager.h"

enum class LogState
{
	TOTAL_START,
	PREPROCESSING_START,
	PREPROCESSING_DONE,
	SEARCH_START,
	SEARCH_DONE,
	SOLUTION_TIME,
	STATUS,
	SOLUTION,
	COUNT_NODES,
	COUNT_DEADENDS
};

class LogItem
{
public:
	LogItem(LogState state, std::string optional);
	LogState state;
	clock_t currentTime;
};

class Sudoku
{
public:
	Sudoku(int width);
	Sudoku(std::vector<int> reqs);
	Sudoku(std::vector<int> reqs, float time, std::vector<std::string>options);

	~Sudoku();

	void buildByRng();

	void printByBoxes();
	void printByColumns();
	void print();
	std::string generateLog();

	void generateProblem(int numToFill);

	std::vector<int> remainingValuesPossible(int rowNum, int colNum);
	std::vector<int> remainingValuesPossible2(int rowNum, int colNum);

	std::string returnSudoku();
	std::string returnSolution();
	std::string returnNoSolution();

	void BTSolveStart();
	bool BTSolve(int row, int col);

	void FCSolveStart();
	bool FCSolve(int row, int col);

private:
	double number;
	int size;
	int boxW;
	int boxH;
	float time;
	int countNodes = 0; 
	bool restarted=false;
	std::string status;
	
	int deadends=0;
	 

	std::vector<int> domain;
	bool BTSearch = true;
	bool FCSearch = false;
	std::vector<LogItem> listOfLogItems;

	void init(int size, int boxW, int boxH);
	void buildSquaresAndLists();
	void fillSudokuByInput(std::vector<int> sudoku);
	void fillSquareByRng(int num, int col);
	//void generateProblem(int numToFill);
	void resetRow(int num);
	void resetSudoku();
	void clear();
	void addToLog(LogState logState, std::string optional = "");
	
	void removeFromDomains(int row, int col, int boxNum, int value);
	void addToDomains(int row, int col, int boxNum, int value);

	std::vector<std::vector<Square*>> listOfRows;
	std::vector<std::vector<Square*>> listOfColumns;
	std::vector<std::vector<Square*>> listOfBoxes; 
	std::vector<Square*> listOfAllSquares;
	
	float calculateTime(clock_t deltaTime);
	std::string convertValue(int v);

	std::random_device rd;	
	std::default_random_engine generator = std::default_random_engine(rd());;
	std::uniform_int_distribution<int> distribution;

public:
	void initDebugLog();
	void debugLog(std::string text, std::string end = "\n");
	std::string debugLogContents;
	FileManager debugFile = FileManager();
	std::string getSudokuPrint(std::string title = "", int row = -1, int col = -1);
};

#endif