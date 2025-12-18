  //=================================================================
// The main program file.
//
// Copyright 2025 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


// PROJECT INCLUDES
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "explorer.h"
#include "ThisThread.h"
#include <math.h>
#include <stdio.h>

// Important definitions


/////////////////////////////////////////////////////////////////////////////////////////

// GLOBAL VARS
Explorer explorer;

/////////////////////////////////////////////////////////////////////////////////////////

// FUNCTION PROTOTYPES
void playSound(char* wav);
int get_action(GameInputs inputs);
int update_game(int action);
void draw_upper_status();
void draw_lower_status();
void draw_border();
void draw_game(int draw_option);
void init_main_map();
void draw_start_screen();
void draw_character_select();
void draw_instructions();
/////////////////////////////////////////////////////////////////////////////////////////

/**
 * Program entry point! This is where it all begins.
 * This function is for all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    uLCD.baudrate(115200);
    wait_us(20000);

    GameInputs inputs;

    Timer t;
    t.start();

    // Draw the start screen
    draw_start_screen();

    // Game state for menu navigation
    enum MenuState { START_SCREEN, CHARACTER_SELECT, INSTRUCTIONS, GAME_RUNNING };
    MenuState current_state = START_SCREEN;

    // This loop handles the start screen and menu navigation
    while(current_state != GAME_RUNNING) {
        inputs = read_inputs();
        
        if (current_state == START_SCREEN) {
            if (inputs.left) {
                // Navigate to character select
                current_state = CHARACTER_SELECT;
                uLCD.cls();
                draw_character_select();
                wait_us(300000); // Debounce delay
            }
            else if (inputs.right) {
                // Navigate to instructions
                current_state = INSTRUCTIONS;
                uLCD.cls();
                draw_instructions();
                wait_us(300000); // Debounce delay
            }
            else if (inputs.center || inputs.b1) {
                // Start game immediately
                srand(t.elapsed_time().count());
                current_state = GAME_RUNNING;
            }
        }
        else if (current_state == CHARACTER_SELECT) {
            // update color of character selected with explorer attribute updated
            if (inputs.b1) {
                // Confirm selection and start game
                srand(t.elapsed_time().count());
                current_state = GAME_RUNNING;
                // default red
            }
            else if (inputs.b2) {
                explorer.character_selected = 1;
                srand(t.elapsed_time().count());
                current_state = GAME_RUNNING;
            }
            else if (inputs.b3) {
                explorer.character_selected = 2;
                srand(t.elapsed_time().count());
                current_state = GAME_RUNNING;
            }
            else if (inputs.b4) {
                explorer.character_selected = 3;
                srand(t.elapsed_time().count());
                current_state = GAME_RUNNING;
            }
            else if (inputs.center) {
                // Go back to start screen
                current_state = START_SCREEN;
                uLCD.cls();
                draw_start_screen();
                wait_us(300000);
            }
        }
        else if (current_state == INSTRUCTIONS) {
            if (inputs.center || inputs.b1) {
                // Start game from instructions
                srand(t.elapsed_time().count());
                current_state = GAME_RUNNING;
            }
            else if (inputs.b3) {
                // Go back to start screen
                current_state = START_SCREEN;
                uLCD.cls();
                draw_start_screen();
                wait_us(300000);
            }
        }
        
        wait_us(50000); // Small delay for input polling
    }


    uLCD.cls();

    explorer_init(&explorer);

    // Initialize game state
    explorer.loc_x = explorer.loc_y = 5;
    
    // Initialize the maps
    map_init();
    init_main_map();

    
    // Initial drawing
    draw_game(FULL_DRAW);

    
    // Main game loop
while(1) {

    // Check if we need to return to menu
    if (explorer.return_to_menu) {
        explorer.return_to_menu = false;

        // reset teleporter
        if (explorer.teleporter_spawned) {
            map_erase(explorer.teleporter_x,explorer.teleporter_y);
            explorer.teleporter_spawned = false;
            explorer.teleporter_x = -1;
            explorer.teleporter_y = -1;
        }
        
        // Reset explorer state (but preserve leaderboard)
        int saved_leaderboard = explorer.leaderboard;
        explorer_init(&explorer);
        explorer.leaderboard = saved_leaderboard;  // Keep high score
        explorer.loc_x = explorer.loc_y = 5;
        
        // Return to menu navigation
        enum MenuState current_state = START_SCREEN;
        draw_start_screen();
        
        // Re-enter the menu loop
        while(current_state != GAME_RUNNING) {
            inputs = read_inputs();
            
            if (current_state == START_SCREEN) {
                if (inputs.left) {
                    current_state = CHARACTER_SELECT;
                    uLCD.cls();
                    draw_character_select();
                    wait_us(300000);
                }
                else if (inputs.right) {
                    current_state = INSTRUCTIONS;
                    uLCD.cls();
                    draw_instructions();
                    wait_us(300000);
                }
                else if (inputs.center || inputs.b1) {
                    srand(t.elapsed_time().count());
                    current_state = GAME_RUNNING;
                }
            }
            else if (current_state == CHARACTER_SELECT) {
                if (inputs.b1) {
                    explorer.character_selected = 0;  // Red (default)
                    srand(t.elapsed_time().count());
                    current_state = GAME_RUNNING;
                }
                else if (inputs.b2) {
                    explorer.character_selected = 1;  // Blue
                    srand(t.elapsed_time().count());
                    current_state = GAME_RUNNING;
                }
                else if (inputs.b3) {
                    explorer.character_selected = 2;  // Green
                    srand(t.elapsed_time().count());
                    current_state = GAME_RUNNING;
                }
                else if (inputs.b4) {
                    explorer.character_selected = 3;  // Yellow
                    srand(t.elapsed_time().count());
                    current_state = GAME_RUNNING;
                }
                else if (inputs.center) {
                    current_state = START_SCREEN;
                    uLCD.cls();
                    draw_start_screen();
                    wait_us(300000);
                }
            }
            else if (current_state == INSTRUCTIONS) {
                if (inputs.center || inputs.b1) {
                    srand(t.elapsed_time().count());
                    current_state = GAME_RUNNING;
                }
                else if (inputs.b3) {
                    current_state = START_SCREEN;
                    uLCD.cls();
                    draw_start_screen();
                    wait_us(300000);
                }
            }
            
            wait_us(50000);
        }
        
        // Clear screen and redraw game
        uLCD.cls();
        draw_game(FULL_DRAW);
        
        // Continue to next iteration of main loop
        continue;
    }
    
        // Timer to measure game update speed
        t.reset();
        t.start();
        
        // Read inputs 
        inputs = read_inputs();
        
        // Determine action (move, act, menu, etc.)
        int action = get_action(inputs);
        
        // Update game
        int result = update_game(action);

        // done
        uLCD.filled_rectangle(0, 0, 160, 7, BLACK);
        char str[1024];
        snprintf(str,sizeof(str),"X:%d Y:%d S:%d L:%d", explorer.loc_x, explorer.loc_y, explorer.score, explorer.lives);
        uLCD.text_string(str,0,0,FONT_7X8,RED);

        
        // Check for game over based on result
        // handle game over or win accordingly
        if (result == WIN) {
            // update leaderboard
            if (explorer.score > explorer.leaderboard) {
                explorer.leaderboard = explorer.score;
            }

            if (explorer.teleporter_spawned) {
                map_erase(explorer.teleporter_x, explorer.teleporter_y); // erase item then reset teleporter location
                explorer.teleporter_spawned = false;
                explorer.teleporter_x = -1;
                explorer.teleporter_y = -1;
            }
    

            uLCD.cls(); // clear screen
            uLCD.locate(3, 6);  // Center
            uLCD.color(GREEN);
            uLCD.printf("GAME OVER \n YOU WON!");

            uLCD.locate(2, 8);
            uLCD.printf("Score: %d", explorer.score);

            uLCD.locate(1, 11);
            uLCD.color(WHITE);
            uLCD.printf("Press BUTTON2");
            uLCD.locate(2, 12);
            uLCD.printf("for Menu");

            // reset return_to_menu flag and handle main loop
            while(1) {
                GameInputs win_input = read_inputs();
                if (win_input.b2) {
                    explorer.return_to_menu = true;  // Set flag
                    // reset score for teleporter to go away
                    explorer.score = 0;
                    uLCD.cls();
                    wait_us(300000);
                    break;  // Exit the while loop
                }
                wait_us(100000);
            }
        } 
        else if (result == GAME_OVER) {  
            uLCD.cls();
            uLCD.locate(4, 6);
            uLCD.color(RED);
            uLCD.printf("GAME OVER");
            
            uLCD.locate(4, 8);
            uLCD.printf("Score: %d", explorer.score);
            
            uLCD.locate(2, 11);
            uLCD.color(WHITE);
            uLCD.printf("Button2: Menu");

            while(1) {
                GameInputs death_input = read_inputs();
                if (death_input.b2) {
                    explorer.return_to_menu = true;
                    // reset score for teleporter to go away
                    explorer.score = 0;
                    uLCD.cls();
                    wait_us(300000);
                    break;
                }
                wait_us(100000);
            }
        }
        // Draw the game
        draw_game(MOVED);
        
        // Compute update time
        t.stop();
        int dt = t.read_ms();

        // Display and wait
        // Text is 8 pixels tall
        if (dt < 100) {
            wait_us((100 - dt) * 1000); // wait to enforce a 10 Hz update rate
        }
    }
}

// draw correct selected character
void draw_selected_explorer(int u, int v)
{
    switch(explorer.character_selected) {
        case 1:
            draw_blue_explorer(u, v);
            break;
        case 2:
            draw_green_explorer(u, v);
            break;
        case 3:
            draw_yellow_explorer(u, v);
            break;
        default:
            draw_red_explorer(u, v);  // Default red (case 0)
            break;
    }
}

// Function to draw the start screen
void draw_start_screen()
{
    uLCD.cls();
    uLCD.color(WHITE);
    
    uLCD.locate(2, 1);
    uLCD.printf("CAVE EXPLORER");

    uLCD.color(RED);
    uLCD.locate(2, 3);
    uLCD.printf("HIGH SCORE: %d", explorer.leaderboard);
    
    uLCD.locate(1, 7);
    uLCD.color(BLUE);
    uLCD.printf("< LEFT: Character");
    
    uLCD.locate(1, 9);
    uLCD.color(BLUE);
    uLCD.printf("> RIGHT: Tutorial");

    
    uLCD.locate(1, 13);
    uLCD.color(GREEN);
    uLCD.printf("+ CENTER: START");
}

// Function to draw character select screen
void draw_character_select()
{
    uLCD.cls();
    uLCD.color(WHITE);
    
    uLCD.locate(3, 1);
    uLCD.printf("SELECT YOUR");

    uLCD.locate(4, 2);
    uLCD.printf("CHARACTER");
    
    draw_red_explorer(8, 60);
    draw_blue_explorer(40, 60);
    draw_green_explorer(77, 60);
    draw_yellow_explorer(110, 60);

    uLCD.color(BLUE);
    uLCD.locate(1, 10);
    uLCD.printf("+ CENTER: Menu");
    
    uLCD.color(GREEN);
    uLCD.locate(1, 12);
    uLCD.printf("BUTTON1: Player1");
    uLCD.color(GREEN);
    uLCD.locate(1, 13);
    uLCD.printf("BUTTON2: Player2");
    uLCD.color(GREEN);
    uLCD.locate(1, 14);
    uLCD.printf("BUTTON3: Player3");
    uLCD.color(GREEN);
    uLCD.locate(1, 15);
    uLCD.printf("BUTTON4: Player4");
}

// Function to draw instructions screen
void draw_instructions()
{
    uLCD.cls();

    uLCD.color(BLUE);
    uLCD.locate(0, 1);
    uLCD.printf("CONTROLS:");
    
    uLCD.color(WHITE);
    uLCD.locate(0, 2);
    uLCD.printf("Joystick: Movement");
    uLCD.locate(0, 3);
    uLCD.printf("BUTTON1: Weapon");
    uLCD.locate(0, 4);
    uLCD.printf("BUTTON2: Shockwave");
    uLCD.locate(0, 5);
    uLCD.printf("BUTTON3: Pause");

    draw_heal(0, 56);
    uLCD.locate(2, 7);
    uLCD.printf("+1 Life");
    
    draw_poison(0, 64);
    uLCD.locate(2, 8);
    uLCD.printf("-1 Life");
    
    draw_nightvision(0, 72);
    uLCD.locate(2, 9);
    uLCD.printf("Night Vision");
    
    draw_teleporter(0, 80);
    uLCD.locate(2, 10);
    uLCD.printf("Teleport to boss");
    uLCD.locate(2, 11);
    uLCD.printf("spawns if S>100");
    uLCD.locate(0, 12);
    uLCD.printf("Shockwave to win");

    uLCD.color(BLUE);
    uLCD.locate(1, 14);
    uLCD.printf("BUTTON3: Back");
    
    uLCD.color(GREEN);
    uLCD.locate(0, 15);
    uLCD.printf("+ CENTER: START");
}





/**
 * Plays a .wav file
 */
void playSound(char* wav)
{
    // OPTIONAL: Implement
    
}

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possible return values are defined below.
 * Get Actions from User (pushbuttons, and nav_switch)
 * Based on push button and navigation switch inputs, determine which action
 * needs to be performed (may be no action).
 */
int get_action(GameInputs inputs) // get_action code block
{
    // 1. Check your action and menu button inputs and return the corresponding action value
    if(inputs.b1) return ACTION_BUTTON;
    if(inputs.b2) return SHOCKWAVE_BUTTON;
    if(inputs.b3) return MENU_BUTTON;
    // 2. Check for your navigation switch inputs and return the corresponding action value
    if(inputs.up) return GO_UP;
    if(inputs.down) return GO_DOWN;
    if(inputs.left) return GO_LEFT;
    if(inputs.right) return GO_RIGHT;
    if(inputs.center) return CENTER_PRESSED;
    // If no button is pressed, just return no action value
    return NO_ACTION;
}

/**
 * This function is called by update game to check when the explorer collects an object. 
 * We update the explorer and game conditions as needed based on the object type.
 */
int get_object(){
    // Get item at current location
    MapItem* current_item = get_current(explorer.loc_x, explorer.loc_y);

    if (!current_item) return NO_RESULT;
    // Initialize a return value variable to denote the item type we got
    int item_type = current_item->type;
    // Check if item is valid and is a nightvision
    // if so, increment the score, remove night vision, enable full map for 10s, respawn at random empty location
    if (item_type == NIGHTVISION) {
        explorer.score += 10;
        map_erase(explorer.loc_x, explorer.loc_y);
        // enable night vision
        explorer.has_night_vision = true;
        explorer.night_vision_start_time = us_ticker_read();
        // add new night vision
        int new_x, new_y;
        do {
            new_x = rand() % map_width();
            new_y = rand() % map_height();
        } while (get_here(new_x, new_y) != NULL); // find empty spot
        add_nightvision(new_x, new_y);

        return GOT_OBJ;
    } 
     // Else, check if the item is a valid heal or poison item or a teleporter
    //  If consumable, remove the heal/poison item and set the return value variable to GOT_OBJ
    //  If teleporter, update the explorer location accordingly and return WIN for baseline win condition
    else if (item_type == HEAL) {
        explorer.lives += 1;
        map_erase(explorer.loc_x, explorer.loc_y);
        return GOT_OBJ;
    } else if (item_type == POISON) {
        explorer.lives -= 1;
        map_erase(explorer.loc_x, explorer.loc_y);
        return GOT_OBJ;
    } else if (item_type == TELEPORTER) {
        // teleport to boss chamber
        explorer.loc_x = 35;  // Inside boss chamber
        explorer.loc_y = 5;

        return GOT_OBJ;
    } else if (item_type == XRAY) {
        explorer.score += 5;
        map_erase(explorer.loc_x, explorer.loc_y);
        explorer.has_xray_vision = true;
        explorer.xray_vision_start_time = us_ticker_read();
        // add ability to walk through walls in update loop, while has_xray_vision walls = walkable EXCEPT boss fight! so make those diff walls
        // // add new xray visions
        int new_x, new_y;
        do { 
            new_x = rand() % map_width();
            new_y = rand() % map_height();
        } while (get_here(new_x, new_y) != NULL || 
                 (new_x >= 31 && new_x <= 39 && new_y >= 1 && new_y <= 9)); // find empty spot NOT in boss chamber
        add_xrayvision(new_x, new_y);

        return GOT_OBJ;
    } else if (item_type == BLAST) {
        explorer.score += 5;
        explorer.blast_waves_owned += 1;
        map_erase(explorer.loc_x, explorer.loc_y);
         // add new blastss
        int new_x, new_y;
        do {
            new_x = rand() % map_width();
            new_y = rand() % map_height();
        } while (get_here(new_x, new_y) != NULL); // find empty spot
        add_blast(new_x, new_y);
        return GOT_OBJ;
    }

    return NO_RESULT; // if no special item collected
}

// for boss
void move_boss() {
    unsigned long long current_time = us_ticker_read();
    if (current_time - explorer.last_boss_move_time < 1000000) { // move every 1s
        return;
    }
    
    explorer.last_boss_move_time = current_time;
    
    int old_x = explorer.boss_x;
    int old_y = explorer.boss_y;
    
    int direction = rand() % 4;
    int new_x = old_x;
    int new_y = old_y;
    
    switch(direction) {
        case 0: new_y -= 1; break;
        case 1: new_y += 1; break;
        case 2: new_x -= 1; break;
        case 3: new_x += 1; break;
    }
    
    // Boss chamber boundaries: x: 31-39, y: 1-9 (INSIDE the walls)
    // The walls are AT y=0 and y=10, so boss can only be in 1-9
    if (new_x >= 31 && new_x <= 39 && new_y >= 1 && new_y <= 9) {
        MapItem* target = get_here(new_x, new_y);
        if (!target || target->walkable) {
            map_erase(old_x, old_y);
            add_boss(new_x, new_y);
            explorer.boss_x = new_x;
            explorer.boss_y = new_y;
        }
    }
    // If move is invalid, boss stays at old position (no change needed)
}

// kill boss
int check_shockwave_hit() {
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;
            int check_x = explorer.loc_x + dx;
            int check_y = explorer.loc_y + dy;
            if (check_x == explorer.boss_x && check_y == explorer.boss_y) {
                explorer.boss_lives -= 1;
            }
            // delete items in radius
            // make sure item is a map item NOT walls
            MapItem* item = get_current(check_x, check_y);
            if (item != NULL && item->type != WALL) {
                map_erase(check_x, check_y);
            }
            if (explorer.boss_lives == 0) return WIN;
        }
    }
    return NO_RESULT;
}


/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the explorer position accordingly.
 * 
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the explorer has not moved.
 */
int update_game(int action) // updates the game with all changes
{
    // Check if nightvision should expire after 10s
    if (explorer.has_night_vision) {
        unsigned long long current_time = us_ticker_read();
        unsigned long long elapsed = current_time - explorer.night_vision_start_time;
        
        if (elapsed > 10000000) {  // 10 seconds
            explorer.has_night_vision = false;
            printf("Night vision expired!\r\n");
        }
    }

    // expire xray vision after 10s
    if (explorer.has_xray_vision) {
        unsigned long long current_time = us_ticker_read();
        unsigned long long elapsed = current_time - explorer.xray_vision_start_time;
        
        if (elapsed > 10000000) {  // 10 seconds
            explorer.has_xray_vision = false;
            printf("xray vision expired!\r\n");
        }
    }

    if (explorer.shockwave_active) {
        unsigned long long current_time = us_ticker_read();
        unsigned long long elapsed = current_time - explorer.shockwave_start_time;
        if (elapsed > 1000000) { // how long drawing of blast is visible = .1s
            explorer.shockwave_active = false;
        }
    }

    move_boss();

    if (explorer.loc_x == explorer.boss_x && explorer.loc_y == explorer.boss_y) {
        explorer.lives -= 1;
    }

    // check if lives = 0; end game
    if (explorer.lives == 0) {
        // Display game over screen
        uLCD.cls();  // Clear screen
        uLCD.filled_rectangle(0, 0, 127, 127, BLACK);
        
        uLCD.locate(4, 6);
        uLCD.color(RED);
        uLCD.printf("GAME OVER");
        
        uLCD.locate(4, 8);
        uLCD.printf("Score: %d", explorer.score);
        
        uLCD.locate(2, 11);
        uLCD.color(WHITE);
        uLCD.printf("Press Button3");
        uLCD.locate(2, 12);
        uLCD.printf("for Menu");

        // update leaderboard
        if (explorer.score > explorer.leaderboard) {
            explorer.leaderboard = explorer.score;
        }

        return GAME_OVER;   
    }

    // pause menu
    if (explorer.paused) {
    uLCD.locate(3, 6);
    uLCD.color(BLUE);
    uLCD.printf("PAUSED GAME");
    
    uLCD.locate(2, 8);
    uLCD.printf("S: %d L: %d", explorer.score, explorer.lives);
    
    // Wait for MENU_BUTTON to be pressed again to unpause
    while(explorer.paused) {
        GameInputs pause_inputs = read_inputs();
        if(pause_inputs.b3) {  // b3 is MENU_BUTTON
            explorer.paused = false;
            draw_game(FULL_DRAW);  // Redraw game when unpausing
            break;
        }
        wait_us(100000);  // Small delay to avoid busy waiting
    }
    return NO_RESULT;
    }


    // Update the previous explorer position to the current position
    explorer.loc_x_prev = explorer.loc_x;
    explorer.loc_y_prev = explorer.loc_y;

    // places teleporter only if score >= 100
    if (explorer.score >= 100 && !explorer.teleporter_spawned) {
    int tx, ty;
    do {
        tx = rand() % 10 + 3;  // Near spawn area (x: 3-12)
        ty = rand() % 10 + 3;  // Near spawn area (y: 3-12)
    } while (get_here(tx, ty) != NULL);  // Find empty spot
        add_teleporter(tx, ty, 35, 5);
        explorer.teleporter_x = tx;
        explorer.teleporter_y = ty;
        explorer.teleporter_spawned = true;
    }

    
    // Check if action is to go up, left, right, and down
    //      Get item north of the current explorer position, update current position upwards, check if item exists and is walkable
    //      else update position to og position and return no result
    //      Check if the explorer gets an object by calling another function, return moved 
    if (action == GO_UP) {
        MapItem* north_item = get_north(explorer.loc_x, explorer.loc_y);
        int dest_y = explorer.loc_y - 1;
        
        // Check if destination is in restricted area
        bool dest_restricted = ((explorer.loc_x >= 30 && explorer.loc_x <= 39 && dest_y >= 0 && dest_y <= 10)) ||
                            (explorer.loc_x <= 0 || explorer.loc_x >= map_width()-1 || dest_y <= 0 || dest_y >= map_height()-1);
        
        if (north_item && !north_item->walkable) {
            if (!explorer.has_xray_vision || dest_restricted) {
                return NO_RESULT;
            }
        }
        
        explorer.loc_y = dest_y;
        // change tile to gray, and only score + =1 if tile is gray

        explorer.score += 1;
        get_object();
        return MOVED;
    }

    if (action == GO_DOWN) {
        MapItem* south_item = get_south(explorer.loc_x, explorer.loc_y);
        int dest_y = explorer.loc_y + 1;
        
        bool dest_restricted = ((explorer.loc_x >= 30 && explorer.loc_x <= 39 && dest_y >= 0 && dest_y <= 10)) ||
                            (explorer.loc_x <= 0 || explorer.loc_x >= map_width()-1 || dest_y <= 0 || dest_y >= map_height()-1);
        
        if (south_item && !south_item->walkable) {
            if (!explorer.has_xray_vision || dest_restricted) {
                return NO_RESULT;
            }
        }
        
        explorer.loc_y = dest_y;
        explorer.score += 1;
        get_object();
        return MOVED;
    }

    if (action == GO_LEFT) {
        MapItem* west_item = get_west(explorer.loc_x, explorer.loc_y);
        int dest_x = explorer.loc_x - 1;
        
        bool dest_restricted = ((dest_x >= 30 && dest_x <= 39 && explorer.loc_y >= 0 && explorer.loc_y <= 10)) ||
                            (dest_x <= 0 || dest_x >= map_width()-1 || explorer.loc_y <= 0 || explorer.loc_y >= map_height()-1);
        
        if (west_item && !west_item->walkable) {
            if (!explorer.has_xray_vision || dest_restricted) {
                return NO_RESULT;
            }
        }
        
        explorer.loc_x = dest_x;
        explorer.score += 1;
        get_object();
        return MOVED;
    }

    if (action == GO_RIGHT) {
        MapItem* east_item = get_east(explorer.loc_x, explorer.loc_y);
        int dest_x = explorer.loc_x + 1;
        
        bool dest_restricted = ((dest_x >= 30 && dest_x <= 39 && explorer.loc_y >= 0 && explorer.loc_y <= 10)) ||
                            (dest_x <= 0 || dest_x >= map_width()-1 || explorer.loc_y <= 0 || explorer.loc_y >= map_height()-1);
        
        if (east_item && !east_item->walkable) {
            if (!explorer.has_xray_vision || dest_restricted) {
                return NO_RESULT;
            }
        }
        
        explorer.loc_x = dest_x;
        explorer.score += 1;
        get_object();
        return MOVED;
    }// Movement section - check if DESTINATION is in restricted area


    // check for other actions (button presses) and handle accordingly    
    if (action == ACTION_BUTTON) {
        // switch weapons **DOLATER advanced
    }

    if (action == SHOCKWAVE_BUTTON) {
        // don't blast if no waves owned
        if (explorer.blast_waves_owned == 0) {
            return  NO_ACTION;
        }

        explorer.shockwave_active = true;
        explorer.shockwave_start_time = us_ticker_read();
        explorer.blast_waves_owned -= 1;
        
        // Force draw the shockwave visual immediately
        draw_game(MOVED);
        
        wait_us(500000);  // 0.5 second - how long blast will kill the boss
        
        return check_shockwave_hit();
    }

    if (action == MENU_BUTTON) {
        // for pausing game/state management
        explorer.paused = !(explorer.paused); // invert boolean
    } 


    // If no action was given, then we simply return no result
    return NO_RESULT;
}




/**
 * Draw the upper status bar.
 */
void draw_upper_status()
{
    uLCD.line(0, 9, 127, 9, GREEN);
}

/**
 * Draw the lower status bar.
 */
void draw_lower_status()
{
    uLCD.line(0, 118, 127, 118, GREEN);
}

/**
 * Draw the border for the map.
 */
void draw_border()
{
    uLCD.filled_rectangle(  0,   9, 127,  14, WHITE);   // Top
    uLCD.filled_rectangle(  0,  13,   2, 114, WHITE);   // Left
    uLCD.filled_rectangle(  0, 114, 127, 117, WHITE);   // Bottom
    uLCD.filled_rectangle(124,  14, 127, 117, WHITE);   // Right
}

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status
 * bars. Unless init is nonzero, this function will optimize drawing by only
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int draw_option)
{
    // Draw game border first
    if(draw_option == FULL_DRAW) 
    {
        draw_border();
        int u = 58;
        int v = 56;
        draw_selected_explorer(u, v);
        return;
    }

    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) { // Iterate over columns of tiles
        for (int j = -4; j <= 4; j++) { // Iterate over one column of tiles
            // Here, we have a given (i,j)

            // TODO: Check if this tile is lit by night vision tool
            // Without night vision tool active, only draw tiles in a radius
            // of 2 from the explorer. For tiles that are at radius 3, use the
            // provided draw_dark_outline function to draw the outline of the
            // visible radius. Anything outside of this should not be drawn.
            // With the night vision tool active, draw all tiles fully lit.

            // Compute the current map (x,y) of this tile
            int x = i + explorer.loc_x;
            int y = j + explorer.loc_y;

            // Compute the previous map (px, py) of this tile
            int px = i + explorer.loc_x_prev;
            int py = j + explorer.loc_y_prev;

            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;

            
            DrawFunc draw = NULL;

            // Calculate distance from center (explorer position)
            int distance = abs(i) > abs(j) ? abs(i) : abs(j);  // Chebyshev distance
            
            // Check if this tile should be visible
            bool visible = false;
            
            if (explorer.has_night_vision) {
                // Night vision active: draw all tiles fully lit
                visible = true;
            } else {
                // Night vision NOT active: only draw tiles within radius 2
                if (distance <= 2) {
                    visible = true;
                } else if (distance == 3) {
                    // Draw dark outline at radius 3
                    draw = draw_dark_outline;
                } else {
                    draw_nothing(u, v);
                }
                // Anything beyond radius 3 is not drawn
            }
            
            // Figure out what to draw for visible tiles
            if (visible && x >= 0 && y >= 0 && x < map_width() && y < map_height()) {
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (draw_option || curr_item != prev_item) {
                    if (curr_item) {
                        draw = curr_item->draw;
                    } else {
                        draw = draw_nothing;
                    }
                } else if (curr_item && curr_item->type == CLEAR) {
                    draw = curr_item->draw;
                }
            } else if (visible && draw_option) {
                // Out of bounds but doing full draw
                draw = draw_wall;
            }
            
            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }

        if (explorer.shockwave_active) {
        int center_u = 58;
        int center_v = 56;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;
                int u = center_u + (dx * 11);
                int v = center_v + (dy * 11);
                uLCD.filled_rectangle(u, v, u+10, v+10, CYAN);
            }
        }
    }

    // keep player in center of screen when map moves around
    int explorer_u = 58;  // Center X pixel
    int explorer_v = 56;  // Center Y pixel
    draw_selected_explorer(explorer_u, explorer_v);

    // Draw status bars
    draw_upper_status();
    draw_lower_status();
}

/**
 * Initialize the main world map with walls and other items.
 */
void init_main_map()
{ 
    printf("Adding walls!\r\n");
    add_wall(            0,              0, HORIZONTAL, map_width());
    add_wall(            0, map_height()-1, HORIZONTAL, map_width());
    add_wall(            0,              0,   VERTICAL, map_height());
    add_wall(map_width()-1,              0,   VERTICAL, map_height());
    printf("Walls done!\r\n");
        
    printf("Adding boss chamber\r\n");
    add_wall(30,  0,   VERTICAL, 10);
    add_wall(30, 10, HORIZONTAL, 10);
    add_wall(39,  0,   VERTICAL, 10);
    printf("Added!\r\n");

    // randomly place boss inside chamber
    int boss_x, boss_y;
    do {
        boss_x = rand() % 9 + 31;  // x: 31-39 (inside chamber)
        boss_y = rand() % 9 + 1;   // y: 1-9 (inside chamber)
    } while (get_here(boss_x, boss_y) != NULL);
    add_boss(boss_x, boss_y);
    explorer.boss_x = boss_x;
    explorer.boss_y = boss_y;
    explorer.last_boss_move_time = us_ticker_read();
    
    // Create maze layout
    printf("Creating maze...\r\n");
    
    // Large starting room around spawn point 
    add_wall(2, 2, HORIZONTAL, 7);  
    add_wall(2, 9, HORIZONTAL, 7);     
    add_wall(2, 2, VERTICAL, 7);     
    add_wall(9, 2, VERTICAL, 7);      
    printf("Starting room created!\r\n");
    
    // Create openings in starting room
    map_erase(5, 2); 
    map_erase(9, 5);   
    map_erase(5, 9);   
    
    // Medium room
    add_wall(12, 3, HORIZONTAL, 6);  
    add_wall(12, 8, HORIZONTAL, 6);  
    add_wall(12, 3, VERTICAL, 5);    
    add_wall(18, 3, VERTICAL, 5);    
    map_erase(15, 3);  
    map_erase(12, 5); 
    printf("Medium room 1 created!\r\n");
    
    // Connecting corridor 
    add_wall(10, 4, HORIZONTAL, 2);   
    add_wall(10, 6, HORIZONTAL, 2);   
    printf("Corridor 1 created!\r\n");
    
    // Small room 
    add_wall(20, 5, HORIZONTAL, 4);
    add_wall(20, 9, HORIZONTAL, 4);
    add_wall(20, 5, VERTICAL, 4);
    add_wall(24, 5, VERTICAL, 4);
    map_erase(22, 5);  
    map_erase(20, 7);  
    printf("Small room created!\r\n");
    
    // Connecting corridor 
    add_wall(18, 6, HORIZONTAL, 2);
    map_erase(19, 7);  // opening
    printf("Corridor to small room created!\r\n");
    
    // Lower area
    add_wall(4, 12, HORIZONTAL, 6);  
    add_wall(4, 17, HORIZONTAL, 6);   
    add_wall(4, 12, VERTICAL, 5);     
    add_wall(10, 12, VERTICAL, 5);    
    map_erase(7, 12);  
    map_erase(10, 14); 
    printf("Lower area created!\r\n");
    
    // Simple vertical connecting corridor from starting room
    add_wall(5, 10, VERTICAL, 2);
    printf("Vertical corridor created!\r\n");
    
    // Medium room
    add_wall(13, 12, HORIZONTAL, 5);
    add_wall(13, 16, HORIZONTAL, 5);
    add_wall(13, 12, VERTICAL, 4);
    add_wall(18, 12, VERTICAL, 4);
    map_erase(15, 12); 
    map_erase(13, 14); 
    printf("Medium room 2 created!\r\n");
    
    // Connecting corridor between medium rooms
    add_wall(11, 13, HORIZONTAL, 2);
    add_wall(11, 15, HORIZONTAL, 2);
    printf("Inter-room corridor created!\r\n");
    
    // Large exploration chamber
    add_wall(21, 10, HORIZONTAL, 6);
    add_wall(21, 16, HORIZONTAL, 6);
    add_wall(21, 10, VERTICAL, 6);
    add_wall(27, 10, VERTICAL, 6);
    map_erase(24, 10); 
    map_erase(21, 13); 
    printf("Large exploration chamber created!\r\n");
    
    // Connecting corridor to large chamber
    add_wall(19, 12, HORIZONTAL, 2);
    add_wall(19, 14, HORIZONTAL, 2);
    printf("Chamber connector created!\r\n");
    
    // Right side room
    add_wall(32, 14, HORIZONTAL, 3);
    add_wall(32, 18, HORIZONTAL, 3);
    add_wall(32, 14, VERTICAL, 4);
    add_wall(35, 14, VERTICAL, 4);
    map_erase(33, 14); 
    map_erase(32, 16); 
    printf("Right side room created!\r\n");
    
    // Simple connecting corridor to right area
    add_wall(28, 15, HORIZONTAL, 4);
    printf("Right area connector created!\r\n");
    add_wall(6, 19, HORIZONTAL, 6);
    add_wall(6, 19, VERTICAL, 4);
    add_wall(12, 19, VERTICAL, 4);
    printf("Bottom medium room created!\r\n");
    add_wall(8, 17, VERTICAL, 2);
    printf("Bottom connector created!\r\n");
    add_wall(29, 17, HORIZONTAL, 2);
    printf("Bottom right connector created!\r\n");
    add_wall(36, 15, VERTICAL, 3);
    add_wall(36, 18, HORIZONTAL, 2);
    printf("Bottom right dead end created!\r\n");
    map_erase(5, 10);   
    map_erase(5, 11);   
   
    printf("Cave maze completed successfully!\r\n");
    
    print_map();

    // the game is run according to the pdf specs.
    // implemented after walls are drawn so we don't place powerups on walls

    // 5 nightvision items
    for (int i = 0; i < 5; i++) {
        int x, y;
        do {
            x = rand() % map_width();
            y = rand() % map_height();
        } while (get_here(x, y) != NULL);  // Find empty spot
        add_nightvision(x, y);
    }
    printf("Night vision items placed!\r\n");
    
    // 3+ heal items
    for (int i = 0; i < 3; i++) {
        int x, y;
        do {
            x = rand() % map_width();
            y = rand() % map_height();
        } while (get_here(x, y) != NULL);  // Find empty spot
        add_heal(x, y);
    }
    printf("Heal items placed!\r\n");
    
    // 3+ poison items
    for (int i = 0; i < 3; i++) {
        int x, y;
        do {
            x = rand() % map_width();
            y = rand() % map_height();
        } while (get_here(x, y) != NULL);  // Find empty spot
        add_poison(x, y);
    }
    printf("Poison items placed!\r\n");

    // 3+ xray items
    for (int i = 0; i < 3; i++) {
        int x, y;
        do {
            x = rand() % map_width();
            y = rand() % map_height();
        } while (get_here(x, y) != NULL);  // Find empty spot
        add_xrayvision(x, y);
    }
    printf("Poison items placed!\r\n");

    // 3+ blast items3
    for (int i = 0; i < 3; i++) {
        int x, y;
        do {
            x = rand() % map_width();
            y = rand() % map_height();
        } while (get_here(x, y) != NULL);  // Find empty spot
        add_blast(x, y);
    }
}

// Feel free to add any helper functions below or elsewhere in the document