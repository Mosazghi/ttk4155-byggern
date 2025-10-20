#pragma once

#include <stdbool.h>

#include "avr.h"

typedef enum {
  ON_CALIBRATE_EVT,
  OFF_CALIBRATE_EVT,
  ON_DIFFICULTY_EVT,
  OFF_DIFFICULTY_EVT,
  ON_GAME_EVT,
  OFF_GAME_EVT,
  ON_SCORE_EVT,
  OFF_SCORE_EVT,
  INCREMENT_UP_EVT,
  INCREMENT_DOWN_EVT,
  SELECT_EVT,
  MOVE_UP_EVT,
  MOVE_DOWN_EVT,
  MOVE_LEFT_EVT,
  MOVE_RIGHT_EVT,
} event_t;

typedef enum {
  MENU,
  GAME,
  SCREEN_CNT,
} screen_t;

struct button_mapping_rules {
  screen_t screen;
  buttons_t button;
  event_t event;
};

typedef struct state_machine state_machine_t;

typedef void (*state)(state_machine_t* machine, event_t input);

struct state_machine_ctx {
  screen_t screen;
  uint8_t cursor;
};

struct state_machine {
  state current_state;
  struct state_machine_ctx ctx;
};

/*******************************************************************/
/*                        State functions                          */
/*******************************************************************/

void state_menu(state_machine_t* mahcine, event_t input);
void state_calibrate(state_machine_t* mahcine, event_t input);
void state_difficulty(state_machine_t* mahcine, event_t input);
void state_game(state_machine_t* mahcine, event_t input);
void state_score(state_machine_t* mahcine, event_t input);

// typedef enum { LVL_EASY, LVL_MEDIUM, LVL_HARD } difficulty_level_t;
// typedef struct {
//   difficulty_level_t current_level;
//   int score;
//   int lives;
//   bool is_in_game;
// } game_state_t;
//
// extern game_state_t g_game_state;
//
// void game_loop(buttons_t *buttons);
// void set_difficulty_level(difficulty_level_t level);
// void reset_high_scores();
