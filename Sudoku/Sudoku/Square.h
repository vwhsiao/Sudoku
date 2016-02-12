#ifndef SQUARE
#define SQUARE

#include <vector>

class Square
{
public:
	Square(int row, int col, int value, int boxH, int boxW);
	Square(int boxH, int boxW, int boxNum);
	~Square();
	void print();

	void initDomain(int size);
	void removeFromDomain(int _value);
	void addToDomain(int _value);
	void resetValue();
	void setValue(int newValue);
	int getValue();
	
	void printDomain();
	std::vector<int> getDomain();


	

	float row, col, boxH, boxW;
	int boxNum;
private:
	std::vector<int> domain;
	int value;
};

#endif