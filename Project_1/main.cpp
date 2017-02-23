#include <iostream>
#include "Game.h"

using namespace std;

class Game;

int main(int argc, char *argv[])
{
    // Create a game
          // Use this instead to create a mini-game:   Game g(3, 5, 2);
        Game g(10, 12, 40);

          // Play the game
        g.play();
		cin.get();
    return 0;
}
