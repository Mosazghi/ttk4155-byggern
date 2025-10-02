#include "game_menu.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "oled.h"
#include "utility.h"

uint8_t set_difficulty_level(uint8_t level) {
  int x = level;
  return x;
}

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
    .parent = NULL  // Will be set in setup function
};
static menu_item_t score_items[] = {
    {"Reset High Scores", NULL, NULL},
    {"Return", NULL, NULL},
};

static menu_t score_menu = {
    .header = "High Scores",
    .title = "High Scores",
    .items = score_items,
    .num_items = 2,
    .parent = NULL  // Will be set in setup function
};

static menu_item_t main_items[] = {{"New Game", &difficulty_menu, NULL},
                                   {"High Scorue", NULL, NULL},
                                   {"Calibrate Joystick", NULL, NULL},
                                   {"Debug", NULL, NULL}};

// GLOBAL MENU STATES
static menu_t g_menu_root = {
    .header = "Ping Pong",
    .title = "Main Menu",
    .items = main_items,
    .num_items = 4,
    .parent = NULL,
};
static menu_state_t g_menu_state;
// ------------------

static void setup_menu_structure(void) {
  // NEW GAME ---
  // Set up parent relationships AFTER all menus are declared
  difficulty_menu.parent = &g_menu_root;

  // Set up callbacks with proper parameters if needed
  // Note: Your callback signature might need adjustment for level parameter
  difficulty_items[0].callback = NULL;  //(void (*)(void))set_difficulty_level;
  difficulty_items[1].callback = NULL;
  difficulty_items[2].callback = NULL;
  difficulty_items[3].callback = NULL;
  // SCORE ---
  score_menu.parent = &g_menu_root;
  score_items[0].callback = NULL;
  score_items[1].callback = NULL;

  // TODO: DEBUG ---
  // MENU STATE ---
  g_menu_state.current_menu = &g_menu_root;
  g_menu_state.is_playing = false;
  g_menu_state.current_render = MAIN_MENU;
  g_menu_state.current_index = 0;
}

void menu_init() {
  // (void)root;
  setup_menu_structure();
}

void update_menu_state(buttons_t buttons) {
  if (buttons.NU == 1) {
    LOG_INF("UP\n");
    menu_move_up(&g_menu_state);
  } else if (buttons.ND == 1) {
    LOG_INF("DOWN\n");
    menu_move_down(&g_menu_state);
  }
  if (buttons.NB == 1) {
    LOG_INF("BUTTON\n");
    menu_select(&g_menu_state);
    // LOG_INF("Selected: %s\n",
    // g_menu_state.current_menu->items[g_menu_state.current_index].label);
  }
}

void menu_loop(buttons_t buttons) {
  update_menu_state(buttons);
  // update_display();
}

void update_display() {
  char display_buffer[128];
  LOG_INF("Current Menu: %s\n", g_menu_state.current_menu->title);
  switch (g_menu_state.current_render) {
    case MAIN_MENU:
      snprintf(display_buffer, sizeof(display_buffer), "Menu: %s",
               g_menu_state.current_menu->title);

      oled_printf(display_buffer, 0, 0);
      // print its items
      for (uint8_t i = 0; i < g_menu_state.current_menu->num_items; i++) {
        if (i == g_menu_state.current_index) {
          snprintf(display_buffer, sizeof(display_buffer), "> %s",
                   g_menu_state.current_menu->items[i].label);
        } else {
          snprintf(display_buffer, sizeof(display_buffer), "  %s",
                   g_menu_state.current_menu->items[i].label);
        }
        oled_printf(display_buffer, 0, i + 1);  // +1 to account for header
      }
      break;
    case NEW_GAME_MENU:
      oled_printf("New Game Menu", 0, 0);
      break;
    case SCORES_MENU:
      oled_printf("Scores Menu", 0, 0);
      break;
    case CALIBRATE_MENU:
      oled_printf("Calibrate Menu", 0, 0);
      break;
    case DEBUG_MENU:
      oled_printf("Debug Menu", 0, 0);
      break;
    default:
      break;
  }
  oled_display();
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

void hat_test_loop(buttons_t buttons) {
  if (buttons.NU == 1) {
    menu_move_up(&g_menu_state);
  } else if (buttons.ND == 1) {
    menu_move_down(&g_menu_state);
  }
  if (buttons.NB == 1) {
    menu_select(&g_menu_state);
    LOG_INF("Selected: %s\n", g_menu_state.current_menu->items[g_menu_state.current_index].label);
  }
  LOG_INF("Current Menu: %s\n", g_menu_state.current_menu->title);
  LOG_INF("Current Selection: %s\n",
          g_menu_state.current_menu->items[g_menu_state.current_index].label);
}
