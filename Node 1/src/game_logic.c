#include "game_logic.h"

#include "oled.h"
#include "utility.h"

const struct button_mapping_rules act2evt[] = {
    {MENU, {.NU = 1}, INCREMENT_UP_EVT},
    {MENU, {.ND = 1}, INCREMENT_DOWN_EVT},
    {MENU, {.NB = 1}, SELECT_EVT},
    {GAME, {.NB = 1}, SELECT_EVT},
};

/*******************************************************************/
/*                        State functions                          */
/*******************************************************************/

void state_menu(state_machine_t* mahcine, event_t input) {}

void state_calibrate(state_machine_t* mahcine, event_t input) {}
void state_difficulty(state_machine_t* mahcine, event_t input) {}
void state_game(state_machine_t* mahcine, event_t input) {}
void state_score(state_machine_t* mahcine, event_t input) {}

// game_state_t g_game_state = {LVL_EASY, 0, 3, false};
// void draw_game();
// void game_reset();
// void set_difficulty_level(difficulty_level_t level) {
//   if (!g_game_state.is_in_game) {
//     // LOG_INF("Difficulty set to %d\n", level);
//     g_game_state.current_level = level;
//     g_game_state.is_in_game = true;
//   }
// }
// void reset_high_scores() { g_game_state.score = 0; }
//
// void game_loop(buttons_t *buttons) {
//   // Simple game logic for demonstration
//   // LOG_INF("Game loop running. Current score: %d\n", g_game_state.score);
//   if (buttons->NB == 1) {      // If the navigation button is pressed
//     g_game_state.score += 10;  // Increase score
//     LOG_DBG("Score increased to %d\n", g_game_state.score);
//   }
//   if (g_game_state.score >= 100) {  // Win condition
//     // LOG_INF("You win! Final score: %d\n", g_game_state.score);
//     game_reset();
//   }
//   draw_game();
// }
//
// void game_reset() {
//   g_game_state.score = 0;
//   g_game_state.lives = 3;
//   g_game_state.is_in_game = false;
//   g_game_state.current_level = LVL_EASY;
// }
//
// void draw_game() {
//   static char display_buffer[32];
//   oled_clear();
//   snprintf(display_buffer, sizeof(display_buffer), "Score: %d", g_game_state.score);
//   oled_printf(display_buffer, 0, 0);
//   snprintf(display_buffer, sizeof(display_buffer), "Lives: %d", g_game_state.lives);
//   oled_printf(display_buffer, 0, 1);
//   switch (g_game_state.current_level) {
//     case LVL_EASY:
//       snprintf(display_buffer, sizeof(display_buffer), "Level: Easy");
//       break;
//     case LVL_MEDIUM:
//       snprintf(display_buffer, sizeof(display_buffer), "Level: Medium");
//       break;
//     case LVL_HARD:
//       snprintf(display_buffer, sizeof(display_buffer), "Level: Hard");
//       break;
//     default:
//       snprintf(display_buffer, sizeof(display_buffer), "Level: Unknown");
//       break;
//   }
//   oled_printf(display_buffer, 0, 2);
//   oled_display();
// }
