// SnakeGame.h
#pragma once
#include "Console.h"
#include <vector>
#include <conio.h>

class SnakeGame
{
    Console* console;
    WindowConsole* winCons;

    struct Point
    {
        int x{};
        int y{};

        Point() = default;
        Point(int x, int y) : x{ x }, y{ y } {}
        bool operator==(const Point& other)
        {
            return x == other.x && y == other.y;
        }
    };

    enum Direction { Up, Down, Left, Right };
    std::vector<Point> snake{};

    Direction currentDirection{};
    Direction nextDirection{};

    Point food;
    int score;
    bool gameOver;

public:
    SnakeGame(Console* console, WindowConsole* winCons) :
        console{ console }, winCons{ winCons }, gameOver{ false }, score{ 0 }
    {
        int centerX = winCons->Column() + winCons->Width() / 2;
        int centerY = winCons->Row() + winCons->Height() / 2;

        snake.push_back(Point{ centerX, centerY });
        snake.push_back(Point{ centerX - 1, centerY });
        snake.push_back(Point{ centerX - 2, centerY });

        currentDirection = Direction::Right;
    }

    void DrawSnake()
    {
        for (const Point& segment : snake)
            winCons->WriteGoto(segment.y, segment.x, 'O');
        winCons->WriteGoto(snake.front().y, snake.front().x, '@');
    }

    void Input()
    {
        // conio.h
        int key = _getch();
        if (key == 0 || key == 0xE0)
            key = _getch();

        switch (key)
        {
        case 27:
            //esc
            break;
        case 72: case 'w': case'W':
            // стрелка вверх
            if (currentDirection != Direction::Down)
                nextDirection = Direction::Up;
            break;
        }

    }
};