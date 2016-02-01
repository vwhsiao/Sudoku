#include "LogItem.h"

LogItem::LogItem(Sudoku::LogState state, clock_t currentTIme)
{
	LogItem::state = state;
	LogItem::currentTime = currentTime;
}