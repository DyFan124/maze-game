// player.c - 玩家系统（迷宫游戏专用）
#include "game.h"
#include <stdio.h>  // 用于printf调试

// ===== 私有数据 =====
static int playerGridX = 1;      // 玩家所在的列（网格坐标）
static int playerGridY = 1;      // 玩家所在的行（网格坐标）
static int playerHealth = 100;   // 生命值
static int playerScore = 0;      // 积分

// ===== 接口函数实现 =====

void Player_Init(void) {
    printf("[玩家系统] 初始化\n");
    
    // 重置玩家状态
    playerGridX = 1;           // 起始位置(1,1) - 确保不是墙
    playerGridY = 1;
    playerHealth = 100;
    playerScore = 0;
    
    printf("  起始位置: (%d, %d)\n", playerGridX, playerGridY);
    printf("  生命值: %d, 积分: %d\n", playerHealth, playerScore);
}

void Player_Update(void) {
    // 记录目标位置
    int targetX = playerGridX;
    int targetY = playerGridY;
    
    // 1. 获取键盘输入（WASD或方向键）
    if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))    targetY--;
    if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))  targetY++;
    if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))  targetX--;
    if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) targetX++;
    
    // 如果没有按键，直接返回
    if (targetX == playerGridX && targetY == playerGridY) {
        return;
    }
    
    // 2. 检查移动是否合法（调用地图系统的接口）
    if (Map_IsWalkable(targetX, targetY)) {
        // 可以移动
        playerGridX = targetX;
        playerGridY = targetY;
        
        // 调试输出
        printf("玩家移动到: (%d, %d)\n", playerGridX, playerGridY);
        
        // 3. 检查当前格子效果（这部分由总指挥在game.c中处理）
        // 比如：如果是道具就加分，如果是陷阱就扣血
    } else {
        // 撞墙了
        printf("玩家撞墙！目标(%d,%d)不可通行\n", targetX, targetY);
    }
}

void Player_Draw(void) {
    // 将网格坐标转换为像素坐标
    int pixelX = playerGridX * CELL_SIZE;
    int pixelY = playerGridY * CELL_SIZE;
    
    // 绘制玩家（红色方块）
    DrawRectangle(pixelX, pixelY, CELL_SIZE, CELL_SIZE, RED);
    
    // 在方块中心画个标记（便于观察）
    DrawRectangle(pixelX + CELL_SIZE/2 - 2, 
                  pixelY + CELL_SIZE/2 - 2, 
                  4, 4, WHITE);
                  
    // 可选：绘制玩家朝向指示器
    // 暂时不需要，因为迷宫是网格移动
}

void Player_GetPosition(int* outGridX, int* outGridY) {
    // 安全地返回玩家当前的网格坐标
    *outGridX = playerGridX;
    *outGridY = playerGridY;
}

int Player_GetHealth(void) {
    return playerHealth;
}

// ===== 其他辅助函数（根据Game.h中的声明）=====

int Player_GetScore(void) {
    return playerScore;
}

void Player_TakeDamage(int damage) {
    playerHealth -= damage;
    if (playerHealth < 0) playerHealth = 0;
    printf("玩家受到 %d 点伤害，剩余生命: %d\n", damage, playerHealth);
}

void Player_AddScore(int points) {
    playerScore += points;
    printf("玩家获得 %d 分，总分: %d\n", points, playerScore);
}