// ============================================
// The header file for explorer class file.
//
// Copyright 2025 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


#ifndef EXPLORER_H
#define EXPLORER_H


// Explorer Structure
typedef struct {
    int loc_x, loc_y, loc_x_prev, loc_y_prev; // Location of the explorer
    int score; //Current score of the explorer
    int lives; // Current lives of the explorer
    int character_selected; // 0,1,2,3 red blue green yellow

    bool paused; // state management of game
    int leaderboard;
    
    // add sword or other tool statuses as desired (or can be placed elsewhere in code)
    unsigned long long night_vision_start_time;
    bool has_night_vision;
    bool teleporter_spawned;
    bool has_xray_vision;
    int xray_vision_start_time;
    int teleporter_x;
    int teleporter_y;
    bool return_to_menu;


    // boss
    int boss_x;
    int boss_y;
    int boss_lives;
    unsigned long long last_boss_move_time;
    int blast_waves_owned;
    bool shockwave_active;
    unsigned long long shockwave_start_time;

    // 
} Explorer;

// Initialize an Explorer structure
// You want to assign initial values to each of the variables defined above
// in the Explorer structure.
void explorer_init (Explorer* explorer);

#endif // EXPLORER_H