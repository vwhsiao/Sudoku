#ifndef SQUARE
#define SQUARE

class Square
{
public:
	Square(int row, int col, int value, int boxH, int boxW);
	Square(int boxH, int boxW, int boxNum);
	~Square();
	void print();

	float row, col, value, boxH, boxW;
	int boxNum;
private:
};

#endif