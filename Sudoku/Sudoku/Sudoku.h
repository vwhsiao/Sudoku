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
	Sudoku(std::vector<int>& reqs, std::vector<std::vector<Square*>> sudoku);
	~Sudoku();

	void buildByRng();

	void printByBoxes();
	void printByColumns();
	void print();
	
	bool checkRow(int num);
	bool checkCol(int num);

	void resetRow(int num);
	void resetSudoku();

	std::vector<int> remainingValuesPossible(int rowNum, int colNum);

	std::string returnOutput();

	void solveStart();
	bool solve(int row, int col);
private:
	double number;
	int size;
	int boxW;
	int boxH;
	bool restarted=false;
	std::vector<int> domain;

	void init(int size, int boxW, int boxH);
	void buildSquaresAndLists();
	void fillSudokuByInput(std::vector<int> sudoku);
	void fillSquareByRng(int num, int col);

	std::vector<std::vector<Square*>> listOfRows;
	std::vector<std::vector<Square*>> listOfColumns;
	std::vector<std::vector<Square*>> listOfBoxes; 
	std::vector<Square*> listOfAllSquares;
	
	std::random_device rd;
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution;
};

#endif