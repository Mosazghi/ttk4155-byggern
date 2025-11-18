#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "avr.h"

/**
 * @brief Menu render states
 */
typedef enum {
  MAIN_MENU,
  NEW_GAME_MENU,
  SCORES_MENU,
  CALIBRATE_MENU,
  DEBUG_MENU,
} menu_render_t;

// Forward declarations
typedef struct menu_t menu_t;
typedef struct menu_item_t menu_item_t;
typedef struct menu_state_t menu_state_t;

struct menu_t {
  const char *header;
  const char *title;
  menu_item_t *items;
  uint8_t num_items;
  menu_t *parent;
};

struct menu_item_t {
  const char *label;
  menu_t *sub_menu;
  void (*callback)(void);
};

struct menu_state_t {
  menu_t *current_menu;
  uint8_t current_index;
  menu_render_t current_render;
};

/**
 * @brief Initializes the menu system
 */
void menu_init();
/**
 * @brief Moves the menu selection up
 * @param state Pointer to the current menu state
 */
void menu_move_up(menu_state_t *state);
/**
 * @brief Moves the menu selection down
 * @param state Pointer to the current menu state
 */
void menu_move_down(menu_state_t *state);
/**
 * @brief Selects the current menu item
 * @param state Pointer to the current menu state
 */
void menu_select(menu_state_t *state);
/**
 * @brief Main menu loop function
 * @param buttons Pointer to the current button states
 */
void menu_loop(buttons_t *buttons);
/**
 * @brief Renders the current menu state to the display
 * @remarks Uses the global g_menu_state variable.
 * TODO: Pass as parameter instead?
 */
void render_menu();
// void menu_test_loop(void);
