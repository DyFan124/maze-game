#include "raylib.h"
#include "game.h"  

// ---------------------------------------------------------
// 1. 地图数据 (Map Data)
// 这里的数字决定了迷宫长什么样
// ---------------------------------------------------------
int map[MAP_ROWS][MAP_COLS] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 2, 1, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1},
    {1, 0, 1, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
    {1, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1} 
};

// ---------------------------------------------------------
// 2. 核心功能函数
// ---------------------------------------------------------

// 功能：负责把地图画在屏幕上
void DrawMap() {
    for (int y = 0; y < MAP_ROWS; y++) {
        for (int x = 0; x < MAP_COLS; x++) {
            
            int type = map[y][x];
            int posX = x * CELL_SIZE;
            int posY = y * CELL_SIZE;
            
            Color color = BLACK; // 默认颜色

            // 根据类型上色
            if (type == TYPE_WALL)  color = DARKGRAY;  // 墙
            if (type == TYPE_FLOOR) color = RAYWHITE;  // 路
            if (type == TYPE_ITEM)  color = YELLOW;    // 道具
            if (type == TYPE_GOAL)  color = GREEN;     // 终点
            if (type == TYPE_TRAP)  color = RED;       // 陷阱

            DrawRectangle(posX, posY, CELL_SIZE, CELL_SIZE, color);
            DrawRectangleLines(posX, posY, CELL_SIZE, CELL_SIZE, LIGHTGRAY); // 画边框
        }
    }
}

// 功能：查询某个坐标是什么东西
// (这是给下周做“玩家移动”的队友留的接口，非常重要！)
int GetTileType(int pixelX, int pixelY) {
    int gridX = pixelX / CELL_SIZE;
    int gridY = pixelY / CELL_SIZE;

    // 防止查到地图外面导致报错
    if (gridX < 0 || gridX >= MAP_COLS || gridY < 0 || gridY >= MAP_ROWS) {
        return -1; 
    }

    return map[gridY][gridX];
}