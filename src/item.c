#include "game.h"
#include <stdlib.h>
#include <stdio.h>

// 最简单的道具系统
#define ITEM_COUNT 15
#define ITEM_SCORE 10

typedef struct {
    int x, y;
    bool active;
} Item;

static Item items[ITEM_COUNT];

// 初始化道具
void Item_Init(void) {
    for (int i = 0; i < ITEM_COUNT; i++) {
        // 随机位置，确保是地面
        do {
            items[i].x = 5 + rand() % (MAZE_WIDTH - 10);
            items[i].y = 5 + rand() % (MAZE_HEIGHT - 10);
        } while (!Map_IsWalkable(items[i].x, items[i].y));
        
        items[i].active = true;
    }
    printf("[道具] 初始化完成\n");
}

// 更新道具
void Item_Update(void) {
    int px, py;
    Player_GetPosition(&px, &py);
    
    for (int i = 0; i < ITEM_COUNT; i++) {
        if (items[i].active && items[i].x == px && items[i].y == py) {
            Player_AddScore(ITEM_SCORE);
            items[i].active = false;
            printf("捡到道具 +%d分\n", ITEM_SCORE);
        }
    }
}

// 绘制道具
void Item_Draw(void) {
    for (int i = 0; i < ITEM_COUNT; i++) {
        if (!items[i].active) continue;
        
        int screenX = items[i].x * CELL_SIZE;
        int screenY = items[i].y * CELL_SIZE;
        
        // 画一个黄色方块
        DrawRectangle(screenX + 4, screenY + 4, 
                     CELL_SIZE - 8, CELL_SIZE - 8, YELLOW);
        
        // 画边框
        DrawRectangleLines(screenX, screenY, 
                          CELL_SIZE, CELL_SIZE, GOLD);
    }
}