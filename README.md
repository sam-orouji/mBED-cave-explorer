# Cave Explorer Game

**Video Demo**: https://youtu.be/M8nHtz5CYdw?si=tEtlb0UxtcA14BqF

A 2D exploration game built on the ARM mbed platform using embedded C programming. Navigate through a mysterious cave, collect powerups, avoid hazards, and defeat the boss to escape!

## Technical Overview

Cave Explorer implements a custom hash table data structure to manage tile positions across a 40x25 tile map. The game runs on a fixed-interval game loop (100ms frames) handling input reading, state updates, and rendering each cycle.

### Hardware Components
- ARM mbed microcontroller
- 128x128 pixel uLCD display
- 5-direction navigation switch
- Pushbuttons on breadboard

### Software Modules
- `map.cpp/h`: Hash table-based world representation with MapItem management
- `graphics.cpp/h`: Sprite rendering and coordinate transformations
- `hash_table.cpp/h`: Custom hash table implementation
- `explorer.h`: Player state management
- `main.cpp`: Game loop and input handling

## Gameplay

### Controls
- **Nav-Switch**: Move in 4 directions
- **Center Nav-Switch**: Win game in boss chamber
- **Pushbutton 1**: Pause/Resume
- **Pushbutton 2**: Use blast wave attack

### Objective
1. Explore the dark cave with limited visibility
2. Collect night vision, health, and blast wave powerups
3. Avoid poison items
4. Reach 100 points to unlock the teleporter
5. Enter boss chamber and defeat the boss using stackable blast wave attacks
6. Victory!

### Scoring System
- +1 point per tile traversed
- +10 points for night vision powerup
- +5 points per blast wave attack used
- High scores tracked on leaderboard

## Features Implemented

**Basic Features (40pts)**
- 4-directional movement, status display, scoring system
- Night vision powerups with temporary full visibility
- Poison/heal items, custom sprites, teleporter system
- Win/loss conditions

**Advanced Features (45pts)**
- Interactive start menu with character select & instructions (5pts)
- 5+ custom sprites for characters, boss, and powerups (5pts)
- Pause/resume game state management (5pts)
- X-ray powerup for wall phasing (5pts)
- Stackable blast wave attack system (5pts)
- Boss fight with random movement and health system (10pts)
- Replay without mbed reset (5pts)
- Persistent leaderboard tracking (5pts)

---
