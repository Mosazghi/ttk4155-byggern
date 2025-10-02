#include "game_menu.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "game_logic.h"
#include "oled.h"
#include "utility.h"

static menu_render_t str_to_render_t(const char *str);
static void set_easy_difficulty(void) { set_difficulty_level(LVL_EASY); }
static void set_medium_difficulty(void) { set_difficulty_level(LVL_MEDIUM); }
static void set_hard_difficulty(void) { set_difficulty_level(LVL_HARD); }

static void return_menu(void) {
  // if (g_menu_state.current_menu->parent != NULL) {
  //   g_menu_state.current_menu = g_menu_state.current_menu->parent;
  //   g_menu_state.current_index = 0;
  // }
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
    .num_items = ARR_LEN(difficulty_items),
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
    .num_items = ARR_LEN(score_items),
    .parent = NULL  // Will be set in setup function
};

static menu_item_t debug_items[] = {
    {"(Placeholder) Debug Option 1", NULL, NULL},
    {"(Placeholder) Debug Option 2", NULL, NULL},
    {"Return", NULL, NULL},
};

static menu_t debug_menu = {
    .header = "Debug Menu",
    .title = "Debug Options",
    .items = debug_items,
    .num_items = ARR_LEN(debug_items),
    .parent = NULL  // Will be set in setup function
};

static menu_item_t calibrate_items[] = {
    {"(Placeholder) Calibrate Option 1", NULL, NULL},
    {"(Placeholder) Calibrate Option 2", NULL, NULL},
    {"Return", NULL, NULL},
};
static menu_t calibrate_menu = {
    .header = "Calibrate Joystick",
    .title = "Calibrate",
    .items = calibrate_items,
    .num_items = ARR_LEN(calibrate_items),
    .parent = NULL  // Will be set in setup function
};

static menu_item_t main_items[] = {{"New Game", &difficulty_menu, NULL},
                                   {"High Scores", &score_menu, NULL},
                                   {"Calibrate Joystick", &calibrate_menu, NULL},
                                   {"Debug", &debug_menu, NULL}};

// GLOBAL MENU STATES
static menu_t g_menu_root = {
    .header = "Ping Pong",
    .title = "Main Menu",
    .items = main_items,
    .num_items = ARR_LEN(main_items),
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
  difficulty_items[0].callback = set_easy_difficulty;
  difficulty_items[1].callback = set_medium_difficulty;
  difficulty_items[2].callback = set_hard_difficulty;
  difficulty_items[3].callback = NULL;
  // SCORE ---
  score_menu.parent = &g_menu_root;
  score_items[0].callback = reset_high_score;
  score_items[1].callback = return_menu;

  // TODO: DEBUG ---
  // MENU STATE ---
  g_menu_state.current_menu = &g_menu_root;
  // g_menu_state.is_playing = false;
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
  char display_buffer[127];
  // Clear
  oled_clear();
  LOG_INF("Current Menu: %s\n", g_menu_state.current_menu->title);
  snprintf(display_buffer, sizeof(display_buffer), "%s", g_menu_state.current_menu->header);
  // Draw
  oled_printf(display_buffer, 0, 0);
  switch (g_menu_state.current_render) {
    case MAIN_MENU:
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
      break;
    case SCORES_MENU:
      snprintf(display_buffer, sizeof(display_buffer), "High Score: %d", g_game_state.score);
      oled_printf(display_buffer, 0, 1);
      break;
    case CALIBRATE_MENU:
      break;
    case DEBUG_MENU:
      break;
    default:
      break;
  }

  // Display
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

void menu_select(menu_state_t *state) {
  menu_item_t *item = &state->current_menu->items[state->current_index];

  // Enter submenu
  if (item->sub_menu != NULL) {
    state->current_menu = item->sub_menu;
    state->current_render = str_to_render_t(item->label);
    state->current_index = 0;
  }
  // Return to parent menu
  else if (item->sub_menu == NULL && state->current_menu->parent != NULL &&
           strcmp(item->label, "Return") == 0) {
    state->current_menu = state->current_menu->parent;
    state->current_index = 0;
  }
  // Execute callback function
  else if (item->callback != NULL) {
    item->callback();
  }
};

static menu_render_t str_to_render_t(const char *str) {
  if (strcmp(str, "MAIN_MENU") == 0) {
    return MAIN_MENU;
  } else if (strcmp(str, "NEW_GAME_MENU") == 0) {
    return NEW_GAME_MENU;
  } else if (strcmp(str, "SCORES_MENU") == 0) {
    return SCORES_MENU;
  } else if (strcmp(str, "CALIBRATE_MENU") == 0) {
    return CALIBRATE_MENU;
  } else if (strcmp(str, "DEBUG_MENU") == 0) {
    return DEBUG_MENU;
  } else {
    LOG_ERR("Unknown menu render state: %s", str);
    return MAIN_MENU;  // Default case
  }
}