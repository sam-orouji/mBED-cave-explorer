// ============================================
// The Map class file
//
// Copyright 2025 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


#include "map.h"

#include "globals.h"
#include "graphics.h"
#define MHF_NBUCKETS 97

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */
struct Map {
    HashTable* items;
    int w, h;
};

#define NUM_MAPS 1

static Map map;
static int active_map;

static const MapItem CLEAR_SENTINEL = {
    .type = CLEAR,
    .draw = draw_nothing
};

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y) {
    // Hint: Think about how you accessed location in Project 1 (0 to 999, 40*y for row + x for item)
    // 1. Return the 1-D xy key
    return ((Y * map.w) + X);
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{
    // Hint: what math operator can be used as an easy way to hash a key?
    // 1. Use MHF_NBUCKETS as part of your hash function
    // 2. Return the hashed key
    return (key % MHF_NBUCKETS);
}

/**
 * Initializes the map, using a hash_table, setting the width and height.
 */
void map_init()
{
    // 1. Set width & height for your map
    // 2. Create hashtable for its items
    map.w = 40;
    map.h = 25;
    map.items = createHashTable(map_hash, MHF_NBUCKETS);
}

Map* get_active_map()
{
    return &map;
}

void print_map()
{
    char lookup[] = {'W', 'D', 'P', 'A', 'K', 'C', 'N',' ','S'};
    Map* map = get_active_map();
    for(int j = 0; j < map->h; j++)
    {
        for (int i = 0; i < map->w; i++)
        {
            MapItem* item = (MapItem*)getItem(map->items, XY_KEY(i, j));
            if (item && item->type < 8) printf("%c", lookup[item->type]);
            else printf(" ");
        }
        printf("\r\n");
    }
}

/**
 * Returns width of active map
 */
int map_width()
{
    // Hint 1 for this and below functions: use another function to get the current active map
    // Hint 2: check the struct of a map item
    return (get_active_map()->w);
}

/**
 * Returns height of active map
 */
int map_height()
{
    return (get_active_map()->h);
}

/**
 * Returns the area of the active map
 */
int map_area()
{
    // HINT: You have the width and height to compute area
    return (map_height() * map_width());
}

/**
 * Returns MapItem at current coordinate location
 */
MapItem* get_current(int x, int y)
{
    // 1. Get map item
    Map* map = get_active_map();
    unsigned int key = XY_KEY(x, y);
    MapItem* current_item = (MapItem*)getItem(map->items, key);
    // 2. Check and remove item if item exist and is clear type
    if (current_item && current_item->type == CLEAR) {
        removeItem(map->items, key);
        free(current_item);
        return NULL;
    }
    // 3. Return the item
    return current_item;
}

/**
 * Returns the MapItem immediately above the given location.
 */
MapItem* get_north(int x, int y) // get north item
{
    // 1. Get map item 
    // 2. Check and remove item if item exist and is clear type
    // 3. Return the item 
    return get_current(x,y - 1);
}

/**
 * Returns the MapItem immediately below the given location.
 */
MapItem* get_south(int x, int y) // get south item
{
    // 1. Get map item
    // 2. Check and remove item if item exist and is clear type
    // 3. Return the item
    return get_current(x,y + 1);
}

/**
 * Returns the MapItem immediately right the given location.
 */
MapItem* get_east(int x, int y) // get east item
{
    // 1. Get map item
    // 2. Check and remove item if item exist and is clear type
    // 3. Return the item
    return get_current(x + 1,y);
}

/**
 * Returns the MapItem immediately left the given location.
 */
MapItem* get_west(int x, int y) // get west item
{
    // 1. Get map item
    // 2. Check and remove item if item exist and is clear type
    // 3. Return the item
    return get_current(x - 1,y); 
}

/**
 * Returns the MapItem at current coordinate location
 */
 MapItem* get_here(int x, int y) // same as get_current
 {
     // You may copy the same code fro get_current for get_here
     // 1. Get map item
     // 2. Check and remove item if item exist and is clear type
     // 3. Return xthe item
     return get_current(x,y);
 }

/**
 * Erase current location map item
 */
void map_erase(int x, int y)
{
    //1. Insert clear value to the map item at current location
    Map* map = get_active_map();
    unsigned key = XY_KEY(x, y);
    //2. Free current location map item if it exists
    void* val = removeItem(map->items, key);
    if (val) {
        free(val);
    }
}

void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_nightvision(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = NIGHTVISION;
    w1->draw = draw_nightvision;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

//  I'm using this to print the boss (if it walks into you you lose, 1 function if on same square)
                                    // if you shockwave on the tile its on you win
                                    // update() logic to "randomly" make him move 1 tile in some direction
void add_boss(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = EXPLORER;
    w1->draw = draw_boss;
    w1->walkable = true; 
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_heal(int x, int y) // code to add a heal item
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = HEAL;
    w1->draw = draw_heal;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
void add_poison(int x, int y) // code to add a poison
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = POISON;
    w1->draw = draw_poison;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_teleporter(int x, int y, int dest_x, int dest_y) // code to add a simple teleporter
{
    MapItem* teleporter = (MapItem*) malloc(sizeof(MapItem));
    teleporter->type = TELEPORTER;
    teleporter->draw = draw_teleporter;
    teleporter->walkable = true;
    teleporter->data = NULL; // Keep it simple, no destination data needed
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), teleporter);
    if (val) free(val); // If something is already there, free it
}

// item that allows you to walk through walls for 10 seconds
void add_xrayvision(int x, int y) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = XRAY;
    w1->draw = draw_xray;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it    
}

// blast items
void add_blast(int x, int y) {
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = BLAST;
    w1->draw = draw_blast;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it    
}


// only count score for visited blocks - add to array
void add_visited(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = VISITED;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val);
}

