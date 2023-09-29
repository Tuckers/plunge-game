#include "../lib/raylib.h"
#include "../lib/raymath.h"
#include "definitions.h"
#include "gate.h"

int score = 0;

void UpdateShip(Ship *ship)
{
    const float halfHeight = (float)SHIPHEIGHT / 2;
    const float halfWidth = (float)SHIPWIDTH / 2;
    const int rotationOffset = 0;
    // apply inertia
    if (ship->velocity.x > 0.1f)
        ship->velocity.x -= INERTIA;
    if (ship->velocity.x < -0.1f)
        ship->velocity.x += INERTIA;
    // apply rotation inertia
    if (ship->rotation > 0.1f)
        ship->rotation -= ROTATION_INERTIA;
    if (ship->rotation < -0.1f)
        ship->rotation += ROTATION_INERTIA;
    // add velocity to position
    ship->position.x += ship->velocity.x;
    // check for out of bounds
    if (ship->position.x < (float)SHIPWIDTH / 2)
        ship->position.x = (float)SHIPWIDTH / 2;
    if (ship->position.x > (float)SCREENWIDTH - (float)SHIPWIDTH / 2)
        ship->position.x = (float)SCREENWIDTH - (float)SHIPWIDTH / 2;
    // rotate triangle points
    const Vector2 v1 = Vector2Rotate((Vector2){-halfWidth, -halfHeight - rotationOffset}, DEG2RAD * ship->rotation);
    const Vector2 v2 = Vector2Rotate((Vector2){0.0f, halfHeight - rotationOffset}, DEG2RAD * ship->rotation);
    const Vector2 v3 = Vector2Rotate((Vector2){halfWidth, -halfHeight - rotationOffset}, DEG2RAD * ship->rotation);
    // add position and update ship values
    ship->p1 = Vector2Add(v1, ship->position);
    ship->p2 = Vector2Add(v2, ship->position);
    ship->p3 = Vector2Add(v3, ship->position);
}

void UpdateLine(Line *line)
{
    line->p1.y -= 10;
    line->p2.y -= 10;
    if (line->p1.y < PLAY_PADDING)
    {
        line->p1.y = PLAY_HEIGHT + PLAY_PADDING;
        line->p2.y = PLAY_HEIGHT + PLAY_PADDING;
    }
}

void checkCollision(Gate *gate, Ship *ship)
{
    Vector2 collisionPoint;
    // check to see if left or right side of triangle contact gate
    const int topOfGate = gate->position.y + gate->height / 2;
    const bool leftCheck = CheckCollisionLines(ship->p1, ship->p2, (Vector2){PLAY_PADDING, topOfGate}, (Vector2){gate->gapPosition, topOfGate}, &collisionPoint);
    const bool rightCheck = CheckCollisionLines(ship->p3, ship->p2, (Vector2){gate->gapPosition + gate->gap, topOfGate}, (Vector2){PLAY_WIDTH + PLAY_PADDING, topOfGate}, &collisionPoint);
    const bool collision = leftCheck || rightCheck;

    if (collision && gate->belowPlayer)
    {
        gate->hit = true;
        gate->belowPlayer = false;
        score--;
    }
    if (gate->position.y < ship->position.y && gate->belowPlayer)
    {
        gate->belowPlayer = false;
        score++;
    }
}

void DrawShip(Ship *ship)
{
    // Draw triangle
    DrawTriangle(ship->p1, ship->p2, ship->p3, ship->color);
}

void DrawLineType(Line *line)
{
    DrawLineEx(line->p1, line->p2, 1, line->color);
}

// START -------------------------------------------------------------------------------------------------//

int main(void)
{
    // INITIALIZATION ------------------------------------------------------------------------------------//
    const int screenHeight = SCREENHEIGHT;
    const int screenWidth = SCREENWIDTH;
    float distance = 0.0f;

    Ship playerShip = {
        MINT,
        {(float)screenWidth / 2, SHIP_POSITION_Y},
        {0.0f, 0.0f},
        0.0f}; 

    Gate gates[MAX_GATES] = {0};
    Line lines[MAX_LINES] = {0};

    for (int i = 0; i < MAX_LINES; i++)
    {
        const int lineY = (PLAY_HEIGHT / (MAX_LINES)*i) + PLAY_PADDING;
        lines[i].p1 = (Vector2){PLAY_PADDING, lineY};
        lines[i].p2 = (Vector2){PLAY_WIDTH + PLAY_PADDING, lineY};
        lines[i].color = LINE_COLOR;
    }

    for (int i = 0; i < MAX_GATES; i++)
    {
        ResetGate(&gates[i]);
    }
    gates[0].position.y = 400;
    gates[1].position.y = 325;
    gates[2].position.y = 275;
    gates[3].position.y = 250;
    gates[4].position.y = 225;
    gates[5].position.y = 200;
    gates[6].position.y = 150;
    gates[7].position.y = 0;

    SetTargetFPS(60);
    InitWindow(screenWidth, screenHeight, "plunge – game");

    Camera2D camera = {0};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    // MAIN LOOP ----------------------------------------------------------------------------------------//
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        distance += FALL_RATE;

        // add input to velocity
        if (IsKeyDown(KEY_RIGHT))
        {
            if (playerShip.velocity.x < MAX_VELOCITY)
                playerShip.velocity.x += 1.5f;
            if (playerShip.rotation > -60)
                playerShip.rotation -= 1.0f;
        }
        if (IsKeyDown(KEY_LEFT))
        {
            if (playerShip.velocity.x > -MAX_VELOCITY)
                playerShip.velocity.x -= 1.5f;
            if (playerShip.rotation < 60)
                playerShip.rotation += 1.0f;
        }
        if (IsKeyPressed(KEY_SPACE))
        {
			if (IsWindowFullscreen())
			{
				ToggleFullscreen();
				SetWindowSize(screenWidth, screenHeight);
			}
			else
			{
				int monitor = GetCurrentMonitor();
				SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
				ToggleFullscreen();
			}
		}

        // update player and gates
        UpdateShip(&playerShip);
        for (int i = 0; i < 8; i++)
            UpdateGate(&gates[i]);
        for (int i = 0; i < MAX_GATES; i++)
            UpdateLine(&lines[i]);

        // check current gate for collisions
        checkCollision(&gates[0], &playerShip);

        // DRAW FRAME
        BeginDrawing();

            ClearBackground(BLACK);

            BeginMode2D(camera);

                DrawRectangle(PLAY_PADDING, PLAY_PADDING, PLAY_WIDTH, PLAY_HEIGHT, DARKEST_GRAY);

                for (int i = 0; i < MAX_GATES; i++)
                    DrawLineType(&lines[i]);
                for (int i = 0; i < 8; i++)
                    DrawGate(&gates[i]);
                DrawShip(&playerShip);

            EndMode2D();
            
            DrawText(TextFormat("%000000009.0f M", distance), 450, 96, 36, WHITE);
            DrawText(TextFormat("SCORE: %d00", score), 450, 150, 36, WHITE);
            // DEBUG
            DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 20, WHITE);
            // DrawText(TextFormat("Collision: %s", collision ? "true" : "false"), 10, 10, 24, collision ? RED : WHITE);

        EndDrawing();
    }

    // De-Initialization ------------------------------------------------------------------------------------//
    CloseWindow(); // Close window and OpenGL context
    return 0;
}
