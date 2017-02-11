#include <queue>
#include <string>
#include <iostream>

using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};


bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    queue<Coord> coordqueue;
    coordqueue.push(Coord(sr, sc));
    maze[sr][sc]= '-';
    
    int count = 0;
    while (!coordqueue.empty())
    {
        Coord current(0, 0);
        current = coordqueue.front();
        int r = current.r();
        int c = current.c();
        if (count != 12)
        {
            cerr<<'('<<r<<','<<c<<')'<<", ";
            count++;
        }
        coordqueue.pop();
        if(r == er && c == ec)
            return true;
        if(r != 0 && maze[r-1][c] == '.')   //NORTH
        {
            coordqueue.push(Coord(r-1, c));
            maze[r-1][c] = '-';
        }
        if(c != nCols-1 && maze[r][c+1] == '.')   //EAST
        {
            coordqueue.push(Coord(r, c+1));
            maze[r][c+1] = '-';
        }
        if(r != nRows-1 && maze[r+1][c] == '.')   //SOUTH
        {
            coordqueue.push(Coord(r+1, c));
            maze[r+1][c] = '-';
        }
        if(c != 0 && maze[r][c-1] == '.')   //WEST
        {
            coordqueue.push(Coord(r, c-1));
            maze[r][c-1] = '-';
        }
    }
    
    return false;
}

int main()
{
    string maze[10] = {
        "XXXXXXXXXX",
        "X........X",
        "XX.X.XXXXX",
        "X..X.X...X",
        "X..X...X.X",
        "XXXX.XXX.X",
        "X.X....XXX",
        "X..XX.XX.X",
        "X...X....X",
        "XXXXXXXXXX"
    };
    
    if (pathExists(maze, 10,10, 6,4, 1,1))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
    return 0;
}
