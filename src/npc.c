#include <stdlib.h>
#include <string.h>
#include <stdio.h>  
#include "game.h"

// 常量定义
#define MAX_NPC 10
#define NPC_MOVE_INTERVAL 0.8f  // NPC每0.8秒移动一次
#define NPC_DAMAGE 30           // NPC对玩家的伤害

// NPC结构体
typedef struct {
    int x, y;           // 网格坐标
    int dir;            // 方向：0上，1右，2下，3左
    bool active;        // 是否激活
    float moveTimer;    // 移动计时器
} NPC;

// 全局变量
static NPC npcs[MAX_NPC];
static int npcCount = 0;
static int playerGridX = 0, playerGridY = 0;  // 玩家位置

// 方向偏移量
static const int dirX[4] = {0, 1, 0, -1};
static const int dirY[4] = {-1, 0, 1, 0};

// 检查位置是否可通行
static bool IsWalkable(int x, int y) {
    return Map_IsWalkable(x, y);  // 直接使用地图系统的函数
}

// 获取可移动的方向列表
static int GetValidDirections(int x, int y, int validDirs[4]) {
    int count = 0;
    for (int d = 0; d < 4; d++) {
        if (IsWalkable(x + dirX[d], y + dirY[d])) {
            validDirs[count++] = d;
        }
    }
    return count;
}

// NPC初始化
void NPC_Init(void) {
    npcCount = 3;  // 固定3个NPC
    
    for (int i = 0; i < npcCount; i++) {
        // 在地图空地上生成NPC
        do {
            npcs[i].x = rand() % MAZE_WIDTH;
            npcs[i].y = rand() % MAZE_HEIGHT;
        } while (!IsWalkable(npcs[i].x, npcs[i].y));
        
        npcs[i].dir = rand() % 4;
        npcs[i].active = true;
        npcs[i].moveTimer = 0;
    }
    
    printf("[NPC] 初始化 %d 个NPC\n", npcCount);
}

// NPC更新（每帧调用）
void NPC_Update(void) {
    float deltaTime = GetFrameTime();  // 获取帧时间
    
    // 获取玩家位置
    Player_GetPosition(&playerGridX, &playerGridY);
    
    // 更新每个NPC
    for (int i = 0; i < npcCount; i++) {
        if (!npcs[i].active) continue;
        
        // 移动计时
        npcs[i].moveTimer += deltaTime;
        if (npcs[i].moveTimer < NPC_MOVE_INTERVAL) {
            continue;  // 还没到移动时间
        }
        
        npcs[i].moveTimer = 0;  // 重置计时器
        
        // 智能移动：尝试向玩家方向移动，否则随机移动
        int validDirs[4];
        int validCount = GetValidDirections(npcs[i].x, npcs[i].y, validDirs);
        
        if (validCount > 0) {
            // 如果有多个方向可选，优先选择朝向玩家的方向
            int bestDir = validDirs[0];
            int bestScore = -999;
            
            for (int j = 0; j < validCount; j++) {
                int dir = validDirs[j];
                int nextX = npcs[i].x + dirX[dir];
                int nextY = npcs[i].y + dirY[dir];
                
                // 计算方向得分（离玩家越近越好）
                int distX = abs(playerGridX - nextX);
                int distY = abs(playerGridY - nextY);
                int score = -(distX + distY);  // 负距离，越小越好
                
                if (score > bestScore) {
                    bestScore = score;
                    bestDir = dir;
                }
            }
            
            npcs[i].dir = bestDir;
            
            // 移动到新位置
            npcs[i].x += dirX[npcs[i].dir];
            npcs[i].y += dirY[npcs[i].dir];
        }
        
        // 碰撞检测：NPC碰到玩家
        if (npcs[i].x == playerGridX && npcs[i].y == playerGridY) {
            Player_TakeDamage(NPC_DAMAGE);
            printf("玩家被NPC%d攻击，-%dHP\n", i, NPC_DAMAGE);
            
            // NPC撞到玩家后后退一步
            npcs[i].x -= dirX[npcs[i].dir];
            npcs[i].y -= dirY[npcs[i].dir];
        }
    }
}

// 绘制所有NPC
void NPC_Draw(void) {
    for (int i = 0; i < npcCount; i++) {
        if (!npcs[i].active) continue;
        
        // 计算屏幕坐标
        int px = npcs[i].x * CELL_SIZE;
        int py = npcs[i].y * CELL_SIZE;
        
        // 绘制NPC身体（蓝色方块）
        DrawRectangle(px + 2, py + 2, CELL_SIZE - 4, CELL_SIZE - 4, BLUE);
        
        // 绘制NPC边框
        DrawRectangleLines(px, py, CELL_SIZE, CELL_SIZE, DARKBLUE);
        
        // 根据方向绘制眼睛
        int eyeSize = CELL_SIZE / 8;
        switch(npcs[i].dir) {
            case 0: // 上
                DrawRectangle(px + CELL_SIZE/2 - eyeSize*2, py + 4, 
                             eyeSize*4, eyeSize*2, WHITE);
                break;
            case 1: // 右
                DrawRectangle(px + CELL_SIZE - 8, py + CELL_SIZE/2 - eyeSize*2,
                             eyeSize*2, eyeSize*4, WHITE);
                break;
            case 2: // 下
                DrawRectangle(px + CELL_SIZE/2 - eyeSize*2, py + CELL_SIZE - 8,
                             eyeSize*4, eyeSize*2, WHITE);
                break;
            case 3: // 左
                DrawRectangle(px + 4, py + CELL_SIZE/2 - eyeSize*2,
                             eyeSize*2, eyeSize*4, WHITE);
                break;
        }
    }
}

// 获取NPC数量
int NPC_GetCount(void) {
    return npcCount;
}

// 获取指定NPC的位置
void NPC_GetPosition(int idx, int* x, int* y) {
    if (idx < 0 || idx >= npcCount) {
        *x = *y = -1;
        return;
    }
    *x = npcs[idx].x;
    *y = npcs[idx].y;
}

// 空函数（为了适配接口）
void NPC_SetExternalData(int (*maze)[MAZE_WIDTH], int mazeWidth, int mazeHeight, void* playerPtr) {
    // 不需要地图数据，直接使用Map_IsWalkable函数
}
