#ifndef GATE_H
#define GATE_H

#include "definitions.h"
#include "../lib/raymath.h"
#include "../lib/raylib.h"
#include <stdio.h>

typedef struct Gate
{
    Color color;
    Vector2 position;
    Vector2 velocity;
    float height;
    int gap;
    int gapPercentage;
    int gapPosition;
    int maxSpeed;
    bool belowPlayer;
    bool hit;
} Gate;

void ResetGate(Gate *gate){
    gate->position.y = PLAY_HEIGHT + PLAY_PADDING;
    gate->belowPlayer = true;
    gate->hit = false;
    gate->velocity.y = 1;
    gate->height = 0;
    gate->gapPercentage = 0;
    gate->gap = (float)GetRandomValue(80, 200);
    gate->gapPosition = (float)GetRandomValue(PLAY_PADDING + 50, (PLAY_PADDING + PLAY_WIDTH - gate->gap - 50));
    gate->color = GREEN;
}

void UpdateGate(Gate *gate)
{
    const float absY = PLAY_HEIGHT + PLAY_PADDING - gate->position.y; // Get distance to bottom of playarea

    // GATE PHASES
    if ( absY < 128 ) // grow from 0 height to 128 at bottom of play area
    {
        gate->height++;
    }

    else if ( absY >= 128 && absY < 400 ) // shrink height while moving
    {
        gate->height = Remap(absY, MAX_GATE_HEIGHT, 400, MAX_GATE_HEIGHT, MIN_GATE_HEIGHT);
    }

    else if ( absY >= 400 ) // open gap and accelerate
    {
        gate->height = MIN_GATE_HEIGHT;
        if ( gate->gapPercentage < 100 ) gate->gapPercentage = Remap(absY, 400, 800, 0, 100);
        if (gate->velocity.y < MAX_GATE_SPEED) gate->velocity.y += GATE_ACCELERATION;
    }

    if ( absY > PLAY_HEIGHT ) // reset gate at top of screen
    {
        ResetGate(gate);
    }

    // update gate position
    gate->position.y -= gate->velocity.y;

}

void DrawGate(Gate *gate)
{
    const Color color = gate->hit ? RED : gate->color;
    const int gapCenter = ( gate->gap / 2 ) + gate->gapPosition;
    const int opening = (float)gate->gap / 100 * gate->gapPercentage;
    const int gapStart = gapCenter - opening / 2;
    const int gapEnd = gapCenter + opening / 2;
    // debug printf("Opening: %d \n Percentage: %d \n Gap: %d \n", opening, gate->gapPercentage, gate->gap);

    if ( gate->gapPercentage > 1 && gate->gapPercentage < 100 )
    {
        DrawRectangle( gapStart, gate->position.y, opening, gate->height, WHITE );
    }
    
    DrawRectangle(PLAY_PADDING, gate->position.y, gapStart - PLAY_PADDING, gate->height, color);
    DrawRectangle(gapEnd, gate->position.y, PLAY_WIDTH - gapEnd + PLAY_PADDING, gate->height, color);
    
}

#endif // GATE_H
