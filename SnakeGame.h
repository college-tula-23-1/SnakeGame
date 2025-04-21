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
        srand(time(NULL));
        int centerX = winCons->Column() + winCons->Width() / 2;
        int centerY = winCons->Row() + winCons->Height() / 2;

        snake.push_back(Point{ centerX, centerY });
        snake.push_back(Point{ centerX - 1, centerY });
        snake.push_back(Point{ centerX - 2, centerY });

        currentDirection = Direction::Right;
        nextDirection = Direction::Right;

        GenerateFood();
        winCons->Show();
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

    bool CheckCollision()
    {
        Point head = snake.front();

        if (head.x <= winCons->Column() ||  head.x >= winCons->Column() + winCons->Width() - 1 ||
            head.y <= winCons->Row() || head.y >= winCons->Row() + winCons->Height() - 1)
        {
            return true;
        }

        for (int i{ 1 }; i < snake.size(); ++i)
            if (head == snake[i])
                return true;

        return false;
    }

    void GenerateFood()
    {
        int positions = winCons->Width() * winCons->Height();
        int attempt = 0;
        bool validPosition = false;

        while (!validPosition && attempt < positions)
        {
            attempt++;
            Point p{
                winCons->Column() + 1 + rand() % (winCons->Width() - 2),
                winCons->Row() + 1 + rand() % (winCons->Height() - 2)
            };
            food.x = p.x;
            food.y = p.y;
            validPosition = true;

            for (Point segment : snake)
            {
                if (segment == p)
                {
                    validPosition = false;
                    break;
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
        winCons->Show();
        for (int y = winCons->Row() + 1; y < winCons->Row() + winCons->Height() - 1; ++y)
        {
            for (int x = winCons->Column() + 1; x < winCons->Column() + winCons->Width() - 1; ++x)
            {
                bool isSegment = false;
                for (Point segment : snake)
                    if (segment.x == x && segment.y == y)
                    {
                        isSegment = true;
                        break;
                    }
                if (!isSegment && !(food.x == x && food.y == y))
                    winCons->WriteGoto(y, x, ' ');
            }
        }

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
        if (CheckCollision())
        {
            gameOver = true;
        }
        if (snake.front() == food)
        {
            GenerateFood();
            Point backSegment = snake.back();
            snake.push_back(backSegment);
            score++;
        }
        DrawSnake();
        DrawFood();
    }

    void Run()
    {
        Start();
        while (!gameOver)
        {
            Update();
            Sleep(100);
        }

        winCons->WriteGoto(winCons->Row() + winCons->Height() / 2,
            winCons->Column() + winCons->Width() / 2 - 12,
            "GameOver! Your score is: " + std::to_string(score));
        std::cin.get();
    }
};