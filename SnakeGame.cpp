#include <iostream>
#include "Console.h"
#include "SnakeGame.h"

int main()
{
    Console console;
    WindowConsole gameWin{ &console, 0, 0, 20, 40 };

    SnakeGame game{ &console, &gameWin };

    game.Run();
}