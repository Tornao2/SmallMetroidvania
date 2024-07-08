#include "raylib.h"

int main(void)
{
    const int screenWidth = 1024;
    const int screenHeight = 768;
    InitWindow(screenWidth, screenHeight, "MetroidCube");
    SetTargetFPS(60);    
    while (!WindowShouldClose())  
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }
    CloseWindow(); 
    return 0;
}