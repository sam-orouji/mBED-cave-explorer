// ============================================
// The explorer class file
//
// Copyright 2025 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================

#include "explorer.h"

void explorer_init (Explorer * e)
{
    // 1. Set starting and previous coordinates for your explorer (top left corner recommended)
    // 2. Set starting lives and score
    // 3. Initialize any other variables you added to the Explorer struct
    e->loc_x = 0;
    e->loc_y = 0;
    e->loc_x_prev = 0;
    e->loc_y_prev = 0;

    e-> score = 0;
    e->lives = 1;
    e-> paused = false;
    e-> leaderboard = 0;
    
    e-> has_night_vision = false;
    e-> night_vision_start_time = 0;
    e-> teleporter_spawned = false;
    e-> teleporter_x = -1;
    e-> teleporter_y = -1;
    e-> has_xray_vision = false;
    e-> xray_vision_start_time = 0;

    e-> return_to_menu = false;

    e-> boss_x = -1;
    e-> boss_y = -1;
    e-> boss_lives = 3;
    e-> blast_waves_owned = 0;
    e-> last_boss_move_time = 0;
    e-> shockwave_active = false;
    e-> shockwave_start_time = 0;
}
