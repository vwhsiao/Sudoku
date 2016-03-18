#ifndef SUDOKU
#define SUDOKU

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <math.h>
#include <time.h>

#include "Square.h"
#include "FileManager.h"


#pragma region OUTPUT LOG

// Enum states for the output log
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

// The individual log items to be added into sudoku for the later output log
class LogItem
{
public:
	LogItem(LogState state, std::string optional);
	LogState state;
	clock_t currentTime;
};

#pragma endregion


class Sudoku
{
public:
	// Constructors
	Sudoku(int width);
	Sudoku(std::vector<int> reqs);
	Sudoku(std::vector<int> reqs, float time, std::vector<std::string>options);
	// Initialization
	void init(int size, int boxW, int boxH);
	void buildSquaresAndLists();
	void fillSudokuByInput(std::vector<int> sudoku);

	// Destructor and Clean-up
	~Sudoku();
	void resetRow(int num);
	void resetSudoku();
	void clear();

	// Sudoku properties
	int size;
	int boxW;
	int boxH;
	std::vector<int> domain;
	// Sudoku lists of Square references
	std::vector<std::vector<Square*>> listOfRows;
	std::vector<std::vector<Square*>> listOfColumns;
	std::vector<std::vector<Square*>> listOfBoxes;
	std::vector<Square*> listOfAllSquares;

	// Random Number Generator
	void buildByRng();
	bool restarted = false;
	void fillSquareByRng(int num, int col);
	std::random_device rd;
	std::default_random_engine generator = std::default_random_engine(rd());;
	std::uniform_int_distribution<int> distribution;
	// Problem Generator
	void generateProblem(int numToFill);
	void generateProblem_withConsistencyOnly(int numToFill);
	
	// Backtracking Search
	void BTSolveStart();
	bool BTSolve(int row, int col);
	// Remaining values possible (2 versions)
	std::vector<int> remainingValuesPossible(int rowNum, int colNum);
	std::vector<int> remainingValuesPossible2(int rowNum, int colNum);

	// Forward Checking Search
	void FCSolveStart();
	bool FCSolve_old(int row, int col, Square* prevHost = nullptr);
	bool FCSolve(int row, int col, Square* prevHost = nullptr);
	// Heuristics
	std::vector<Square*> findCandidates();
	std::vector<Square*> filterByMRV(std::vector<Square*> candidates);
	std::vector<Square*> filterByDH(std::vector<Square*> candidates);
	Square* MRV_only();
	Square* DH_only();
	Square* MRV_DH();
	int LCV(Square* hostSquare);
	bool MACCheck(Square* square);

	// Square alternation
	void cancelValue(Square* square);
	bool assignValue(Square* square, int _value);
	// Neighbors
	void addToNeighborInfos(Square* self, Square* neighbor);
	void buildNeighborInfos(Square* square);
	void applyNeighborInfos(Square* square);
	void addToDomains(Square* square); 
	bool removeFromDomains(Square* square);
	bool removeFromDomainAndCheckSize(Square* s, int row, int col, int value);
	
	// Solution
	bool solution = false;
	bool consistencyCheck();
	
	// Time and Clock
	clock_t timeStart;
	float time;
	bool isTimeUp();
	
	// Tokens
	bool genToken = false;
	bool BTSearch = true;
	bool FCSearch = false;
	bool MRV_bool = false;
	bool LCV_bool = false;
	bool DH_bool = false;

	// Output log
	std::vector<LogItem> listOfLogItems;
	std::string generateLog();
	float calculateTime(clock_t deltaTime);
	int countNodes = 0;
	float timeToCount = 0.0f;
	int deadends = 0;
	std::string status;
	void addToLog(LogState logState, std::string optional = "");

	// Console-printing methods
	void printByBoxes();
	void printByColumns();
	void print();

	// Formatted strings
	std::string returnSudoku();
	std::string returnSolution();
	std::string returnNoSolution();
	std::string getSudokuPrint(std::string title = "", int row = -1, int col = -1);
	std::string convertValue(int v);

	// Debug Log
	int debugFiles = 0;
	int debugCount = 0;
	int debugLimit = 12500;
	std::string debugLogContents = "";
	FileManager debugFile = FileManager();
	void debugLogWriteOut();
	void debugLog(std::string text, std::string end = "\n");
	void debugLogActualNeighborDomains(int row, int col, int boxNum, bool showLastResult = true);
};

#endif