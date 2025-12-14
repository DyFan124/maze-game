#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//陷阱系统模块 - 负责迷宫中陷阱的生成、更新、碰撞检测、绘制等核心逻辑
// 核心功能：
//1. 随机生成陷阱位置（避开边界、障碍物、出口）
//2. 陷阱计时器管理
//3. 检测玩家踩中陷阱并扣除生命值
//4. 绘制陷阱图形

 // 陷阱系统核心常量定义
#define MAX_TRAPS      10          // 迷宫中最大陷阱数量
#define TRAP_DURATION  8.0f        // 单个陷阱激活状态持续时间（秒） 
#define TRAP_RESPAWN_DELAY 5.0f    // 陷阱失效后重生延迟时间（秒）
#define TRAP_DAMAGE    20          // 玩家踩中陷阱受到的伤害值

// 陷阱结构体：存储单个陷阱的状态和属性
typedef struct {
    int x, y;                      // 陷阱在迷宫网格中的坐标（非像素坐标）
    bool active;                   // 陷阱是否处于激活状态（可触发）
    float timer;                   // 陷阱激活状态剩余持续时间
    float respawnTimer;            // 陷阱重生倒计时（失效后开始计时）
} Trap;

// 全局陷阱数组 --存储所有陷阱的状态
static Trap traps[MAX_TRAPS];

//函数CanPlaceTrap
//检查指定网格坐标是否可以放置陷阱
static bool CanPlaceTrap(int x, int y) {
    if (x < 1 || x >= MAZE_WIDTH - 1 || y < 1 || y >= MAZE_HEIGHT - 1 ||
        !Map_IsWalkable(x, y) || Map_GetTile(x, y) == TILE_GOAL) {
        return false;
    }

    // 重复检查：检查该位置是否已有激活的陷阱
    for (int i = 0; i < MAX_TRAPS; i++) {
        if (traps[i].active && traps[i].x == x && traps[i].y == y) {
            return false;
        }
    }

    return true;
}
//FindTrapPosition
//随机寻找一个合法的陷阱放置位置
static void FindTrapPosition(int* outX, int* outY) {
    // 最多尝试100次找合法位置
    for (int attempts = 0; attempts < 100; attempts++) {
        int x = 1 + rand() % (MAZE_WIDTH - 2);
        int y = 1 + rand() % (MAZE_HEIGHT - 2);

        // 检查该位置是否合法，合法则赋值并返回
        if (CanPlaceTrap(x, y)) {
            *outX = x;
            *outY = y;
            return;
        }
    }

    //极端情况： 保底返回迷宫中心
    *outX = MAZE_WIDTH / 2;
    *outY = MAZE_HEIGHT / 2;
}
//初始化陷阱系统
void Trap_Init(void) {
    // 设置随机数种子，基于当前时间生成随机数
    srand((unsigned int)time(NULL));

    // 遍历初始化所有陷阱
    for (int i = 0; i < MAX_TRAPS; i++) {
        traps[i].active = false;                // 初始为非激活状态
        traps[i].timer = 0;                     // 激活计时器清零
        // 随机0~3秒的初始重生延迟，避免所有陷阱同时生成
        traps[i].respawnTimer = (float)(rand() % 3);
        // 为陷阱分配初始位置
        FindTrapPosition(&traps[i].x, &traps[i].y);
    }

    printf("[陷阱] 初始化完成\n");  // 初始化日志
}
//每帧更新陷阱系统状态
void Trap_Update(void) {
    // 获取当前帧的持续时间（秒），用于时间相关计算
    float deltaTime = GetFrameTime();

    // 获取玩家当前的网格坐标
    int playerX, playerY;
    Player_GetPosition(&playerX, &playerY);

    // 遍历更新每个陷阱的状态
    for (int i = 0; i < MAX_TRAPS; i++) {
        if (traps[i].active) {  // 陷阱处于激活状态
            // 1. 更新陷阱激活剩余时间
            traps[i].timer -= deltaTime;

            // 2. 检测玩家是否踩中陷阱（坐标重合）
            if (playerX == traps[i].x && playerY == traps[i].y) {
                Player_TakeDamage(TRAP_DAMAGE);          // 玩家受到伤害
                traps[i].active = false;                 // 陷阱失效
                traps[i].respawnTimer = TRAP_RESPAWN_DELAY;  // 开始重生计时
                // 打印伤害日志
                printf("踩中陷阱！-%dHP (剩余:%d)\n", TRAP_DAMAGE, Player_GetHealth());
            }
            // 3. 陷阱激活时间结束，自动失效
            else if (traps[i].timer <= 0) {
                traps[i].active = false;                 // 陷阱失效
                traps[i].respawnTimer = TRAP_RESPAWN_DELAY;  // 开始重生计时
            }
        }
        else {  // 陷阱处于非激活状态
            // 1. 更新重生倒计时
            traps[i].respawnTimer -= deltaTime;
            // 2. 重生计时结束，重新生成陷阱
            if (traps[i].respawnTimer <= 0) {
                FindTrapPosition(&traps[i].x, &traps[i].y);  // 重新找新位置
                traps[i].active = true;                      // 激活陷阱
                traps[i].timer = TRAP_DURATION;              // 重置激活持续时间
            }
        }
    }
}
//绘制所有激活状态的陷阱
void Trap_Draw(void) {
    Color trapColor = { 180, 0, 0, 255 };    // 陷阱主体颜色（深红色）
    Color borderColor = { 255, 50, 50, 255 }; // 陷阱边框颜色（亮红色）

    // 遍历绘制每个激活的陷阱
    for (int i = 0; i < MAX_TRAPS; i++) {
        if (!traps[i].active) continue;  // 跳过非激活状态的陷阱

        // 将网格坐标转换为像素坐标（CELL_SIZE为每个网格的像素大小）
        int px = traps[i].x * CELL_SIZE;
        int py = traps[i].y * CELL_SIZE;

        // 绘制陷阱主体
        DrawRectangle(px + 3, py + 3, CELL_SIZE - 6, CELL_SIZE - 6, trapColor);

        // 绘制陷阱边框
        DrawRectangleLines(px, py, CELL_SIZE, CELL_SIZE, borderColor);

        // 绘制白色"X"标记
        DrawLine(px + 6, py + 6, px + CELL_SIZE - 6, py + CELL_SIZE - 6, WHITE);
        DrawLine(px + CELL_SIZE - 6, py + 6, px + 6, py + CELL_SIZE - 6, WHITE);
    }
}