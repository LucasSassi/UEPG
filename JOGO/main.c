#include "raylib.h"
#include <stdlib.h> // Para rand()

// ----------------------------------------------------------------------------------
// Definições do Jogo
// ----------------------------------------------------------------------------------
#define SCREEN_WIDTH 450
#define SCREEN_HEIGHT 800

// === TAMANHO DO PÁSSARO NA TELA ===
// Defina aqui o tamanho que você quer que o pássaro tenha na tela,
// independentemente do tamanho da imagem original.
#define BIRD_DRAW_WIDTH 44
#define BIRD_DRAW_HEIGHT 36
// =================================

#define GRAVITY 0.6f
#define JUMP_FORCE 12.0f

#define PIPE_SPEED 3.0f
#define PIPE_WIDTH 80
#define PIPE_GAP 200 // Espaço entre os canos

// Define quantos "pares" de canos teremos na tela ao mesmo tempo
#define MAX_PIPES 4

// ----------------------------------------------------------------------------------
// Estruturas
// ----------------------------------------------------------------------------------
typedef struct Bird
{
    Rectangle rect; // O retângulo de colisão e posição
    float velocityY;
} Bird;

typedef struct Pipe
{
    Rectangle topRect;
    Rectangle bottomRect;
    bool active; // O cano está na tela?
    bool scored; // Já passamos por este cano?
} Pipe;

// ----------------------------------------------------------------------------------
// Função Principal
// ----------------------------------------------------------------------------------
int main(void)
{
    // --- 1. Inicialização ---
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Meu Flappy Bird");
    SetTargetFPS(60);

    // --- Inicializa o Pássaro ---
    Bird bird = {0};
    bird.rect.width = BIRD_DRAW_WIDTH;   // Usa o tamanho de desenho definido
    bird.rect.height = BIRD_DRAW_HEIGHT; // Usa o tamanho de desenho definido
    bird.rect.x = SCREEN_WIDTH / 4.0f;
    bird.rect.y = SCREEN_HEIGHT / 2.0f - bird.rect.height / 2.0f;
    bird.velocityY = 0;

    // --- Inicializa os Canos ---
    Pipe pipes[MAX_PIPES] = {0};
    for (int i = 0; i < MAX_PIPES; i++)
    {
        float startX = SCREEN_WIDTH + (i * (SCREEN_WIDTH / 2.0f));
        int gapY = GetRandomValue(PIPE_GAP, SCREEN_HEIGHT - PIPE_GAP);

        pipes[i].topRect.width = PIPE_WIDTH;
        pipes[i].topRect.height = gapY - (PIPE_GAP / 2.0f);
        pipes[i].topRect.x = startX;
        pipes[i].topRect.y = 0;

        pipes[i].bottomRect.width = PIPE_WIDTH;
        pipes[i].bottomRect.height = SCREEN_HEIGHT - gapY - (PIPE_GAP / 2.0f);
        pipes[i].bottomRect.x = startX;
        pipes[i].bottomRect.y = gapY + (PIPE_GAP / 2.0f);

        pipes[i].active = true;
        pipes[i].scored = false;
    }

    // --- Estado do Jogo ---
    bool gameOver = false;
    int score = 0;

    Texture2D birdTexture = LoadTexture("passaro.png");

    // --- 2. Loop Principal do Jogo ---
    while (!WindowShouldClose())
    {
        // --- 3. Atualização (Lógica) ---
        if (!gameOver)
        {
            // --- Lógica do Pássaro ---
            if (IsKeyPressed(KEY_SPACE))
            {
                bird.velocityY = -JUMP_FORCE;
            }
            bird.velocityY += GRAVITY;
            bird.rect.y += bird.velocityY;

            // --- Lógica dos Canos ---
            for (int i = 0; i < MAX_PIPES; i++)
            {
                if (pipes[i].active)
                {
                    pipes[i].topRect.x -= PIPE_SPEED;
                    pipes[i].bottomRect.x -= PIPE_SPEED;

                    // Recicla o cano
                    if (pipes[i].topRect.x < -PIPE_WIDTH)
                    {
                        float nextX = pipes[i].topRect.x + (MAX_PIPES * (SCREEN_WIDTH / 2.0f));
                        int gapY = GetRandomValue(PIPE_GAP, SCREEN_HEIGHT - PIPE_GAP);

                        pipes[i].topRect.x = nextX;
                        pipes[i].topRect.height = gapY - (PIPE_GAP / 2.0f);

                        pipes[i].bottomRect.x = nextX;
                        pipes[i].bottomRect.y = gapY + (PIPE_GAP / 2.0f);
                        pipes[i].bottomRect.height = SCREEN_HEIGHT - gapY - (PIPE_GAP / 2.0f);

                        pipes[i].scored = false;
                    }

                    // Pontuação
                    if (!pipes[i].scored && pipes[i].topRect.x < bird.rect.x)
                    {
                        score++;
                        pipes[i].scored = true;
                    }
                }
            }

            // --- Lógica de Colisão ---
            // Chão ou teto
            if (bird.rect.y > (SCREEN_HEIGHT - bird.rect.height) || bird.rect.y < 0)
            {
                gameOver = true;
            }

            // Canos
            for (int i = 0; i < MAX_PIPES; i++)
            {
                if (CheckCollisionRecs(bird.rect, pipes[i].topRect) || CheckCollisionRecs(bird.rect, pipes[i].bottomRect))
                {
                    gameOver = true;
                }
            }
        }
        else // Game Over
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                // Reset Jogo
                gameOver = false;
                score = 0;

                bird.rect.y = SCREEN_HEIGHT / 2.0f - bird.rect.height / 2.0f;
                bird.velocityY = 0;

                for (int i = 0; i < MAX_PIPES; i++)
                {
                    float startX = SCREEN_WIDTH + (i * (SCREEN_WIDTH / 2.0f));
                    int gapY = GetRandomValue(PIPE_GAP, SCREEN_HEIGHT - PIPE_GAP);

                    pipes[i].topRect.x = startX;
                    pipes[i].topRect.y = 0;
                    pipes[i].topRect.height = gapY - (PIPE_GAP / 2.0f);

                    pipes[i].bottomRect.x = startX;
                    pipes[i].bottomRect.y = gapY + (PIPE_GAP / 2.0f);
                    pipes[i].bottomRect.height = SCREEN_HEIGHT - gapY - (PIPE_GAP / 2.0f);

                    pipes[i].active = true;
                    pipes[i].scored = false;
                }
            }
        }

        // --- 4. Desenho (Renderização) ---
        BeginDrawing();
        ClearBackground(SKYBLUE);

        // Desenha os Canos
        for (int i = 0; i < MAX_PIPES; i++)
        {
            if (pipes[i].active)
            {
                DrawRectangleRec(pipes[i].topRect, DARKGREEN);
                DrawRectangleRec(pipes[i].bottomRect, DARKGREEN);
            }
        }

        // ==============================================================================
        DrawTexturePro(birdTexture,
                       (Rectangle){0, 0, (float)birdTexture.width, (float)birdTexture.height}, // Retângulo de Origem (imagem inteira)
                       bird.rect,                                                              // Retângulo de Destino (onde e qual tamanho desenhar)
                       (Vector2){0, 0},                                                        // Ponto de Rotação (pivô)
                       0.0f,                                                                   // Rotação em graus
                       WHITE);                                                                 // Cor (WHITE = cor original)
        //
        // ==============================================================================

        // Desenha o Score
        DrawText(TextFormat("%d", score), SCREEN_WIDTH / 2 - 20, 50, 40, WHITE);

        // Tela de Game Over
        if (gameOver)
        {
            DrawText("GAME OVER", SCREEN_WIDTH / 2 - MeasureText("GAME OVER", 40) / 2, SCREEN_HEIGHT / 2 - 40, 40, RED);
            DrawText("Pressione [ENTER] para recomeçar", SCREEN_WIDTH / 2 - MeasureText("Pressione [ENTER] para recomeçar", 20) / 2, SCREEN_HEIGHT / 2 + 10, 20, GRAY);
        }

        EndDrawing();
    }

    // --- 5. Limpeza ---
    //
    UnloadTexture(birdTexture);

    CloseWindow();
    return 0;
}