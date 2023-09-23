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
    bool belowPlayer;
    bool hit;
} Gate;

typedef struct Ship {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    Color color;
    Vector2 p1;
    Vector2 p2;
    Vector2 p3;
} Ship;

void DrawGate(Gate *gate)
{
    Color color = gate->hit ? RED : gate->color;
    DrawRectangle( 0, gate->position.y, gate->gapPosition, 20, color );
    DrawRectangle( gate->gapPosition + gate->gap, gate->position.y, SCREENWIDTH - (gate->gapPosition + gate->gap), 20, color );
}

void UpdateGate(Gate *gate)
{
    if (gate->position.y < 0)
    {
        gate->position.y = SCREENHEIGHT;
        gate->belowPlayer = true;
        gate->hit = false;
    }
    
    // adjust gate velocity
    if (gate->velocity.y > -10) gate->velocity.y -= 0.3f;
    // update gate position
    gate->position.y += gate->velocity.y;
}

void UpdateShip(Ship *ship)
{
    const float halfHeight = SHIPHEIGHT / 2;
    const float halfWidth = SHIPWIDTH / 2;
    const int rotationOffset = 0;
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
    // rotate triangle points
    const Vector2 v1 = Vector2Rotate((Vector2){ -halfWidth, -halfHeight - rotationOffset }, DEG2RAD * ship->rotation);
    const Vector2 v2 = Vector2Rotate((Vector2){ 0.0f, halfHeight - rotationOffset}, DEG2RAD * ship->rotation);
    const Vector2 v3 = Vector2Rotate((Vector2){ halfWidth, -halfHeight - rotationOffset}, DEG2RAD * ship->rotation);
    // add position and update ship values
    ship->p1 = Vector2Add(v1, ship->position);
    ship->p2 = Vector2Add(v2, ship->position);
    ship->p3 = Vector2Add(v3, ship->position);
}

bool checkCollision(Gate *gate, Ship *ship){
        Vector2 collisionPoint;
        // check to see if left or right side of triangle contact gate
        const bool leftCheck = CheckCollisionLines(ship->p1, ship->p2, gate->position, (Vector2){gate->gapPosition, gate->position.y}, &collisionPoint);
        const bool rightCheck = CheckCollisionLines(ship->p3, ship->p2, (Vector2){gate->gapPosition + gate->gap, gate->position.y}, (Vector2){SCREENWIDTH, gate->position.y}, &collisionPoint);
        return leftCheck || rightCheck;

}

void DrawShip(Ship *ship){
    // Draw triangle
    DrawTriangle( ship->p1, ship->p2, ship->p3, ship->color);
}


// START -------------------------------------------------------------------------------------------------//

int main(void)
{
    // INITIALIZATION ------------------------------------------------------------------------------------//
    const int screenHeight = SCREENHEIGHT;
    const int screenWidth = SCREENWIDTH;
    float distance = 0.0f;
    int score = 0;

    Ship playerShip = { 
        {(float)screenWidth / 2 , SHIPVERTICALPOSITION},
        {0.0f , 0.0f},
        0.0f,
        RED
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
        gates[i].belowPlayer = true;
        gates[i].hit = false;
    }

    InitWindow(screenWidth, screenHeight, "plunge – game");

    Camera2D camera = { 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    SetTargetFPS(60);

    // MAIN LOOP ----------------------------------------------------------------------------------------//
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
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

        // update player and gates
        UpdateShip( &playerShip );
        UpdateGate( &gates[0] );
        
        // check current gate for collisions
        bool collision = checkCollision(&gates[0], &playerShip);

        // update score
        if (collision && gates[0].belowPlayer) 
        {
            gates[0].hit = true;
            gates[0].belowPlayer = false;
            score--;
        }
        if (gates[0].position.y < playerShip.position.y && gates[0].belowPlayer)
        {
            gates[0].belowPlayer = false;
            score++;
        }

        // DRAW FRAME
        BeginDrawing();

            ClearBackground(BLACK);

            BeginMode2D(camera);

                DrawShip( &playerShip );
                DrawGate( &gates[0] ); 

            EndMode2D();  
            DrawText(TextFormat("%000000009.0f M", distance), 450, 96, 36, WHITE);
            DrawText(TextFormat("SCORE: %d00", score), 450, 150, 36, WHITE);
            DrawText(TextFormat("Collision: %s", collision ? "true" : "false"), 10, 10, 24, collision ? RED : WHITE);

        EndDrawing();
    }

    // De-Initialization ------------------------------------------------------------------------------------//
    CloseWindow();        // Close window and OpenGL context
    return 0;
}

