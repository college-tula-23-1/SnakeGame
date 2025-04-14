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
        nextDirection = Direction::Right;
    }

    void Move()
    {
        currentDirection = nextDirection;
        Point newHead = snake.front();
        switch (currentDirection)
        {
        case Direction::Up: newHead.y--; break;
        case Direction::Down: newHead.y++; break;
        case Direction::Right: newHead.x++; break;
        case Direction::Left: newHead.x--; break;
        }
        snake.insert(snake.begin(), newHead);

        snake.pop_back();
    }

    void GenerateFood()
    {
        int positions = winCons->Width() * winCons->Height();
        int attempt = 0;

        while (attempt < positions)
        {
            attempt++;
            Point p{
                winCons->Column() + 1 + rand() % (winCons->Width() - 2),
                winCons->Row() + 1 + rand() % (winCons->Height() - 2)
            };

            for (Point segment : snake)
            {
                if (segment == p)
                {

                }
            }
        }
    }

    void DrawFood()
    {
        winCons->WriteGoto(food.y, food.x, '$');
    }

    void DrawSnake()
    {
        system("cls");
        winCons->Show();

        for (const Point& segment : snake)
            winCons->WriteGoto(segment.y, segment.x, 'O');
        winCons->WriteGoto(snake.front().y, snake.front().x, '@');
    }

    void Input()
    {
        if (_kbhit())
        {
            switch (_getch())
            {
            case 27:
                //esc
                gameOver = true;
                break;
                // стрелки и wasd
            case 72: case 'w': case'W':
                if (currentDirection != Direction::Down)
                    nextDirection = Direction::Up;
                break;
            case 80: case 's': case'S':
                if (currentDirection != Direction::Up)
                    nextDirection = Direction::Down;
                break;
            case 75: case 'a': case'A':
                if (currentDirection != Direction::Right)
                    nextDirection = Direction::Left;
                break;
            case 77: case 'd': case'D':
                if (currentDirection != Direction::Left)
                    nextDirection = Direction::Right;
                break;
            }
        }
    }
    void Start()
    {
        gameOver = false;
        score = 0;
        system("cls");

        DrawSnake();
    }

    void Update()
    {
        if (gameOver)
            return;

        Input();
        Move();

        DrawSnake();
        DrawFood();
    }

    void Run()
    {
        Start();
        while (!gameOver)
        {
            Update();
            Sleep(200);
        }
    }
};