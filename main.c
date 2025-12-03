/*******************************************************************************************
*
*   raylib [core] example - Basic 3d example
*
*   Welcome to raylib!
*
*   To compile example, just press F5.
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2013-2023 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include "constants.h"  // 引入刚才写的规则书

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

// ---------------------------------------------------------
// 3. 主程序 (Main Loop)
// ---------------------------------------------------------
int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_TITLE);
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
            
            ClearBackground(BLACK); // 清空每一帧
            
            DrawMap(); // 调用你的画图函数
            
            // 将来队友的代码会加在这里：
            // DrawPlayer();
            // DrawNPC();
            // DrawUI();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}