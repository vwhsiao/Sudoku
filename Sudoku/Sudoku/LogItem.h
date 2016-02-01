#ifndef LOGITEM
#define LOGITEM

#include <vector>
#include <iostream>
#include "Sudoku.h"
#include <string>

class LogItem
{
public:
	LogItem(Sudoku::LogState state, float currentTIme);
	Sudoku::LogState state;
	float currentTime;
};

#endif LOGITEM