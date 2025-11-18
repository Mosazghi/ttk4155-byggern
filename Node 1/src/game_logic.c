#include "game_logic.h"

#include "can.h"
#include "main.h"
#include "oled.h"
#include "utility.h"

/* -- en enum --*/
typedef enum { IR_BLOCKED, SCORE_UPDATE } can_message_type_t;

static void game_reset(game_state_t *state);
static bool is_game_over(game_state_t *state);
static void render_game_over();

game_state_t g_game_state = {LVL_EASY, 0, 3, false};

void try_start_game(difficulty_level_t level, game_state_t *state) {
  if (!state->is_in_game) {
    state->score = 0;
    state->lives = 10;
    state->is_in_game = true;
    state->current_level = level;
    // TODO: Notify NODE 2 that the game has started (with the selected diff. level)?
  }
}

void reset_high_scores() { g_game_state.score = 0; }

void game_loop(game_state_t *state, joystick_xy_t *joystick) {
  can_message_t can_buf = {0};
  if (state->new_can_msg && state->is_in_game) {
    can_buf = can_receive();
    if (can_buf.id == IR_BLOCKED) {
      state->lives--;
    }
    if (can_buf.id == SCORE_UPDATE) {
      state->score++;
    }
    state->new_can_msg = false;
  }

  bool has_lost = is_game_over(state);

  if (has_lost) {
    render_game_over();
    game_reset(state);
  }
}

void game_reset(game_state_t *state) {
  state->lives = 10;
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
      snprintf(disp_buf, sizeof(disp_buf), "Level: Casual");
      break;
    case LVL_MEDIUM:
      snprintf(disp_buf, sizeof(disp_buf), "Level: Gamer");
      break;
    case LVL_HARD:
      snprintf(disp_buf, sizeof(disp_buf), "Level: GOAT");
      break;
    default:
      snprintf(disp_buf, sizeof(disp_buf), "Level: Unknown");
      break;
  }
  oled_printf(disp_buf, 0, 2);
  oled_display();
}

static void render_game_over() {
  static char disp_buf[32];
  oled_clear();
  oled_set_font(FONT_L);
  snprintf(disp_buf, sizeof(disp_buf), "Your Score: %d", g_game_state.score);
  oled_printf("You Tried!", 15, 2);
  oled_printf(disp_buf, 15, 4);
  oled_set_font(FONT_S);
  oled_printf("Git gud...", 15, 7);
  oled_display();
  _delay_ms(3800);
  oled_set_font(FONT_M);
}

static bool is_game_over(game_state_t *state) { return state->lives <= 0; }
