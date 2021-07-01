/**
 * @file snake.cpp
 * @author atctwo
 * @brief implementation of snake game engine
 * @version 0.1
 * @date 2021-07-01
 * 
 * @copyright Copyright (c) 2021 atctwo
 * 
 */

#include "snake.h"

SnakeGame::SnakeGame(int16_t width, int16_t height)
{
    // save grid dimensions
    this->grid_width = width;
    this->grid_height = height;

    // set default settings
    this->loop_at_edge = false;
    this->collide_with_snake = true;

    // reset game
    this->reset();
}

void SnakeGame::reset()
{
    // clear snake segment and food arrays
    this->snake_segments.clear();
    this->food.clear();

    // add snake segments
    std::pair<int16_t, int16_t> segment;
    segment.first = this->grid_width / 2;
    segment.second = this->grid_height / 2;
    this->snake_segments.insert(this->snake_segments.begin(), segment);

    // add food
    this->spawnFood();

    // reset current direction
    this->current_direction = SNAKE_DIRECTION_LEFT;

    // reset game over details
    this->is_game_over = false;
    this->game_over_reason = SNAKE_GAME_OVER_NOT;

    // reset score
    this->score = 0;
}

void SnakeGame::update()
{
    if (!this->is_game_over)
    {

        // move the snake forward one block
        std::pair<int16_t, int16_t> new_segment;
        switch(this->current_direction)
        {
            case SNAKE_DIRECTION_UP:
                new_segment.first = this->snake_segments[0].first;
                new_segment.second = this->snake_segments[0].second - 1;
                break;

            case SNAKE_DIRECTION_DOWN:
                new_segment.first = this->snake_segments[0].first;
                new_segment.second = this->snake_segments[0].second + 1;
                break;
            
            default:
            case SNAKE_DIRECTION_LEFT:
                new_segment.first = this->snake_segments[0].first - 1;
                new_segment.second = this->snake_segments[0].second;
                break;

            case SNAKE_DIRECTION_RIGHT:
                new_segment.first = this->snake_segments[0].first + 1;
                new_segment.second = this->snake_segments[0].second;
                break;
        }
        this->snake_segments.insert(this->snake_segments.begin(), new_segment);

        // remove the last segment
        this->snake_segments.pop_back();

        // check for food
        for (int16_t i = 0; i < this->food.size(); i++)
        {
            if (this->snake_segments[0] == this->food[i])
            {
                // add segment to snake
                std::pair<int16_t, int16_t> new_segment;
                switch(this->current_direction)
                {
                    case SNAKE_DIRECTION_UP:
                        new_segment.first = this->snake_segments[0].first;
                        new_segment.second = this->snake_segments[0].second - 1;
                        break;

                    case SNAKE_DIRECTION_DOWN:
                        new_segment.first = this->snake_segments[0].first;
                        new_segment.second = this->snake_segments[0].second + 1;
                        break;
                    
                    default:
                    case SNAKE_DIRECTION_LEFT:
                        new_segment.first = this->snake_segments[0].first - 1;
                        new_segment.second = this->snake_segments[0].second;
                        break;

                    case SNAKE_DIRECTION_RIGHT:
                        new_segment.first = this->snake_segments[0].first + 1;
                        new_segment.second = this->snake_segments[0].second;
                        break;
                }
                this->snake_segments.insert(this->snake_segments.begin(), new_segment);

                // remove food
                this->food.erase(this->food.begin() + i);

                // spawn new food
                this->spawnFood();

                // increment score
                this->score++;
            }
        }

        // loop at edge
        if (this->loop_at_edge) // loop to the other side of the screen when the snake collides with the wall
        {
            for (int16_t i = 0; i < this->snake_segments.size(); i++)
            {
                if (this->snake_segments[i].first < 0) this->snake_segments[i].first = this->grid_width - 1;
                if (this->snake_segments[i].first >= this->grid_width) this->snake_segments[i].first = 0;
                if (this->snake_segments[i].second < 0) this->snake_segments[i].second = this->grid_height - 1;
                if (this->snake_segments[i].second >= this->grid_height) this->snake_segments[i].second = 0;
            }
        }
        else // end the game when the snake collides with the wall
        {
            for (int16_t i = 0; i < this->snake_segments.size(); i++)
            {
                if ( (this->snake_segments[i].first < 0) ||
                     (this->snake_segments[i].first >= this->grid_width) ||
                     (this->snake_segments[i].second < 0) ||
                     (this->snake_segments[i].second >= this->grid_height)
                ) {
                    // end the game
                    this->is_game_over = true;
                    this->game_over_reason = SNAKE_GAME_OVER_EDGE;
                }
            }
        }

        // check for snake collisions
        if (this->collide_with_snake)
        {
            for (int16_t i = 0; i < this->snake_segments.size(); i++)
            {
                for (int16_t j = 0; j < this->snake_segments.size(); j++)
                {
                    if (i != j)
                    {
                        if (this->snake_segments[i] == this->snake_segments[j])
                        {
                            // two snake segments are in the same space, so end the game
                            this->is_game_over = true;
                            this->game_over_reason = SNAKE_GAME_OVER_SNAKE;
                        }
                    }
                }
            }
        }

    }
}

void SnakeGame::spawnFood(int16_t fruit_to_spawn)
{
    srand(time(NULL));
    for (uint8_t i = 0; i < fruit_to_spawn; i++)
    {
        std::pair<int16_t, int16_t> food_segment;
        while(true)
        {
            // choose a random grid position
            food_segment.first = rand() % this->grid_width;
            food_segment.second = rand() % this->grid_height;

            // check if there is a snake segment there
            for (std::pair<int16_t, int16_t> snake_segment : this->snake_segments)
            {
                if (food_segment == snake_segment) continue;
            }

            // if we've reached this point, the food segment isn't in the same space as a snake segment
            break;
        }

        this->food.insert(this->food.begin(), food_segment);
    }
}

void SnakeGame::setSnakeDirection(SnakeDirection direction)
{
    if ( (this->current_direction == SNAKE_DIRECTION_LEFT && direction != SNAKE_DIRECTION_RIGHT) ||
         (this->current_direction == SNAKE_DIRECTION_RIGHT && direction != SNAKE_DIRECTION_LEFT) ||
         (this->current_direction == SNAKE_DIRECTION_UP && direction != SNAKE_DIRECTION_DOWN) ||
         (this->current_direction == SNAKE_DIRECTION_DOWN && direction != SNAKE_DIRECTION_UP)
    ) this->current_direction = direction;
}

std::vector<std::pair<int16_t, int16_t>> SnakeGame::getSnakeSegments() const
{
    return this->snake_segments;
}

std::vector<std::pair<int16_t, int16_t>> SnakeGame::getFood() const
{
    return this->food;
}

int16_t SnakeGame::getGridWidth()
{
    return this->grid_width;
}

int16_t SnakeGame::getGridHeight()
{
    return this->grid_height;
}

SnakeDirection SnakeGame::getSnakeDirection()
{
    return this->current_direction;
}

uint16_t SnakeGame::getScore()
{
    return this->score;
}

bool SnakeGame::isGameOver()
{
    return this->is_game_over;
}

SnakeGameOverReason SnakeGame::gameOverReason()
{
    return this->game_over_reason;
}

void SnakeGame::setLoopAtEdge(bool loop_at_edge)
{
    this->loop_at_edge = loop_at_edge;
}

void SnakeGame::setCollideWithSnake(bool collide_with_snake)
{
    this->collide_with_snake = collide_with_snake;
}