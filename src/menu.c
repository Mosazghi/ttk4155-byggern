#include "menu.h"
#include "utility.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

void set_difficulty_level(uint8_t level);

static void return_menu(void) {
  // This function is intentionally left blank.
  // The actual return logic is handled in menu_select.
}

static void reset_high_score(void) {
  // TODO: Logic
  LOG_INF("High score has been reset!\n");
}

// Solution: Initialize without parent, then set parent after declaration
static menu_item_t difficulty_items[] = {
    {"Easy", NULL, NULL},
    {"Medium", NULL, NULL},
    {"Hard", NULL, NULL},
    {"Return", NULL, NULL},
};

static menu_t difficulty_menu = {
    .header = "Choose Difficulty",
    .title = "New Game",
    .items = difficulty_items,
    .num_items = 4,
    .parent = NULL // Will be set in setup function
};
static menu_item_t score_items[] = {
    {"Reset High Scores", NULL, NULL},
    {"Return", NULL, NULL},
};

static menu_t scor_menu = {
    .header = "High Scores",
    .title = "High Scores",
    .items = score_items,
    .num_items = 2,
    .parent = NULL // Will be set in setup function
};

static menu_item_t main_items[] = {{"New Game", &difficulty_menu, NULL},
                                   {"High Scorue", NULL, NULL},
                                   {"Calibrate Joystick", NULL, NULL},
                                   {"Debug", NULL, NULL}};

menu_t menu_root = {
    .header = "Ping Pong",
    .title = "Main Menu",
    .items = main_items,
    .num_items = 4,
    .parent = NULL,
};

// Global menu state
static menu_state_t menu_data_state;

static void setup_menu_structure(void) {
  // NEW GAME ---
  // Set up parent relationships AFTER all menus are declared
  difficulty_menu.parent = &menu_root;

  // Set up callbacks with proper parameters if needed
  // Note: Your callback signature might need adjustment for level parameter
  difficulty_items[0].callback = (void (*)(void))set_difficulty_level;
  difficulty_items[1].callback = (void (*)(void))set_difficulty_level;
  difficulty_items[2].callback = (void (*)(void))set_difficulty_level;
  difficulty_items[3].callback = (void (*)(void))return_menu;
  // SCORE ---
  scor_menu.parent = &menu_root;
  score_items[0].callback = &reset_high_score;
  score_items[1].callback = (void (*)(void))return_menu;

  // TODO: DEBUG ---
}

void menu_init(menu_state_t *state, menu_t *root) {
  (void)root;
  setup_menu_structure();
  state->current_menu = &menu_root;
  state->current_index = 0;
}

void menu_move_up(menu_state_t *state) {
  if (state->current_index > 0) {
    state->current_index--;
  } else
    state->current_index = state->current_menu->num_items - 1;
}

void menu_move_down(menu_state_t *state) {
  if (state->current_index < state->current_menu->num_items - 1) {
    state->current_index++;
  } else
    state->current_index = 0;
}

bool menu_select(menu_state_t *state) {
  menu_item_t *item = &state->current_menu->items[state->current_index];

  // Enter submenu
  if (item->sub_menu != NULL) {
    state->current_menu = item->sub_menu;
    state->current_index = 0;
    return true;
  }

  // Return to parent menu
  else if (item->sub_menu == NULL && state->current_menu->parent != NULL &&
           strcmp(item->label, "Return") == 0) {
    state->current_menu = state->current_menu->parent;
    state->current_index = 0;
    return true;
  }

  // Execute callback function
  else if (item->callback != NULL) {
    item->callback();
    return true;
  }

  return false;
};

// void menu_test_loop(void) {
//     joystick_xy_t js = avr_get_joystick();

//     if (js.y > 40) {
//         menu_move_up(&menu_data.state);
//     }
//     else if (js.y < -40) {
//         menu_move_down(&menu_data.state);
//     }

//     if (js.btn == 1) {
//         menu_select(&menu_data.state);
//         LOG_INF("Selected: %s\n",
//             menu_data.state.current_menu->items[menu_data.state.current_index].label);
//     }

//     LOG_INF("Current Menu: %s\n", menu_data.state.current_menu->title);
//     LOG_INF("Current Selection: %s\n",
//             menu_data.state.current_menu->items[menu_data.state.current_index].label);
// }

void hat_test_loop(buttons_t buttons) {
  if (buttons.NU == 1) {
    menu_move_up(&menu_data_state);
  } else if (buttons.ND == 1) {
    menu_move_down(&menu_data_state);
  }
  if (buttons.NB == 1) {
    menu_select(&menu_data_state);
    LOG_INF("Selected: %s\n",
            menu_data_state.current_menu->items[menu_data_state.current_index]
                .label);
  }
  LOG_INF("Current Menu: %s\n", menu_data_state.current_menu->title);
  LOG_INF(
      "Current Selection: %s\n",
      menu_data_state.current_menu->items[menu_data_state.current_index].label);
}
