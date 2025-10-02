#include "game_logic.h"
#include "utility.h"

game_state_t g_game_state;
void set_difficulty_level(difficulty_level_t level) { 
  g_game_state.current_level = level;

}
void reset_high_scores() { g_game_state.score = 0; }