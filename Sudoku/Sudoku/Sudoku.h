#include <vector>
#include <random>
#include <iostream>
#include <math.h>
//#include "SudokuBox.h"
class Sudoku
{
public:
	Sudoku(int width);
	~Sudoku();
	void build();
	void print();
	
	void buildRow(int num, int col);
	
	bool checkRow(int num);
	bool checkCol(int num);
	void resetRow(int num);

	std::vector<int> remainingValuesPossible(int rowNum, int colNum);

private:
	double number;
	int size;
	int boxW;
	int boxH;
	bool restarted;
	std::vector<int> domain;

	std::vector<std::vector<int>> listOfRows;
	std::vector<std::vector<int>> listOfColumns;

	//std::vector<std::vector<SudokuBox>> listOfBoxes;
	
	std::random_device rd;
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution;
};

