#include "game.h"
#include <math.h>   //提供数学函数和宏定义
#include "audio.h"  // 音频接口

static float playerX = CELL_SIZE * 1.5f;
static float playerY = CELL_SIZE * 1.5f;
static float playerSpeed = 4.5f;//移动速度
static float playerRadius = CELL_SIZE * 0.25f;//角色半径
static int playerHealth = 100;//生命值
static int playerScore = 0;//分数

static bool CheckCollisionWithWalls(float newX, float newY) {
    int gridX = (int)(newX / CELL_SIZE);
    int gridY = (int)(newY / CELL_SIZE);   
    return !Map_IsWalkable(gridX, gridY);
}//定义检测函数，用于检测玩家是否可移动，防止进入墙壁（！反转ture，false）

void Player_Init(void) {
    playerX = CELL_SIZE * 1.5f;
    playerY = CELL_SIZE * 1.5f;
    playerHealth = 100;
    playerScore = 0;
    printf("[玩家] 初始化完成\n");
}//初始化角色

void Player_Update(void) {
    float moveX = 0, moveY = 0;
    if (IsKeyDown(KEY_W)) moveY = -1;
    if (IsKeyDown(KEY_S)) moveY = 1;
    if (IsKeyDown(KEY_A)) moveX = -1;
    if (IsKeyDown(KEY_D)) moveX = 1;//通过wasd操控角色移动
    
    if (moveX != 0 || moveY != 0) {
        float length = sqrtf(moveX*moveX + moveY*moveY);
        moveX /= length;
        moveY /= length;
        
        float newX = playerX + moveX * playerSpeed;
        float newY = playerY + moveY * playerSpeed;//计算角色移动后位置
        
        if (!CheckCollisionWithWalls(newX, newY)) {
            playerX = newX;
            playerY = newY;
        }//如果角色不会撞墙就移动到新位置
    }
}

void Player_Draw(void) {
    DrawCircle((int)playerX, (int)playerY, playerRadius, RED);
    DrawCircle((int)playerX, (int)playerY, playerRadius-2, (Color){255,100,100,255});
}//角色绘制

int Player_GetHealth(void) { return playerHealth; }
int Player_GetScore(void) { return playerScore; }
void Player_GetPosition(int* outGridX, int* outGridY) {
    *outGridX = (int)(playerX / CELL_SIZE);
    *outGridY = (int)(playerY / CELL_SIZE);
}
void Player_TakeDamage(int damage) {
    playerHealth -= damage; 
    if(playerHealth<0) playerHealth=0; 
    Audio_PlaySound(SFX_DAMAGE); } // 受击时播放音效 
void Player_AddScore(int points) {
    playerScore += points;
    Audio_PlaySound(SFX_SCORE);  // 得分时播放音效
}      
