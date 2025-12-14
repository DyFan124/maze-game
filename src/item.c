#include "game.h"
#include <stdlib.h>
#include <stdio.h>

//道具系统模块 - 负责迷宫中道具的生成、拾取检测、积分、绘制等核心逻辑
//核心功能：
//1. 随机生成指定数量的道具（仅在可通行地面生成）
//2. 检测玩家拾取道具并增加分数
//3. 拾取后的道具失效
//4. 绘制道具图形（黄色方块+金色边框）

 // 道具系统核心常量定义
#define ITEM_COUNT  15      // 迷宫中生成的道具总数量
#define ITEM_SCORE  10      // 拾取单个道具获得的分数

// 道具结构体：存储单个道具的位置和状态
typedef struct {
    int x, y;               // 道具在迷宫网格中的坐标（非像素坐标）
    bool active;            // 道具是否处于激活状态（可拾取）
} Item;

// 全局道具数组--存储所有道具的状态
static Item items[ITEM_COUNT];

void Item_Init(void) {
    for (int i = 0; i < ITEM_COUNT; i++) {
        // 循环生成随机位置，直到找到可通行的地面
        do {
            // 生成5 ~ (MAZE_WIDTH-10) 范围内的随机X坐标（避开迷宫边缘）
            items[i].x = 5 + rand() % (MAZE_WIDTH - 10);
            // 生成5 ~ (MAZE_HEIGHT-10) 范围内的随机Y坐标（避开迷宫边缘）
            items[i].y = 5 + rand() % (MAZE_HEIGHT - 10);
        } while (!Map_IsWalkable(items[i].x, items[i].y));  // 确保位置可通行

        items[i].active = true;  // 道具初始化为激活状态（可拾取）
    }
    printf("[道具] 初始化完成\n"); 
}

void Item_Update(void) {
    // 获取玩家当前的网格坐标（用于碰撞检测）
    int px, py;
    Player_GetPosition(&px, &py);

    // 遍历检测每个道具是否被拾取
    for (int i = 0; i < ITEM_COUNT; i++) {
        // 仅检测激活状态的道具，且玩家坐标与道具坐标重合
        if (items[i].active && items[i].x == px && items[i].y == py) {
            Player_AddScore(ITEM_SCORE);  // 玩家增加对应分数
            items[i].active = false;      // 道具设为失效（不可重复拾取）
            printf("捡到道具 +%d分\n", ITEM_SCORE);  
        }
    }
}

void Item_Draw(void) {
    // 遍历绘制每个激活的道具
    for (int i = 0; i < ITEM_COUNT; i++) {
        if (!items[i].active) continue;  // 跳过失效的道具

        // 将网格坐标转换为屏幕像素坐标（CELL_SIZE为每个网格的像素大小）
        int screenX = items[i].x * CELL_SIZE;
        int screenY = items[i].y * CELL_SIZE;

        // 绘制道具主体：黄色方块（带4像素内边距）
        DrawRectangle(screenX + 4, screenY + 4,
            CELL_SIZE - 8, CELL_SIZE - 8, YELLOW);

        // 绘制道具边框：金色线条，增强辨识度
        DrawRectangleLines(screenX, screenY,
            CELL_SIZE, CELL_SIZE, GOLD);
    }
}