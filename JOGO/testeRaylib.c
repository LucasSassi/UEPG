#include "raylib.h"

int main(void)
{
    // --- Configuração ---
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Input de Teclado");

    // Posição inicial do quadrado
    Rectangle player = { screenWidth / 2.0f - 25, screenHeight / 2.0f - 25, 50, 50 };
    float moveSpeed = 5.0f;

    SetTargetFPS(60);

    // --- Loop Principal ---
    while (!WindowShouldClose())
    {
        // --- Atualização (Lógica) ---
        if (IsKeyDown(KEY_D))
        {
            player.x += moveSpeed;
        }
        if (IsKeyDown(KEY_A))
        {
            player.x -= moveSpeed;
        }
        if (IsKeyDown(KEY_W))
        {
            player.y -= moveSpeed;
        }
        if (IsKeyDown(KEY_S))
        {
            player.y += moveSpeed;
        }

        // --- Desenho (Renderização) ---
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawRectangleRec(player, RED); // Desenha o retângulo
            DrawText("Use A e D para mover", 10, 10, 20, WHITE);
        EndDrawing();
    }

    // --- Limpeza ---
    CloseWindow();
    return 0;
}