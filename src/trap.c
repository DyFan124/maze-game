#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// 陷阱常量
#define MAX_TRAPS 10
#define TRAP_DURATION 8.0f
#define TRAP_RESPAWN_DELAY 5.0f
#define TRAP_DAMAGE 20

// 陷阱结构
typedef struct {
    int x, y;           // 网格坐标
    bool active;        // 是否激活
    float timer;        // 剩余时间
    float respawnTimer; // 重生计时器
} Trap;

// 全局变量
static Trap traps[MAX_TRAPS];

// 检查位置是否可以放陷阱
static bool CanPlaceTrap(int x, int y) {
    // 边界检查
    if (x < 1 || x >= MAZE_WIDTH-1 || y < 1 || y >= MAZE_HEIGHT-1) {
        return false;
    }
    
    // 必须是可通行的地面
    if (!Map_IsWalkable(x, y)) {
        return false;
    }
    
    // 不能是出口
    if (Map_GetTile(x, y) == TILE_GOAL) {
        return false;
    }
    
    // 检查是否已有陷阱
    for (int i = 0; i < MAX_TRAPS; i++) {
        if (traps[i].active && traps[i].x == x && traps[i].y == y) {
            return false;
        }
    }
    
    return true;
}

// 生成陷阱位置
static void FindTrapPosition(int* outX, int* outY) {
    int attempts = 0;
    while (attempts < 100) {
        int x = 1 + rand() % (MAZE_WIDTH - 2);
        int y = 1 + rand() % (MAZE_HEIGHT - 2);
        
        if (CanPlaceTrap(x, y)) {
            *outX = x;
            *outY = y;
            return;
        }
        attempts++;
    }
    
    // 找不到合适位置，返回中心附近
    *outX = MAZE_WIDTH / 2;
    *outY = MAZE_HEIGHT / 2;
}

// 初始化陷阱系统
void Trap_Init(void) {
    srand((unsigned int)time(NULL));
    
    for (int i = 0; i < MAX_TRAPS; i++) {
        traps[i].active = false;
        traps[i].timer = 0;
        traps[i].respawnTimer = (float)(rand() % 3); // 随机延迟生成
        FindTrapPosition(&traps[i].x, &traps[i].y);
    }
    
    printf("[陷阱] 初始化完成\n");
}

// 更新陷阱
void Trap_Update(void) {
    float deltaTime = GetFrameTime();
    
    // 获取玩家位置
    int playerX, playerY;
    Player_GetPosition(&playerX, &playerY);
    
    for (int i = 0; i < MAX_TRAPS; i++) {
        if (traps[i].active) {
            // 更新计时器
            traps[i].timer -= deltaTime;
            
            // 检测玩家是否踩中
            if (playerX == traps[i].x && playerY == traps[i].y) {
                Player_TakeDamage(TRAP_DAMAGE);
                traps[i].active = false;
                traps[i].respawnTimer = TRAP_RESPAWN_DELAY;
                printf("踩中陷阱！-%dHP (剩余:%d)\n", TRAP_DAMAGE, Player_GetHealth());
            }
            // 陷阱时间到自动消失
            else if (traps[i].timer <= 0) {
                traps[i].active = false;
                traps[i].respawnTimer = TRAP_RESPAWN_DELAY;
            }
        } else {
            // 重生计时
            traps[i].respawnTimer -= deltaTime;
            if (traps[i].respawnTimer <= 0) {
                // 重新找位置（避免在原地重生）
                FindTrapPosition(&traps[i].x, &traps[i].y);
                traps[i].active = true;
                traps[i].timer = TRAP_DURATION;
            }
        }
    }
}

// 绘制陷阱
void Trap_Draw(void) {
    Color trapColor = {180, 0, 0, 255};  // 深红色
    Color borderColor = {255, 50, 50, 255};  // 亮红色边框
    
    for (int i = 0; i < MAX_TRAPS; i++) {
        if (!traps[i].active) continue;
        
        int px = traps[i].x * CELL_SIZE;
        int py = traps[i].y * CELL_SIZE;
        
        // 陷阱主体
        DrawRectangle(px + 3, py + 3, CELL_SIZE - 6, CELL_SIZE - 6, trapColor);
        
        // 边框
        DrawRectangleLines(px, py, CELL_SIZE, CELL_SIZE, borderColor);
        
        // 画个"X"表示危险
        DrawLine(px + 6, py + 6, 
                px + CELL_SIZE - 6, py + CELL_SIZE - 6, WHITE);
        DrawLine(px + CELL_SIZE - 6, py + 6, 
                px + 6, py + CELL_SIZE - 6, WHITE);
    }
}
