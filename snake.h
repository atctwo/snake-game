/**
 * @file snake.h
 * @author atctwo
 * @brief header file for snake game engine
 * @version 0.1
 * @date 2021-07-01
 * 
 * @copyright Copyright (c) 2021 atctwo
 * 
 */

#ifndef SNAKE_ENGINE_H
#define SNAKE_ENGINE_H

#include <inttypes.h>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <utility>

/**
 * @brief the direction the snake is moving
 * 
 */
enum SnakeDirection {

    SNAKE_DIRECTION_UP,
    SNAKE_DIRECTION_DOWN,
    SNAKE_DIRECTION_LEFT,
    SNAKE_DIRECTION_RIGHT

};

/**
 * @brief reasons why the game ended
 * 
 */
enum SnakeGameOverReason {

    SNAKE_GAME_OVER_NOT,        // the game hasn't ended
    SNAKE_GAME_OVER_OTHER,      // the game ended for some reason
    SNAKE_GAME_OVER_SNAKE,      // the snake collided with itself
    SNAKE_GAME_OVER_EDGE        // the snake collided with the edge

};

/**
 * @brief the main game engine
 * 
 */
class SnakeGame {

    public:

        /**
         * @brief Construct a new Snake Game object
         * 
         * @param width the number of collumns in the grid
         * @param height the number of rows in the grid
         */
        SnakeGame(int16_t width, int16_t height);

        /**
         * @brief Get the Grid Width
         * @return int16_t the grid width
         */
        int16_t getGridWidth();

        /**
         * @brief Get the Grid Height
         * @return int16_t the grid height
         */
        int16_t getGridHeight();

        /**
         * @brief Get the direction the snake is currently moving in
         * @return SnakeDirection 
         */
        SnakeDirection getSnakeDirection();

        /**
         * @brief Get the current score
         * each food that the snake eats increases the score by 1
         * @return uint16_t the current score
         */
        uint16_t getScore();

        /**
         * @brief Get the segments that make up the snake
         * this returns an std::vector of std::pair<int16_t, int16_t>, where the first element
         * is the segment's x component, and the second element is the segment's y component.  each
         * segment is represented as one element in the vector.
         * @return std::vector<std::pair<int16_t, int16_t>> 
         */
        std::vector<std::pair<int16_t, int16_t>> getSnakeSegments() const;

        /**
         * @brief Get the current food positions
         * this returns a std::vector of std::pair<int16_t, int16_t>, where the first element
         * is the food's x component, and the second element is the food's y component.  each
         * bit of food is represented as one element in the vector.
         * @return std::vector<std::pair<int16_t, int16_t>> 
         */
        std::vector<std::pair<int16_t, int16_t>> getFood() const;

        /**
         * @brief Set the snake's direction
         * @param direction the direction for the snake to move in
         */
        void setSnakeDirection(SnakeDirection direction);

        /**
         * @brief update the snake
         * this method does a few things:
         * - moves the snake forward by one square
         * - checks if the snake is at a bit of food
         * - checks if the snake is at the edge of the grid
         * - checks if the snake has collided with itself
         * this method should be called however fast you want the snake to move (ie: if you want the snake to move every 100ms, call this
         * method every 100 ms).
         */
        void update();

        /**
         * @brief returns whether or not the game is over
         * @return true the game is over
         * @return false the game is not over
         */
        bool isGameOver();

        /**
         * @brief returns the reason the game ended
         * if the game has not ended, this will return SNAKE_GAME_OVER_NOT.  otherwise, this will return the reason the
         * game ended
         * @return SnakeGameOverReason the reason the game ended
         */
        SnakeGameOverReason gameOverReason();

        /**
         * @brief resets the entire game
         * this resets the entire game engine.  the snake and food will be reset, the score will be set to 0, and any game over flags
         * will be cleared, so the game can start from scratch.
         */
        void reset();

        /**
         * @brief spawns a bit of food at a random place on the grid
         * food won't be spawned on top of the snake
         * @param fruit_to_spawn how many bits of food to spawn
         */
        void spawnFood(int16_t fruit_to_spawn=1);

        /**
         * @brief Enable or disable edge looping
         * If this is set to true, when the snake meets the edge of the grid, it will wrap around, and appear at the opposite edge (it will
         * loop the screen).  If this is false, when the snake meets the edge, the game will end, and the game over reason will be set to
         * SNAKE_GAME_OVER_EDGE.  This is false by default
         * @param loop_at_edge whether or not to loop at the edge of the screen
         */
        void setLoopAtEdge(bool loop_at_edge);

        /**
         * @brief Enable or disable collisions with the snake
         * If this is true, when the snake collides with itself, the game will end, and the game over reason will be set to SNAKE_GAME_OVER_SNAKE.
         * If this is false, when the snake collides with itself, the snake will pass over itself, and the game will continue.
         * This is false by default.
         * @param collide_with_snake whether or not to detect collisions with the snake 
         */
        void setCollideWithSnake(bool collide_with_snake);

    private:

        bool loop_at_edge;
        bool collide_with_snake;

        int16_t grid_width;
        int16_t grid_height;
        SnakeDirection current_direction;
        uint16_t score;
        std::vector<std::pair<int16_t, int16_t>> snake_segments;
        std::vector<std::pair<int16_t, int16_t>> food;

        bool is_game_over;
        SnakeGameOverReason game_over_reason;
        
};


#endif // SNAKE_ENGINE_H