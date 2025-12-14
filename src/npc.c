#include <stdlib.h>
#include <stdio.h>
#include "game.h"

//NPC系统模块 - 负责迷宫中敌对NPC的生成、随机移动、碰撞检测、绘制等核心逻辑
//核心功能:
//1.随机生成指定数量的NPC（仅在可通行的地面生成）
//2.NPC独立计时、随机移动（上下左右4个方向）
//3.检测NPC与玩家碰撞，触发玩家扣血并让NPC回退
//4.绘制NPC图形（蓝色方块+深蓝色边框）
//5.提供获取NPC数量和位置的接口函数

 // NPC系统核心常量定义
#define npcCount  3

 // NPC结构体 - 存储单个NPC的状态和属性
typedef struct {
    int x, y;               // NPC在迷宫网格中的坐标
    bool active;            // NPC是否处于激活状态
    float moveTimer;        // 单个NPC的独立移动计时器（控制移动频率）
} NPC;

// 全局变量定义
static NPC npcs[npcCount];        // 存储3个NPC的数组


void NPC_Init(void) {
    for (int i = 0; i < 3; i++) {
        // 循环生成随机位置，直到找到可通行的地面
        do {
            // 生成5 ~ (MAZE_WIDTH-10) 范围内的随机X坐标（避开迷宫边缘）
            npcs[i].x = 5 + rand() % (MAZE_WIDTH - 10);
            // 生成5 ~ (MAZE_HEIGHT-10) 范围内的随机Y坐标（避开迷宫边缘）
            npcs[i].y = 5 + rand() % (MAZE_HEIGHT - 10);
        } while (!Map_IsWalkable(npcs[i].x, npcs[i].y));  // 确保位置可通行

        npcs[i].active = true;                          // NPC初始化为激活状态
        npcs[i].moveTimer = (float)i * 0.2f;            // 错开移动时间（0/0.2/0.4秒），避免同步移动(移动节奏错开)
    } 
    printf("[NPC] 初始化完成\n");
}

void NPC_Update(void) {
    // 获取当前帧的持续时间（秒），用于时间相关计算
    float deltaTime = GetFrameTime();

    // 获取玩家当前的网格坐标（用于碰撞检测）
    int px, py;
    Player_GetPosition(&px, &py);

    // 遍历更新每个NPC的状态
    for (int i = 0; i < 3; i++) {
        if (!npcs[i].active) continue;  // 跳过非激活状态的NPC

        // 累加当前NPC的独立移动计时器
        npcs[i].moveTimer += deltaTime;

        // 控制移动频率：每0.3秒移动一次，未到时间则跳过
        if (npcs[i].moveTimer < 0.3f) continue;
        npcs[i].moveTimer = 0;  // 重置移动计时器

        // 保存移动前的位置（用于碰撞后回退）
        int oldX = npcs[i].x;
        int oldY = npcs[i].y;

        // 完全随机选择移动方向（0=上，1=右，2=下，3=左）
        int dir = rand() % 4;
        int newX = oldX, newY = oldY;  // 初始化新坐标为当前坐标

        // 根据随机方向计算新坐标
        switch (dir) {
        case 0: newY--; break; // 向上移动：Y坐标-1
        case 1: newX++; break; // 向右移动：X坐标+1
        case 2: newY++; break; // 向下移动：Y坐标+1
        case 3: newX--; break; // 向左移动：X坐标-1
        }

        // 检查新位置是否可通行（非墙壁），可通行则更新NPC坐标
        if (Map_IsWalkable(newX, newY)) {
            npcs[i].x = newX;
            npcs[i].y = newY;
        }

        // 碰撞检测：NPC与玩家坐标重合则触发扣血
        if (npcs[i].x == px && npcs[i].y == py) {
            Player_TakeDamage(30);          // 玩家受到30点伤害
            npcs[i].x = oldX;               // NPC回退到移动前的位置
            npcs[i].y = oldY;
        }
    }
}
void NPC_Draw(void) {
    // 遍历绘制每个激活的NPC
    for (int i = 0; i < 3; i++) {
        if (!npcs[i].active) continue;  // 跳过非激活状态的NPC

        // 将网格坐标转换为屏幕像素坐标（CELL_SIZE为每个网格的像素大小）
        int screenX = npcs[i].x * CELL_SIZE;
        int screenY = npcs[i].y * CELL_SIZE;

        // 绘制NPC主体：蓝色填充方块
        DrawRectangle(screenX, screenY, CELL_SIZE, CELL_SIZE, BLUE);
        // 绘制NPC边框：深蓝色线条，增强辨识度
        DrawRectangleLines(screenX, screenY, CELL_SIZE, CELL_SIZE, DARKBLUE);
    }
}

 //接口函数，获取当前NPC的总数量
int NPC_GetCount(void) {
    return 3;  // 返回固定值3（可优化为返回npcCount变量，增强可维护性）
}

//接口函数，让游戏的其他模块安全地获取指定 NPC 的位置
void NPC_GetPosition(int idx, int* x, int* y) {
    // 索引合法性检查，避免数组越界
    if (idx >= 0 && idx < 3) {
        *x = npcs[idx].x;  // 赋值X坐标到输出参数
        *y = npcs[idx].y;  // 赋值Y坐标到输出参数
    }
}