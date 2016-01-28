#ifndef SQUARE
#define SQUARE

class Square
{
public:
	Square(int row, int col, int value, int boxH, int boxW);
	~Square();
	void print();

	float row, col, value, boxH, boxW;
	int boxNum;
private:
};

#endif