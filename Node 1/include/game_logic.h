#pragma once

#include <stdbool.h>

#include "input.h"

/**
 * @brief Difficulty levels for the game
 */
typedef enum { LVL_EASY, LVL_MEDIUM, LVL_HARD } difficulty_level_t;

/**
 * @brief Game state structure
 */
typedef struct {
  difficulty_level_t current_level;
  int score;
  int lives;
  bool is_in_game;
  bool new_can_msg;
} game_state_t;

/**
 * @brief Main game loop function
 * @param state Pointer to the current game state
 * @param joystick Pointer to the current joystick position
 */
void game_loop(game_state_t *state, joystick_xy_t *joystick);
/**
 * @brief Resets the high scores
 * @remarks This function resets the global game state's score to zero.
 */
void reset_high_scores();
/**
 * @brief Renders the game state
 * @param state Pointer to the current game state
 */
void render_game(game_state_t *state);
/**
 * @brief Attempts to start the game with the specified difficulty level
 * @param level The difficulty level to start the game with
 * @param state Pointer to the current game state
 */
void try_start_game(difficulty_level_t level, game_state_t *state);

// Global game state variable
extern game_state_t g_game_state;
