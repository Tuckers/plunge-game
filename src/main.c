#include "../lib/raylib.h"
#include "../lib/raymath.h"

#define MAX_GATES   100
#define INERTIA     0.1f
#define ROTATIONINERTIA 0.1f
#define SCREENWIDTH 1080
#define SCREENHEIGHT 1920
#define SHIPWIDTH 36
#define SHIPHEIGHT 48
#define SHIPVERTICALPOSITION 340
#define MAX_VELOCITY 100.0f

typedef struct Gate {
    Color color;
    Vector2 position;
    Vector2 velocity;
    int gap;
    int gapPosition;
    int maxSpeed;
} Gate;

typedef struct Ship {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    Color color;
} Ship;

void DrawGate(Gate gate){
    DrawRectangle( 0, gate.position.y, gate.gapPosition, 20, gate.color );
    DrawRectangle( gate.gapPosition + gate.gap, gate.position.y, SCREENWIDTH - (gate.gapPosition + gate.gap), 20, gate.color );
}

void UpdateShip(Ship *ship){
    // apply inertia
    if (ship->velocity.x > 0.1f) ship->velocity.x -= INERTIA;
    if (ship->velocity.x < -0.1f) ship->velocity.x += INERTIA;
    // apply rotation inertia
    if (ship->rotation > 0.1f) ship->rotation -= ROTATIONINERTIA;
    if (ship->rotation < -0.1f) ship->rotation += ROTATIONINERTIA;
    // add velocity to position
    ship->position.x += ship->velocity.x;
    // check for out of bounds
    if (ship->position.x < (float)SHIPWIDTH / 2) ship->position.x = (float)SHIPWIDTH / 2;
    if (ship->position.x > (float)SCREENWIDTH - SHIPWIDTH / 2) ship->position.x = (float)SCREENWIDTH - SHIPWIDTH / 2;

}

void DrawShip(Ship *ship){
    const float halfHeight = SHIPHEIGHT / 2;
    const float halfWidth = SHIPWIDTH / 2;
    // Rotate triangle points
    const Vector2 v1 = Vector2Rotate((Vector2){ -halfWidth, -halfHeight }, DEG2RAD * ship->rotation);
    const Vector2 v2 = Vector2Rotate((Vector2){ 0.0f, halfHeight }, DEG2RAD * ship->rotation);
    const Vector2 v3 = Vector2Rotate((Vector2){ halfWidth, -halfHeight }, DEG2RAD * ship->rotation);
    // Add position and draw triangle
    DrawTriangle( Vector2Add(v1, ship->position), Vector2Add(v2, ship->position), Vector2Add(v3, ship->position), ship->color);
}


// START -------------------------------------------------------------------------------------------------//

int main(void)
{
    // INITIALIZATION ------------------------------------------------------------------------------------//
    const int screenHeight = SCREENHEIGHT;
    const int screenWidth = SCREENWIDTH;
    float distance = 0.0f;

    Ship playerShip = { 
        {(float)screenWidth / 2 , SHIPVERTICALPOSITION},
        {0.0f , 0.0f},
        0.0f,
        RED,
     };

    Gate gates[MAX_GATES] = { 0 };

    for (int i = 0; i < MAX_GATES; i++)
    {
        gates[i].gap = (float)GetRandomValue(50, 200);
        gates[i].gapPosition = (float)GetRandomValue(50, (screenWidth - gates[i].gap - 50));
        gates[i].color = (Color){ GetRandomValue(200, 240), GetRandomValue(200, 240), GetRandomValue(200, 250), 255 };
        gates[i].position.x = 0.0f;
        gates[i].position.y = (float)screenHeight;
        gates[i].velocity.x = 0.0f;
        gates[i].velocity.y = 0.0f;
    }

    InitWindow(screenWidth, screenHeight, "plunge – game");

    Camera2D camera = { 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    SetTargetFPS(60);

    // MAIN LOOP ----------------------------------------------------------------------------------------//
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        // UPDATE PLAYER MOVEMENT

        if (gates[0].position.y < 0) gates[0].position.y = screenHeight;
        
        distance += 5;

        // add input to velocity
        if (IsKeyDown(KEY_RIGHT)) {
            if (playerShip.velocity.x < MAX_VELOCITY) playerShip.velocity.x += 1.5f;
            if (playerShip.rotation > -60) playerShip.rotation -= 1.0f;
        }
        if (IsKeyDown(KEY_LEFT)) {
            if (playerShip.velocity.x > -MAX_VELOCITY) playerShip.velocity.x -= 1.5f;
            if (playerShip.rotation < 60) playerShip.rotation += 1.0f;
        }

        UpdateShip( &playerShip );

        // gate velocity
        if (gates[0].velocity.y > -10) gates[0].velocity.y -= 0.3f;
        gates[0].position.y += gates[0].velocity.y;

        // DRAW FRAME
        BeginDrawing();

            ClearBackground(BLACK);

            BeginMode2D(camera);

                DrawShip( &playerShip );
                DrawGate( gates[0] ); 

            EndMode2D();  
            DrawText(TextFormat("%000000009.0f M", distance), 450, 96, 36, WHITE);

        EndDrawing();
    }

    // De-Initialization ------------------------------------------------------------------------------------//
    CloseWindow();        // Close window and OpenGL context
    return 0;
}

