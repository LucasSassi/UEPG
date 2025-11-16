#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include <math.h>

// Estrutura do Jogador
typedef struct {
    float x;
    float y;
    float width;
    float height;
    float speed;
    int score;
} Player;

// Estrutura dos Itens a Coletar
typedef struct {
    float x;
    float y;
    float radius;
    int active;
} Item;

// Estrutura dos Inimigos
typedef struct {
    float x;
    float y;
    float width;
    float height;
    float speed;
} Enemy;

#define MAX_ITEMS 10
#define MAX_ENEMIES 3
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 600

int main() {
    // Inicializar janela
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "JOGO - Raylib");
    SetTargetFPS(60);
    
    // Inicializar jogador
    Player player = {
        .x = SCREEN_WIDTH / 2,
        .y = SCREEN_HEIGHT - 50,
        .width = 30,
        .height = 30,
        .speed = 5.0f,
        .score = 0
    };
    
    // Inicializar itens a coletar
    Item items[MAX_ITEMS];
    for (int i = 0; i < MAX_ITEMS; i++) {
        items[i].x = rand() % (SCREEN_WIDTH - 30) + 15;
        items[i].y = rand() % (SCREEN_HEIGHT / 2) + 15;
        items[i].radius = 8;
        items[i].active = 1;
    }
    
    // Inicializar inimigos
    Enemy enemies[MAX_ENEMIES];
    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies[i].x = rand() % (SCREEN_WIDTH - 50) + 25;
        enemies[i].y = rand() % (SCREEN_HEIGHT / 2) + 50;
        enemies[i].width = 30;
        enemies[i].height = 30;
        enemies[i].speed = 2.0f + (i * 0.5f);
    }
    
    int gameOver = 0;
    
    // Loop principal do jogo
    while (!WindowShouldClose() && !gameOver) {
        // ===== ENTRADA =====
        if (IsKeyDown(KEY_LEFT) && player.x > 0) {
            player.x -= player.speed;
        }
        if (IsKeyDown(KEY_RIGHT) && player.x < SCREEN_WIDTH - player.width) {
            player.x += player.speed;
        }
        if (IsKeyDown(KEY_UP) && player.y > 0) {
            player.y -= player.speed;
        }
        if (IsKeyDown(KEY_DOWN) && player.y < SCREEN_HEIGHT - player.height) {
            player.y += player.speed;
        }
        
        // ===== LÓGICA =====
        
        // Movimento dos inimigos
        for (int i = 0; i < MAX_ENEMIES; i++) {
            enemies[i].x += enemies[i].speed;
            
            // Inimigos rebatendo nas laterais
            if (enemies[i].x <= 0 || enemies[i].x >= SCREEN_WIDTH - enemies[i].width) {
                enemies[i].speed *= -1;
            }
        }
        
        // Colisão do jogador com itens
        for (int i = 0; i < MAX_ITEMS; i++) {
            if (items[i].active) {
                float dx = player.x + player.width/2 - items[i].x;
                float dy = player.y + player.height/2 - items[i].y;
                float distance = sqrt(dx*dx + dy*dy);
                
                if (distance < player.width/2 + items[i].radius) {
                    items[i].active = 0;
                    player.score += 10;
                    
                    // Reaparecer item em novo local
                    items[i].x = rand() % (SCREEN_WIDTH - 30) + 15;
                    items[i].y = rand() % (SCREEN_HEIGHT / 2) + 15;
                    items[i].active = 1;
                }
            }
        }
        
        // Colisão do jogador com inimigos
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (player.x < enemies[i].x + enemies[i].width &&
                player.x + player.width > enemies[i].x &&
                player.y < enemies[i].y + enemies[i].height &&
                player.y + player.height > enemies[i].y) {
                gameOver = 1;
            }
        }
        
        // ===== DESENHO =====
        BeginDrawing();
        
        ClearBackground(RAYWHITE);
        
        // Desenhar fundo com grid
        for (int i = 0; i < SCREEN_WIDTH; i += 50) {
            DrawLine(i, 0, i, SCREEN_HEIGHT, LIGHTGRAY);
        }
        for (int i = 0; i < SCREEN_HEIGHT; i += 50) {
            DrawLine(0, i, SCREEN_WIDTH, i, LIGHTGRAY);
        }
        
        // Desenhar itens
        for (int i = 0; i < MAX_ITEMS; i++) {
            if (items[i].active) {
                DrawCircle((int)items[i].x, (int)items[i].y, items[i].radius, GOLD);
                DrawCircleLines((int)items[i].x, (int)items[i].y, items[i].radius, ORANGE);
            }
        }
        
        // Desenhar jogador
        DrawRectangle((int)player.x, (int)player.y, (int)player.width, (int)player.height, BLUE);
        DrawRectangleLines((int)player.x, (int)player.y, (int)player.width, (int)player.height, DARKBLUE);
        
        // Desenhar inimigos
        for (int i = 0; i < MAX_ENEMIES; i++) {
            DrawRectangle((int)enemies[i].x, (int)enemies[i].y, (int)enemies[i].width, (int)enemies[i].height, RED);
            DrawRectangleLines((int)enemies[i].x, (int)enemies[i].y, (int)enemies[i].width, (int)enemies[i].height, MAROON);
        }
        
        // Desenhar UI
        DrawText("JOGO RAYLIB", 20, 20, 30, BLUE);
        DrawText(TextFormat("Score: %d", player.score), 20, 60, 20, BLACK);
        DrawText("Use SETAS para mover | ESC para sair", 20, SCREEN_HEIGHT - 30, 15, DARKGRAY);
        
        // Desenhar título dos autores
        DrawText("LUCAS SASSI | EDUARDO PARIZE | LUCAS DE POZO | VINICIUS RIBAS", 
                 20, SCREEN_HEIGHT - 50, 10, GRAY);
        
        EndDrawing();
    }
    
    // Tela de Game Over
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        
        DrawText("GAME OVER!", SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 - 100, 60, RED);
        DrawText(TextFormat("Pontuacao Final: %d", player.score), 
                 SCREEN_WIDTH/2 - 180, SCREEN_HEIGHT/2, 30, WHITE);
        DrawText("Pressione ESC para sair", SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/2 + 100, 20, YELLOW);
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}