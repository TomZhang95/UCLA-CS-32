#pragma once
#ifndef HISTORY_INCLUDED
#define HISTORY_INCLUDED
#include "globals.h"

class History
{
public:
	History(int nRows, int nCols);
	bool record(int r, int c);
	void display() const;
private:
	int m_nRows;
	int m_nCols;
	int m_historyGrid[MAXROWS][MAXCOLS];
};

#endif // HISTORY_INCLUDED