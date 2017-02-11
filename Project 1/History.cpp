#include "History.h"
#include <iostream>

using namespace std;

History::History(int nRows, int nCols)
	: m_nRows(nRows), m_nCols(nCols)
{
	int r, c;
	for (r = 1; r <= m_nRows; r++)
		for (c = 1; c <= m_nCols; c++)
			m_historyGrid[r - 1][c - 1] = 0;
}

bool History::record(int r, int c)
{
	if (r>m_nRows || c>m_nCols) //out of bounds
		return false;
	m_historyGrid[r - 1][c - 1] += 1; //a poison pellet is ate at this grid
	return true;
}

void History::display() const
{
	char m_historyGridDisplay[MAXROWS][MAXCOLS];
	int eatTimes;
	int r, c;
	for (r = 1; r <= m_nRows; r++)
		for (c = 1; c <= m_nCols; c++)
		{
			eatTimes = m_historyGrid[r - 1][c - 1];
			if (eatTimes == 0)
				m_historyGridDisplay[r - 1][c - 1] = '.';
			else if (eatTimes > 26)
				m_historyGridDisplay[r - 1][c - 1] = 'Z';
			else
				m_historyGridDisplay[r - 1][c - 1] = eatTimes + 64;

		}
	clearScreen();
	for (r = 1; r <= m_nRows; r++)
	{
		for (c = 1; c <= m_nCols; c++)
		{
			cout << m_historyGridDisplay[r - 1][c - 1];
		}
		cout << endl;
	}
	cout << endl;
}