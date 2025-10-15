#pragma once

#include <stdbool.h>

#include "input.h"

typedef enum { LVL_EASY, LVL_MEDIUM, LVL_HARD } difficulty_level_t;

typedef struct {
  difficulty_level_t current_level;
  int score;
  int lives;
  bool is_in_game;
} game_state_t;

void game_loop(game_state_t *state, joystick_xy_t *joystick);
void reset_high_scores();
void render_game(game_state_t *state);
void try_start_game(difficulty_level_t level, game_state_t *state);

// Global game state variable
extern game_state_t g_game_state;
