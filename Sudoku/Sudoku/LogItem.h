#ifndef LOGITEM
#define LOGITEM

#include <vector>
#include <iostream>
#include "Sudoku.h"
#include <string>
#include <time.h>
class LogItem
{
public:
	LogItem(Sudoku::LogState state, clock_t currentTIme);
	Sudoku::LogState state;
	clock_t currentTime;
};

#endif LOGITEM