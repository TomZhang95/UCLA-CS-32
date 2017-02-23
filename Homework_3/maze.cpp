#include <string>

using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    if (sr == er && sc == ec)
        return true;
    maze[sr][sc] = '@';
    if (sr != 0 && maze[sr-1][sc] == '.' && pathExists(maze, nRows, nCols, sr-1, sc, er, ec))
        return true;
    if (sc != nCols - 1 && maze[sr][sc+1] == '.' && pathExists(maze, nRows, nCols, sr, sc+1, er, ec))
        return true;
    if (sr != nRows - 1 && maze[sr+1][sc] == '.' && pathExists(maze, nRows, nCols, sr+1, sc, er, ec))
        return true;
    if (sr != 0 && maze[sr][sc-1] == '.' && pathExists(maze, nRows, nCols, sr, sc-1, er, ec))
        return true;
    return false;
}