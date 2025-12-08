// map.c - 随机迷宫生成 (Raylib 4.5兼容)
#include "game.h"
#include <stdio.h>
#include <stdlib.h>  // 用于rand()

// ========== 私有数据 ==========
static int gameMap[MAZE_HEIGHT][MAZE_WIDTH];  // 30×50地图数组

// ========== 私有辅助函数 ==========
static void GenerateRandomMaze(void) {
    printf("正在生成随机 %dx%d 迷宫...\n", MAZE_WIDTH, MAZE_HEIGHT);
    
    // 1. 初始化为全墙
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            gameMap[y][x] = TILE_WALL;
        }
    }
    
    // 2. 使用简单算法生成迷宫（DFS-like）
    // 从起点(1,1)开始挖
    int startX = 1;
    int startY = 1;
    
    // 创建挖路用的栈
    int stack[MAZE_WIDTH * MAZE_HEIGHT][2];
    int stackSize = 0;
    
    // 起点入栈
    stack[stackSize][0] = startX;
    stack[stackSize][1] = startY;
    stackSize++;
    gameMap[startY][startX] = TILE_FLOOR;
    
    // 四个方向
    int directions[4][2] = {{0, -2}, {2, 0}, {0, 2}, {-2, 0}};  // 每次移动2格
    
    while (stackSize > 0) {
        // 获取当前点
        int currentX = stack[stackSize-1][0];
        int currentY = stack[stackSize-1][1];
        
        // 查找未访问的邻居（距离2格）
        int unvisited[4];
        int unvisitedCount = 0;
        
        for (int i = 0; i < 4; i++) {
            int nextX = currentX + directions[i][0];
            int nextY = currentY + directions[i][1];
            
            // 检查边界和是否已访问
            if (nextX > 0 && nextX < MAZE_WIDTH-1 && 
                nextY > 0 && nextY < MAZE_HEIGHT-1 && 
                gameMap[nextY][nextX] == TILE_WALL) {
                unvisited[unvisitedCount] = i;
                unvisitedCount++;
            }
        }
        
        if (unvisitedCount > 0) {
            // 随机选择一个方向
            int dirIndex = unvisited[rand() % unvisitedCount];
            int dirX = directions[dirIndex][0];
            int dirY = directions[dirIndex][1];
            
            // 挖通中间墙
            int midX = currentX + dirX/2;
            int midY = currentY + dirY/2;
            gameMap[midY][midX] = TILE_FLOOR;
            
            // 挖通目标格子
            int nextX = currentX + dirX;
            int nextY = currentY + dirY;
            gameMap[nextY][nextX] = TILE_FLOOR;
            
            // 新点入栈
            stack[stackSize][0] = nextX;
            stack[stackSize][1] = nextY;
            stackSize++;
        } else {
            // 回溯
            stackSize--;
        }
    }
    
    // 3. 设置入口和出口
    gameMap[1][1] = TILE_FLOOR;                    // 入口
    gameMap[MAZE_HEIGHT-2][MAZE_WIDTH-2] = TILE_FLOOR;  // 出口位置先挖通
    
    // 标记出口
    gameMap[MAZE_HEIGHT-2][MAZE_WIDTH-2] = TILE_GOAL;
    
    // 4. 确保所有边界是墙
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        gameMap[y][0] = TILE_WALL;
        gameMap[y][MAZE_WIDTH-1] = TILE_WALL;
    }
    for (int x = 0; x < MAZE_WIDTH; x++) {
        gameMap[0][x] = TILE_WALL;
        gameMap[MAZE_HEIGHT-1][x] = TILE_WALL;
    }
    
    printf("随机迷宫生成完成！\n");
    printf("入口: (1,1), 出口: (%d,%d)\n", MAZE_WIDTH-2, MAZE_HEIGHT-2);
}

// ========== 接口函数实现 ==========

void Map_Init(void) {
    printf("[地图] 初始化 %dx%d 迷宫\n", MAZE_WIDTH, MAZE_HEIGHT);
    
    // 初始化随机种子
    SetRandomSeed((unsigned int)time(NULL));  // Raylib 4.5兼容方式
    
    // 生成随机迷宫
    GenerateRandomMaze();
    
    printf("[地图] 初始化完成\n");
}

void Map_Draw(void) {
    // 简单绘制：只画墙壁和通路
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            int pixelX = x * CELL_SIZE;
            int pixelY = y * CELL_SIZE;
            
            int tileType = gameMap[y][x];
            Color tileColor;
            
            if (tileType == TILE_WALL) {
                tileColor = DARKGRAY;
            } else if (tileType == TILE_GOAL) {
                tileColor = GREEN;
            } else {
                tileColor = LIGHTGRAY;
            }
            
            DrawRectangle(pixelX, pixelY, CELL_SIZE, CELL_SIZE, tileColor);
            DrawRectangleLines(pixelX, pixelY, CELL_SIZE, CELL_SIZE, GRAY);
        }
    }
}

int Map_GetTile(int gridX, int gridY) {
    if (gridX < 0 || gridX >= MAZE_WIDTH || 
        gridY < 0 || gridY >= MAZE_HEIGHT) {
        return TILE_WALL;
    }
    
    return gameMap[gridY][gridX];
}

bool Map_IsWalkable(int gridX, int gridY) {
    int tileType = Map_GetTile(gridX, gridY);
    return (tileType == TILE_FLOOR || tileType == TILE_GOAL);
}
