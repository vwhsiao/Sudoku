#ifndef SUDOKU
#define SUDOKU

#include <vector>
#include <random>
#include <iostream>
#include <math.h>
#include "Square.h"
#include <string>
class Sudoku
{
public:
	Sudoku(int width);
	Sudoku(std::vector<int>& reqs);

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

	std::string returnOutput();

	void solveStart();
	bool solve(int row, int col);

private:
	double number;
	int size;
	int boxW;
	int boxH;
	float time;
	bool restarted=false;
	std::vector<int> domain;
	bool BTSearch = true;


	void init(int size, int boxW, int boxH);
	void buildSquaresAndLists();
	void fillSudokuByInput(std::vector<int> sudoku);
	void fillSquareByRng(int num, int col);
	//void generateProblem(int numToFill);
	void resetRow(int num);
	void resetSudoku();
	void clear();

	std::vector<std::vector<Square*>> listOfRows;
	std::vector<std::vector<Square*>> listOfColumns;
	std::vector<std::vector<Square*>> listOfBoxes; 
	std::vector<Square*> listOfAllSquares;
	
	std::string convertValue(int v);

	std::random_device rd;	
	std::default_random_engine generator = std::default_random_engine(rd());;
	std::uniform_int_distribution<int> distribution;
};

#endif