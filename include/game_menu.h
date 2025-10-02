#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "avr.h"

// Forward declarations
typedef struct menu_t menu_t;
typedef struct menu_item_t menu_item_t;
typedef struct menu_state_t menu_state_t;

// Structure definitions
struct menu_item_t {
  const char *label;
  menu_t *sub_menu;
  void (*callback)(void);
};

struct menu_t {
  const char *header;
  const char *title;
  menu_item_t *items;
  uint8_t num_items;
  menu_t *parent;
};
typedef enum {
  MAIN_MENU,
  NEW_GAME_MENU,
  SCORES_MENU,
  CALIBRATE_MENU,
  DEBUG_MENU,
} menu_render_t;

struct menu_state_t {
  menu_t *current_menu;
  uint8_t current_index;
  menu_render_t current_render;
  bool is_playing;
};

void menu_init();
void menu_move_up(menu_state_t *state);
void menu_move_down(menu_state_t *state);
bool menu_select(menu_state_t *state);
void hat_test_loop(buttons_t buttons);
void menu_loop(buttons_t buttons);
void update_display();
void update_menu_state(buttons_t buttons);
// void menu_test_loop(void);
