// ============================================
// The Graphics class file
//
// Copyright 2025 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


#include "graphics.h"

#include "globals.h"

void draw_nothing(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, BLACK);
}

void draw_img(int u, int v, const char* img)
{
    int colors[11*11];
    for (int i = 0; i < 11*11; i++)
    {
        if (img[i] == 'R') colors[i] = RED;
        else if (img[i] == 'Y') colors[i] = 0xFFFF00;   // Yellow
        else if (img[i] == 'G') colors[i] = GREEN;
        else if (img[i] == 'B') colors[i] = BLUE;
        else if (img[i] == 'O') colors[i] = DORANGE;
        else if (img[i] == 'D') colors[i] = 0xD2691E;   // "Dirt"
        else if (img[i] == '5') colors[i] = LGREY;      // 50% grey
        else if (img[i] == '3') colors[i] = DGREY;
        else if (img[i] == 'W') colors[i] = 0xFFFFFF; // white
        else if (img[i] == 'Z') colors[i] = BLACK;
        else if (img[i] == 'P') colors[i] = 0x800080; // purple
        else colors[i] = BLACK;
    }
    uLCD.BLIT(u, v, 11, 11, colors);
    wait_us(250); // Recovery time!
}

// For each sprite, there is an array of 121 characters describing the color of each pixel of the tile where the sprite is drawn
// Characters describe colors according to draw_img
// Feel free to add more!
void draw_wall(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, 0x8e8e8e8e);
}

void draw_boss(int u, int v)
{
    char explorer_sprite[122] = 
        "ZZPPPPPPPZZ"
        "ZZPPWPWPPZZ"
        "ZZPPPPPPPZZ"
        "ZZPPWWWPPZZ"
        "ZZPWPPPWPZZ"
        "ZZZZPPPZZZZ"
        "PPPPPPPPPPP"
        "PPPPPPPPPPP"
        "ZZZZPPPZZZZ"
        "PPPPPZPPPPP"
        "PPPPPZPPPPP";
    draw_img(u, v, explorer_sprite);
}

void draw_nightvision(int u, int v)
{
    char star_sprite[122] = 
        "ZZZZZYZZZZZ" 
        "ZZZZYYYZZZZ"  
        "ZZZYYYYYZZZ"  
        "YYYYYYYYYYY" 
        "ZYYYYYYYYYZ"  
        "ZYYYZZZYYYZ"  
        "ZZYYYYYYYZZ"  
        "ZZZYYYYYZZZ" 
        "ZZZYYYYZZZZ"  
        "ZZZYZZZYZZZ"  
        "ZZYZZZZZYZZ";
    draw_img(u, v, star_sprite);
}




// main character is red
void draw_red_explorer(int u, int v)
{
    // white explorer
    char explorer_sprite[122] = 
        "ZZRRRRRRRZZ"
        "ZZRRWRWRRZZ"
        "ZZRRRRRRRZZ"
        "ZZRWRRRWRZZ"
        "ZZRRWWWRRZZ"
        "ZZZZRRRZZZZ"
        "RRRRRRRRRRR"
        "RRRRRRRRRRR"
        "ZZZZRRRZZZZ"
        "RRRRRZRRRRR"
        "RRRRRZRRRRR";
    draw_img(u, v, explorer_sprite);
}

void draw_blue_explorer(int u, int v)
{   
    // blue explorer
    char explorer_sprite[122] = 
        "ZZBBBBBBBZZ"
        "ZZBBWBWBBZZ"
        "ZZBBBBBBBZZ"
        "ZZBWBBBWBZZ"
        "ZZBBWWWBBZZ"
        "ZZZZBBBZZZZ"
        "BBBBBBBBBBB"
        "BBBBBBBBBBB"
        "ZZZZBBBZZZZ"
        "BBBBBZBBBBB"
        "BBBBBZBBBBB";
    draw_img(u, v, explorer_sprite);
}

void draw_green_explorer(int u, int v)
{
    char explorer_sprite[122] = 
        "ZZGGGGGGGZZ"
        "ZZGGZGZGGZZ"
        "ZZGGGGGGGZZ"
        "ZZGZGGGZGZZ"
        "ZZGGZZZGGZZ"
        "ZZZZGGGZZZZ"
        "GGGGGGGGGGG"
        "GGGGGGGGGGG"
        "ZZZZGGGZZZZ"
        "GGGGGZGGGGG"
        "GGGGGZGGGGG";
    draw_img(u, v, explorer_sprite);
}

void draw_yellow_explorer(int u, int v)
{
    char explorer_sprite[122] = 
        "ZZYYYYYYYZZ"
        "ZZYYZYZYYZZ"
        "ZZYYYYYYYZZ"
        "ZZYZYYYZYZZ"
        "ZZYYZZZYYZZ"
        "ZZZZYYYZZZZ"
        "YYYYYYYYYYY"
        "YYYYYYYYYYY"
        "ZZZZYYYZZZZ"
        "YYYYYZYYYYY"
        "YYYYYZYYYYY";
    draw_img(u, v, explorer_sprite);
}

void draw_blast(int u, int v) 
{
    char snowflake_sprite[122] = 
        "ZZZZZWZZZZZ"
        "ZZWZZWZZWZZ"
        "ZZZWZWZWZZZ"
        "ZZZZZWZZZZZ"
        "ZWWWWWWWWWZ"
        "ZZZZZWZZZZZ"
        "ZZZWZWZWZZZ"
        "ZZWZZWZZWZZ"
        "ZZZZZWZZZZZ"
        "ZZZZZZZZZZZ"
        "ZZZZZZZZZZZ";
    draw_img(u, v, snowflake_sprite);
}

void draw_heal(int u, int v)
{
    // mushroom = heal
    char mushroom_sprite[122] = 
        "ZZZZZZZZZZZ"
        "ZZZRRRRRZZZ"
        "ZZRRWRRWRZZ"
        "ZRRRRRRRRRZ"
        "ZRRWRRRRWRZ"
        "ZRRRRRRRRRZ"
        "ZZZDDDDDZZZ"
        "ZZZZDDDZZZ"
        "ZZZZDDDZZZ"
        "ZZZZDDDZZZ"
        "ZZZZZZZZZZZ";
    draw_img(u, v, mushroom_sprite);
}

void draw_poison(int u, int v)
{
    char poison_sprite[122] = 
        "ZZZZWWWZZZZ"
        "ZZZZWWWZZZZ"
        "ZZZGGGGGZZZ"
        "ZZGGGGGGGZZ"
        "ZZGGGGGGGZZ"
        "ZZGGGGGGGZZ"
        "ZZGGGGGGGZZ"
        "ZZGGGGGGGZZ"
        "ZZZGGGGGZZZ"
        "ZZZGGGGGZZZ"
        "ZZZZZZZZZZZ";
    draw_img(u, v, poison_sprite);
}

void draw_teleporter(int u, int v)
{
    char teleporter_sprite[122] = 
        "ZZZZZZZZZZZ"
        "ZZZPPPPPZZZ"
        "ZZPZZZZZPZZ"
        "ZPZZPPPZZPZ"
        "ZPZPZZZPZPZ"
        "ZPZPZZZPZPZ"
        "ZPZPZZZPZPZ"
        "ZPZZPPPZZPZ"
        "ZZPZZZZZPZZ"
        "ZZZPPPPPZZZ"
        "ZZZZZZZZZZZ";
    draw_img(u, v, teleporter_sprite);
}

void draw_xray (int u, int v)
{
     char teleporter_sprite[122] = 
        "ZZZZZZZZZZZ"
        "ZZZZWWWZZZZ"
        "ZZZWWWWWZZZ"
        "ZZWWZWZWWZZ"
        "ZZWZZWZZWZZ"
        "ZZWWWWWWWZZ"
        "ZZWWZZZWWZZ"
        "ZZWWZZZWWZZ"
        "ZWWWWWWWWWZ"
        "ZWZWZZZWZWZ"
        "ZZZZZZZZZZZ";
    draw_img(u, v, teleporter_sprite);
}

// Provided: calculate which sprite to draw based on the screen position
// Use this in draw_game()
void draw_dark_outline(int u, int v)
{
    // Convert screen coordinates back to relative tile position
    int i = (u - 3) / 11 - 5;  // Calculate i from u
    int j = (v - 15) / 11 - 4; // Calculate j from v
    
    // Fill the entire tile with dark color first
    uLCD.filled_rectangle(u, v, u+10, v+10, BLACK);
    
    // Determine which side is closest to the center and draw orange line
    if (abs(i) > abs(j)) {
        // Horizontal edge is closest to center
        if (i < 0) {
            // Right edge (orange line on right)
            uLCD.filled_rectangle(u+10, v, u+10, v+10, DORANGE);  // orange line on right edge
        } else {
            // Left edge (orange line on left)
            uLCD.filled_rectangle(u, v, u, v+10, DORANGE);  // orange line on left edge
        }
    } else if (abs(i) != abs(j)) {
        // Vertical edge is closest to center
        if (j < 0) {
            // Bottom edge (orange line on bottom)
            uLCD.filled_rectangle(u, v+10, u+10, v+10, DORANGE);  // orange line on bottom edge
        } else {
            // Top edge (orange line on top)
            uLCD.filled_rectangle(u, v, u+10, v, DORANGE);  // orange line on top edge
        }
    }
}