#pragma once

#include <stdbool.h>

#include "avr.h"

typedef enum { LVL_EASY, LVL_MEDIUM, LVL_HARD } difficulty_level_t;
typedef struct {
  difficulty_level_t current_level;
  int score;
  int lives;
  bool is_in_game;
} game_state_t;

extern game_state_t g_game_state;

void game_loop(buttons_t *buttons);
void set_difficulty_level(difficulty_level_t level);
void reset_high_scores();