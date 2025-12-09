// player.c - 最简单图片版
#include "game.h"
#include <math.h>

// ===== 私有数据 =====
static float playerX = CELL_SIZE * 1.5f;
static float playerY = CELL_SIZE * 1.5f;
static float playerSpeed = 4.5f;
static int playerHealth = 100;
static int playerScore = 0;

// 新增：图片纹理
static Texture2D playerTexture;

// ===== 辅助函数 =====
static bool CheckCollisionWithWalls(float newX, float newY) {
    int gridX = (int)(newX / CELL_SIZE);
    int gridY = (int)(newY / CELL_SIZE);
    return !Map_IsWalkable(gridX, gridY);
}

// ===== 接口函数 =====
void Player_Init(void) {
    playerX = CELL_SIZE * 1.5f;
    playerY = CELL_SIZE * 1.5f;
    playerHealth = 100;
    playerScore = 0;
    
    // 关键：加载图片
    playerTexture = LoadTexture("player.png");
    printf("[玩家] 图片加载完成\n");
}

void Player_Update(void) {
    float moveX = 0, moveY = 0;
    
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) moveY = -1;
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) moveY = 1;
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) moveX = -1;
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) moveX = 1;
    
    if (moveX != 0 || moveY != 0) {
        // 对角线速度调整
        if (moveX != 0 && moveY != 0) {
            moveX *= 0.7f;
            moveY *= 0.7f;
        }
        
        float newX = playerX + moveX * playerSpeed;
        float newY = playerY + moveY * playerSpeed;
        
        if (!CheckCollisionWithWalls(newX, newY)) {
            playerX = newX;
            playerY = newY;
        }
    }
}

void Player_Draw(void) {
    // 关键：用图片绘制，图片居中
    DrawTexture(playerTexture, 
               (int)playerX - playerTexture.width/2,  // X居中
               (int)playerY - playerTexture.height/2, // Y居中
               WHITE);
    
    // 可选：调试时显示碰撞框
    // DrawCircleLines((int)playerX, (int)playerY, 15, RED);
}

// 下面的函数都不需要改
int Player_GetHealth(void) { return playerHealth; }
int Player_GetScore(void) { return playerScore; }
void Player_GetPosition(int* outGridX, int* outGridY) {
    *outGridX = (int)(playerX / CELL_SIZE);
    *outGridY = (int)(playerY / CELL_SIZE);
}
void Player_TakeDamage(int damage) { 
    playerHealth -= damage; 
    if(playerHealth < 0) playerHealth = 0; 
}
void Player_AddScore(int points) { playerScore += points; }