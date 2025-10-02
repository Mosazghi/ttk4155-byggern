#include "game_menu.h"

#include <avr/pgmspace.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "game_logic.h"
#include "oled.h"
#include "utility.h"

// Store strings in flash memory to save RAM
const char str_easy[] PROGMEM = "Easy";
const char str_medium[] PROGMEM = "Medium";
const char str_hard[] PROGMEM = "Hard";
const char str_return[] PROGMEM = "Return";
const char str_new_game[] PROGMEM = "New Game";
const char str_high_scores[] PROGMEM = "High Scores";
const char str_calibrate[] PROGMEM = "Calibrate Joystick";
const char str_debug[] PROGMEM = "Debug";
const char str_reset_scores[] PROGMEM = "Reset High Scores";
const char str_choose_difficulty[] PROGMEM = "Choose Difficulty";
const char str_high_scores_title[] PROGMEM = "High Scores";
const char str_debug_menu[] PROGMEM = "Debug Menu";
const char str_debug_options[] PROGMEM = "Debug Options";
const char str_calibrate_title[] PROGMEM = "Calibrate";
const char str_ping_pong[] PROGMEM = "Ping Pong";
const char str_main_menu[] PROGMEM = "Main Menu";
const char str_placeholder1[] PROGMEM = "Debug Opt 1";
const char str_placeholder2[] PROGMEM = "Debug Opt 2";
const char str_cal_opt1[] PROGMEM = "Cal Opt 1";
const char str_cal_opt2[] PROGMEM = "Cal Opt 2";

menu_render_t str_to_render_t(const char *str);
void set_easy_difficulty(void) { set_difficulty_level(LVL_EASY); }
void set_medium_difficulty(void) { set_difficulty_level(LVL_MEDIUM); }
void set_hard_difficulty(void) { set_difficulty_level(LVL_HARD); }

// Solution: Initialize without parent, then set parent after declaration
menu_item_t difficulty_items[] = {
    {str_easy, NULL, NULL},
    {str_medium, NULL, NULL},
    {str_hard, NULL, NULL},
    {str_return, NULL, NULL},
};

menu_t difficulty_menu = {
    .header = str_choose_difficulty,
    .title = str_new_game,
    .items = difficulty_items,
    .num_items = ARR_LEN(difficulty_items),
    .parent = NULL  // Will be set in setup function
};

menu_item_t score_items[] = {
    {str_reset_scores, NULL, NULL},
    {str_return, NULL, NULL},
};

menu_t score_menu = {
    .header = str_high_scores_title,
    .title = str_high_scores_title,
    .items = score_items,
    .num_items = ARR_LEN(score_items),
    .parent = NULL  // Will be set in setup function
};

menu_item_t debug_items[] = {
    {str_placeholder1, NULL, NULL},
    {str_placeholder2, NULL, NULL},
    {str_return, NULL, NULL},
};

menu_t debug_menu = {
    .header = str_debug_menu,
    .title = str_debug_options,
    .items = debug_items,
    .num_items = ARR_LEN(debug_items),
    .parent = NULL  // Will be set in setup function
};

menu_item_t calibrate_items[] = {
    {str_cal_opt1, NULL, NULL},
    {str_cal_opt2, NULL, NULL},
    {str_return, NULL, NULL},
};

menu_t calibrate_menu = {
    .header = str_calibrate,
    .title = str_calibrate_title,
    .items = calibrate_items,
    .num_items = ARR_LEN(calibrate_items),
    .parent = NULL  // Will be set in setup function
};

menu_item_t main_items[] = {{str_new_game, &difficulty_menu, NULL},
                            {str_high_scores, &score_menu, NULL},
                            {str_calibrate, &calibrate_menu, NULL},
                            {str_debug, &debug_menu, NULL}};

// GLOBAL MENU STATES
menu_t g_menu_root = {
    .header = str_ping_pong,
    .title = str_main_menu,
    .items = main_items,
    .num_items = ARR_LEN(main_items),
    .parent = NULL,
};
menu_state_t g_menu_state;
// ------------------

void setup_menu_structure(void) {
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
  score_items[0].callback = reset_high_scores;
  score_items[1].callback = NULL;

  // TODO: DEBUG ---
  debug_menu.parent = &g_menu_root;
  debug_items[0].callback = NULL;  // Placeholder
  debug_items[1].callback = NULL;  // Placeholder
  debug_items[2].callback = NULL;  // CALIBRATE ---
  // CALIBRATE ---
  calibrate_menu.parent = &g_menu_root;
  calibrate_items[0].callback = NULL;  // Placeholder
  calibrate_items[1].callback = NULL;  // Placeholder
  calibrate_items[2].callback = NULL;  // RETURN ---
  // MENU STATE ---
  g_menu_state.current_menu = &g_menu_root;
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
  }
}

void menu_loop(buttons_t buttons) {
  update_menu_state(buttons);
  // update_display();
}

void update_display() {
  static char display_buffer[32];  // Reduced size and made static
  char temp_str[16];               // Temporary buffer for PROGMEM strings

  // Clear
  oled_clear();

  // Copy header from PROGMEM to RAM
  strcpy_P(temp_str, g_menu_state.current_menu->header);
  LOG_DBG("Current Menu: %s\n", temp_str);
  strncpy(display_buffer, temp_str, sizeof(display_buffer) - 1);
  display_buffer[sizeof(display_buffer) - 1] = '\0';

  // Draw header
  oled_printf(display_buffer, 0, 0);

  for (uint8_t i = 0; i < g_menu_state.current_menu->num_items; i++) {
    // Copy menu item label from PROGMEM
    strcpy_P(temp_str, g_menu_state.current_menu->items[i].label);

    if (i == g_menu_state.current_index) {
      snprintf(display_buffer, sizeof(display_buffer), "> %s", temp_str);
    } else {
      snprintf(display_buffer, sizeof(display_buffer), "  %s", temp_str);
    }
    oled_printf(display_buffer, 0, i + 1);  // +1 to account for header
  }

  switch (g_menu_state.current_render) {
    case MAIN_MENU:
      break;
    case NEW_GAME_MENU:
      break;
    case SCORES_MENU:
      snprintf(display_buffer, sizeof(display_buffer), "Score: %d", g_game_state.score);
      oled_printf(display_buffer, 0, 0);
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
  // Return to parent menu - compare with PROGMEM string
  else if (item->sub_menu == NULL && state->current_menu->parent != NULL &&
           strcmp_P("Return", item->label) == 0) {
    state->current_menu = state->current_menu->parent;
    state->current_index = 0;
  }
  // Execute callback function
  else if (item->callback != NULL) {
    item->callback();
  }
};

menu_render_t str_to_render_t(const char *str) {
  // Since we're comparing PROGMEM strings, we need to use strcmp_P
  if (strcmp_P("New Game", str) == 0) {
    return NEW_GAME_MENU;
  } else if (strcmp_P("High Scores", str) == 0) {
    return SCORES_MENU;
  } else if (strcmp_P("Calibrate Joystick", str) == 0) {
    return CALIBRATE_MENU;
  } else if (strcmp_P("Debug", str) == 0) {
    return DEBUG_MENU;
  } else {
    return MAIN_MENU;  // Default case
  }
}