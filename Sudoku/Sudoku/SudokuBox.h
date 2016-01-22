#include <vector>
#include <iostream>

class SudokuBox
{
public:
	SudokuBox(int width, int height);
	~SudokuBox();
	void print();
	bool exists(int value);

private:
	int width, height, sudokuSize;
	std::vector<std::vector<int>> box;
};

