/**
 * @file raylib_example.cpp
 * @author atctwo
 * @brief example showing use of Snake Engine, using raylib as a frontend
 * @version 0.1
 * @date 2021-07-01
 * 
 * heavily based on the Basic Window example from https://www.raylib.com/examples.html
 * 
 * @copyright Copyright (c) 2021 atctwo
 * 
 */



#include "raylib.h"
#include "snake.h"
#include <string>

int main(void)
{
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 400;

    const int snake_width = 60;             // the number of columns
    const int snake_height = 30;            // the number of rows
    double update_time = 0.1;         // the number of seconds between updates / the snake moving
    double last_update_time = 0;      // time since the last update
    bool paused = false;              // is the game paused?

    // set up raylib window
    InitWindow(screenWidth, screenHeight, "Snake Game Engine Raylib Frontend");
    SetTargetFPS(60);

    // initalise snake
    SnakeGame snake(snake_width, snake_height);
    snake.setLoopAtEdge(true);
    //snake.setCollideWithSnake(false);

    const int snake_grid_width = screenWidth / snake_width;         // width of each column in pixels
    const int snake_grid_height = screenHeight / snake_height;      // width of each row in pixels

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update / move snake
        // if the last update was more than update_time seconds ago, call snake.update() and reset the last update time
        if (!paused && (GetTime() - last_update_time > update_time))
        {
            snake.update();
            last_update_time = GetTime();
        }

        // set snake direction
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))        snake.setSnakeDirection(SNAKE_DIRECTION_UP);
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))      snake.setSnakeDirection(SNAKE_DIRECTION_DOWN);
        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))      snake.setSnakeDirection(SNAKE_DIRECTION_LEFT);
        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))     snake.setSnakeDirection(SNAKE_DIRECTION_RIGHT);

        // reset game
        if (IsKeyPressed(KEY_R)) snake.reset();

        // pause game
        if (IsKeyPressed(KEY_P)) paused = !paused;

        // Draw
        BeginDrawing();

            ClearBackground(RAYWHITE);

            // draw snake grid (the background grid)
            for (int y = 0; y < snake.getGridHeight(); y++)
            {
                for (int x = 0; x < snake.getGridWidth(); x++)
                {
                    DrawRectangleLines(x * snake_grid_width, y * snake_grid_height, snake_grid_width, snake_grid_height, LIGHTGRAY);
                }
            }

            // draw snake segments
            std::vector<std::pair<int16_t, int16_t>> segments = snake.getSnakeSegments();
            for (std::pair<int16_t, int16_t> segment : segments)
            {
                DrawRectangle(
                    segment.first * snake_grid_width, 
                    segment.second * snake_grid_height, 
                    snake_grid_width, snake_grid_height, BLUE
                );
            }

            // draw food
            std::vector<std::pair<int16_t, int16_t>> food = snake.getFood();
            for (std::pair<int16_t, int16_t> f : food)
            {
                DrawRectangle(
                    f.first * snake_grid_width, 
                    f.second * snake_grid_height, 
                    snake_grid_width, snake_grid_height, RED
                );
            }

            // draw score
            DrawText("Score: ", 2, 2, 20, BLACK);
            DrawText(std::to_string(snake.getScore()).c_str(), 70, 2, 20, BLACK);

            // if the game is over, draw the game over reason
            if (snake.isGameOver())
            {
                DrawText("Game Over!", 20, 20, 50, BLACK);
                switch(snake.gameOverReason())
                {
                    case SNAKE_GAME_OVER_SNAKE:
                        DrawText("You collided with the snake", 20, 70, 30, BLACK);
                        break;

                    case SNAKE_GAME_OVER_EDGE:
                        DrawText("You collided with the edge", 20, 70, 30, BLACK);
                        break;

                    case SNAKE_GAME_OVER_OTHER:
                        DrawText("The game ended for some reason", 20, 70, 30, BLACK);
                        break;
                }
                DrawText("Press R to continue", 20, 100, 30, BLACK);
            }

            // if the game is paused, show a message
            if (paused) DrawText("Paused", 20, 20, 30, BLACK);

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();        // Close window and OpenGL context

    return 0;
}