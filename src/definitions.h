#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include "../lib/raylib.h"

// layout
#define SCREENWIDTH 1080
#define SCREENHEIGHT 1920
#define SHIPWIDTH 36
#define SHIPHEIGHT 48
#define PLAY_WIDTH 984
#define PLAY_HEIGHT 1718
#define PLAY_PADDING 48
#define SHIP_POSITION_Y 340
#define TEXT_SIZE_L 36
#define TEXT_SIZE_M 24
#define TEXT_SIZE_S 16
#define MAX_GATE_HEIGHT 128
#define MIN_GATE_HEIGHT 4

// physics
#define INERTIA 0.1f
#define ROTATION_INERTIA 0.1
#define MAX_VELOCITY 100.0f
#define FALL_RATE 5
#define MAX_GATE_SPEED 10
#define GATE_ACCELERATION 0.1f

// gamelogic
#define MAX_GATES 10
#define MAX_LINES 10

// colors
#define DARKEST_GRAY CLITERAL(Color) { 8, 8, 8, 255 }
#define MINT CLITERAL(Color) { 27, 246, 141, 255 }
#define DARK_MINT CLITERAL(Color) { 39, 170, 107, 255 }
#define LINE_COLOR CLITERAL(Color) { 255, 255, 255, 100 }

// types

typedef struct Ship
{
    Color color;
    Vector2 position;
    Vector2 velocity;
    float rotation;
    Vector2 p1;
    Vector2 p2;
    Vector2 p3;
} Ship;

typedef struct Line
{
    Vector2 p1;
    Vector2 p2;
    Color color;
} Line;

#endif // DEFINITIONS_H