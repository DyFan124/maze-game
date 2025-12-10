// player.c - 完整版本
#include "game.h"
#include <math.h>
#include <stdio.h> 

static float playerX = CELL_SIZE * 1.5f;
static float playerY = CELL_SIZE * 1.5f;
static float playerSpeed = 4.5f;
static float playerRadius = CELL_SIZE * 0.25f;
static int playerHealth = 100;
static int playerScore = 0;

// 碰撞检测函数
static bool CheckCollisionWithWalls(float newX, float newY) {
    int gridX = (int)(newX / CELL_SIZE);
    int gridY = (int)(newY / CELL_SIZE);
    
    // 简单检查玩家当前所在的格子
    return !Map_IsWalkable(gridX, gridY);
}

void Player_Init(void) {
    playerX = CELL_SIZE * 1.5f;
    playerY = CELL_SIZE * 1.5f;
    playerHealth = 100;
    playerScore = 0;
    printf("[玩家] 初始化完成\n");
}

void Player_Update(void) {
    float moveX = 0, moveY = 0;
    
    if (IsKeyDown(KEY_W)) moveY = -1;
    if (IsKeyDown(KEY_S)) moveY = 1;
    if (IsKeyDown(KEY_A)) moveX = -1;
    if (IsKeyDown(KEY_D)) moveX = 1;
    
    // 有移动输入才处理
    if (moveX != 0 || moveY != 0) {
        // 标准化
        float length = sqrtf(moveX*moveX + moveY*moveY);
        moveX /= length;
        moveY /= length;
        
        float newX = playerX + moveX * playerSpeed;
        float newY = playerY + moveY * playerSpeed;
        
        if (!CheckCollisionWithWalls(newX, newY)) {
            playerX = newX;
            playerY = newY;
        }
    }
}

void Player_Draw(void) {
    DrawCircle((int)playerX, (int)playerY, playerRadius, RED);
    DrawCircle((int)playerX, (int)playerY, playerRadius-2, (Color){255,100,100,255});
}

int Player_GetHealth(void) { return playerHealth; }
int Player_GetScore(void) { return playerScore; }
void Player_GetPosition(int* outGridX, int* outGridY) {
    *outGridX = (int)(playerX / CELL_SIZE);
    *outGridY = (int)(playerY / CELL_SIZE);
}
void Player_TakeDamage(int damage) { playerHealth -= damage; if(playerHealth<0) playerHealth=0;Audio_PlaySound(SFX_DAMAGE); } // ← 新增：受击时播放音效
void Player_AddScore(int points) { playerScore += points; Audio_PlaySound(SFX_SCORE); } // ← 新增：得分时播放音效 

PlayerData* Player_GetData(void) {
    static PlayerData playerData;
    
    // 从现有数据填充
    int gridX, gridY;
    Player_GetPosition(&gridX, &gridY);
    
    playerData.gridX = gridX;
    playerData.gridY = gridY;
    playerData.health = Player_GetHealth();
    playerData.score = Player_GetScore();
    
    return &playerData;
}