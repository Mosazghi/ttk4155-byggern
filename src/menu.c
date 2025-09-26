#include "menu.h"
#include <stdint.h>
#include <stdbool.h>
#include "utility.h"
#include <string.h>


static struct {
    menu_state_t state;

    // Score menu items & structure
    menu_item_t score_items[2];
    menu_t score_menu;

    // Future menu items & structure

    //Main menu items & structure
    menu_item_t main_items[3];
    menu_t main_menu;
}
menu_data = {
    .score_items = {
        {"Reset High Score", NULL, NULL},
        {"Return", NULL, NULL}
    },
    .score_menu = {
        .header = "Current High Score: ",
        .title = "High Score",
        .items = NULL,
        .num_items = 2,
        .parent = NULL
    },

    .main_items = {
        {"New Game", NULL, NULL},
        {"High Score", NULL, NULL},
        {"Calibrate Joystick", NULL, NULL}
    },
    .main_menu = {
        .header = "Name of the Game",
        .title = "Main Menu",
        .items = NULL,
        .num_items = 3,
        .parent = NULL
    }
};


// menu_t score_menu;
// menu_item_t main_items[3];
// menu_t main_menu;
// score_menu = {
//     .header = "Current High Score: ",
//     .title = "High Score",
//     .items = score_menu_items,
//     .num_items = 2,
//     .parent = main_menu
// };

// main_items[] = {
//     {"New Game", NULL, NULL},
//     {"High Score", score_menu, NULL},
//     {"Calibrate Joystick", NULL, NULL}
// };

// menu_t main_menu = {
//     .header = "Name of the Game",
//     .title = "Main Menu",
//     .items = main_items,
//     .num_items = 3,
//     .parent = NULL
// };

// menu_item_t score_menu_items[] = {
//     {"Reset High Score", NULL, NULL},
//     {"Return", NULL, NULL}
// };



static void setup_menu_structure(void) {
    // Link score menu
    menu_data.score_menu.items = menu_data.score_items;
    menu_data.score_menu.parent = &menu_data.main_menu;
    
    // Link main menu
    menu_data.main_menu.items = menu_data.main_items;
    menu_data.main_items[1].sub_menu = &menu_data.score_menu;
    
    // Initialize state
    menu_data.state.current_menu = &menu_data.main_menu;
    menu_data.state.current_index = 0;
}

void menu_init(menu_state_t *state, menu_t *root) {
    (void)root;
    setup_menu_structure();
    state->current_menu = &menu_data.main_menu;
    state->current_index = 0;
}

void menu_move_up(menu_state_t *state){
    if (state->current_index > 0) {
        state->current_index --;
    }
    else state->current_index = state->current_menu->num_items - 1;
}

void menu_move_down(menu_state_t *state) {
    if (state->current_index < state->current_menu->num_items -1) {
        state->current_index ++;
    }
    else state->current_index = 0;
}

// void menu_return(menu_state_t *state) {
//     menu_item_t *item =&state->current_menu->items[state->]
// }

bool menu_select(menu_state_t *state){
    // FIXME: If items size < current_index, add a check 
    menu_item_t *item = &state->current_menu->items[state->current_index];
    if (item->sub_menu != NULL) {
        state->current_menu = item->sub_menu;
        state->current_index = 0;
        return true;
    }
    else if (item->sub_menu == NULL && state->current_menu->parent != NULL && strcmp(item->label, "Return") == 0) {
        state->current_menu = state->current_menu->parent;
        state->current_index = 0;
        return true;
    }
    else return false;
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
        menu_move_up(&menu_data.state);
    }
    else if (buttons.ND == 1) {
        menu_move_down(&menu_data.state);
    }
    if (buttons.NB == 1) {
        menu_select(&menu_data.state);
        LOG_INF("Selected: %s\n", 
            menu_data.state.current_menu->items[menu_data.state.current_index].label);
    }
    LOG_INF("Current Menu: %s\n", menu_data.state.current_menu->title);
    LOG_INF("Current Selection: %s\n", 
            menu_data.state.current_menu->items[menu_data.state.current_index].label);
    
}