#include "LogItem.h"

LogItem::LogItem(Sudoku::LogState state, float currentTIme)
{
	LogItem::state = state;
	LogItem::currentTime = currentTime;
}