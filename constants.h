#ifndef CONSTANTS_H
#define CONSTANTS_H

// --- 1. 窗口基础设置 ---
#define SCREEN_WIDTH  800   // 屏幕宽度
#define SCREEN_HEIGHT 600   // 屏幕高度
#define GAME_TITLE    "Maze Game - Module 1"

// --- 2. 地图核心设置 ---
#define CELL_SIZE     40    // 每一个格子的像素大小 (40x40)
#define MAP_ROWS      15    // 地图有多少行 (600 / 40 = 15)
#define MAP_COLS      20    // 地图有多少列 (800 / 40 = 20)

// --- 3. 地图数字字典 (队友都要背下来) ---
// 0 = 地板 (可以走)
// 1 = 墙壁 (撞头)
// 2 = 道具 (加分)
// 3 = 终点
// 4 = 陷阱
#define TYPE_FLOOR 0
#define TYPE_WALL  1
#define TYPE_ITEM  2
#define TYPE_GOAL  3
#define TYPE_TRAP  4

#endif