#include <vector>
#include <random>
#include <iostream>

class Sudoku
{
public:
	Sudoku(int width);
	~Sudoku();
	void build();
	void print();
	
	void buildRow(int num);
	
	bool checkRow(int num);
	bool checkCol(int num);

	std::vector<int> remainingValuesPossible(int rowNum, int colNum);

private:
	int size;
	std::vector<int> domain;
	std::vector<std::vector<int>> listOfRows;
	std::vector<std::vector<int>> listOfColumns;
	std::vector<std::vector<int>> listOfBoxes;

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution;
};

