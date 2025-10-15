#include "game_logic.h"

#include "main.h"
#include "oled.h"
#include "utility.h"

static void game_reset(game_state_t *state);
static bool is_game_over(game_state_t *state);

game_state_t g_game_state = {LVL_EASY, 0, 3, false};

void try_start_game(difficulty_level_t level, game_state_t *state) {
  if (!state->is_in_game) {
    state->score = 0;
    state->lives = 3;
    state->is_in_game = true;
    state->current_level = level;
    // TODO: Notify NODE 2 that the game has started (with the selected diff. level)?!
  }
}

// WARN: Try to avoid accessing the global variable directly
void reset_high_scores() { g_game_state.score = 0; }

void game_loop(game_state_t *state, joystick_xy_t *joystick) {
  bool prev_btn_pressed = false;
  joystick_dir_t joystick_dir = {0};

  while (1) {
    joystick_dir = joystick_get_dir(joystick, 15);
    if (joystick->btn && !prev_btn_pressed) {
      // TODO: Send CAN message when joystick buttonc is pressed
    }

    switch (joystick_dir) {
      case UP:
        // TODO: Send CAN message for UP movement
        break;
      case DOWN:
        // TODO: Send CAN message for DOWN movement
        break;
      case LEFT:
        // TODO: Send CAN message for LEFT movement
        break;
      case RIGHT:
        // TODO: Send CAN message for RIGHT movement
        break;
      default:
        break;
    }

    prev_btn_pressed = joystick->btn;

    bool has_lost = is_game_over(state);
    if (has_lost) {
      game_reset(state);
      break;
    }
  }
}

void game_reset(game_state_t *state) {
  state->score = 0;
  state->lives = 3;
  state->is_in_game = false;
  state->current_level = LVL_EASY;
}

void render_game(game_state_t *state) {
  static char disp_buf[32];
  oled_clear();
  snprintf(disp_buf, sizeof(disp_buf), "Score: %d", state->score);
  oled_printf(disp_buf, 0, 0);
  snprintf(disp_buf, sizeof(disp_buf), "Lives: %d", state->lives);
  oled_printf(disp_buf, 0, 1);
  switch (state->current_level) {
    case LVL_EASY:
      snprintf(disp_buf, sizeof(disp_buf), "Level: Easy");
      break;
    case LVL_MEDIUM:
      snprintf(disp_buf, sizeof(disp_buf), "Level: Medium");
      break;
    case LVL_HARD:
      snprintf(disp_buf, sizeof(disp_buf), "Level: Hard");
      break;
    default:
      snprintf(disp_buf, sizeof(disp_buf), "Level: Unknown");
      break;
  }
  oled_printf(disp_buf, 0, 2);
  oled_display();
}

static bool is_game_over(game_state_t *state) { return state->lives <= 0; }
