#include <stdlib.h>
#include <stdio.h>
#include "game.h"

// NPC结构
typedef struct {
    int x, y;        // 网格坐标
    bool active;     // 是否激活
    float moveTimer; // 独立移动计时器
} NPC;

// 全局变量
static NPC npcs[3];
static int npcCount = 3;

// NPC初始化
void NPC_Init(void) {
    for (int i = 0; i < 3; i++) {
        // 在地图空地上生成NPC
        do {
            npcs[i].x = 5 + rand() % (MAZE_WIDTH - 10);
            npcs[i].y = 5 + rand() % (MAZE_HEIGHT - 10);
        } while (!Map_IsWalkable(npcs[i].x, npcs[i].y));
        
        npcs[i].active = true;
        npcs[i].moveTimer = (float)i * 0.2f;  // 错开移动时间
    }
    printf("[NPC] 初始化完成\n");
}

// NPC更新 - 完全随机移动
void NPC_Update(void) {
    float deltaTime = GetFrameTime();
    
    // 获取玩家位置
    int px, py;
    Player_GetPosition(&px, &py);
    
    // 更新每个NPC
    for (int i = 0; i < 3; i++) {
        if (!npcs[i].active) continue;
        
        // 每个NPC独立计时
        npcs[i].moveTimer += deltaTime;
        
        // 每0.3秒移动一次（更活跃）
        if (npcs[i].moveTimer < 0.3f) continue;
        npcs[i].moveTimer = 0;
        
        // 保存当前位置
        int oldX = npcs[i].x;
        int oldY = npcs[i].y;
        
        // 完全随机移动（4个方向）
        int dir = rand() % 4;
        int newX = oldX, newY = oldY;
        
        switch (dir) {
            case 0: newY--; break; // 上
            case 1: newX++; break; // 右
            case 2: newY++; break; // 下
            case 3: newX--; break; // 左
        }
        
        // 检查新位置是否可通行
        if (Map_IsWalkable(newX, newY)) {
            npcs[i].x = newX;
            npcs[i].y = newY;
        }
        
        // 碰撞检测
        if (npcs[i].x == px && npcs[i].y == py) {
            Player_TakeDamage(30);
            // NPC退回到原来位置
            npcs[i].x = oldX;
            npcs[i].y = oldY;
        }
    }
}

// 绘制NPC - 蓝色，无数字
void NPC_Draw(void) {
    for (int i = 0; i < 3; i++) {
        if (!npcs[i].active) continue;
        
        int screenX = npcs[i].x * CELL_SIZE;
        int screenY = npcs[i].y * CELL_SIZE;
        
        // 蓝色NPC，不要数字
        DrawRectangle(screenX, screenY, CELL_SIZE, CELL_SIZE, BLUE);
        DrawRectangleLines(screenX, screenY, CELL_SIZE, CELL_SIZE, DARKBLUE);
    }
}

// 获取NPC数量
int NPC_GetCount(void) {
    return 3;
}

// 获取NPC位置
void NPC_GetPosition(int idx, int* x, int* y) {
    if (idx >= 0 && idx < 3) {
        *x = npcs[idx].x;
        *y = npcs[idx].y;
    }
}
