#ifndef GAME_H
#define GAME_H

#include "raylib.h"  
#include <stdbool.h>  

// ========== 第一部分：游戏全局配置 (总指挥定义) ==========
// 这些常量所有人共用，修改需通知全组
#define SCREEN_WIDTH  2000 // 窗口宽度（像素）
#define SCREEN_HEIGHT 1200 
#define TARGET_FPS    60      // 目标帧率
#define CELL_SIZE     40      // 每个网格的像素大小
#define MAZE_WIDTH    15      // 迷宫宽度（格子数）
#define MAZE_HEIGHT   15      // 迷宫高度（格子数）
#define MAP_ROWS      15      // 地图有多少行 (600 / 40 = 15)
#define MAP_COLS      20      // 地图有多少列 (800 / 40 = 20)

// ========== 第二部分：地图格子类型定义 (采用地图系统同学的方案) ==========
// 注意：全组必须统一使用这些常量值，不可自行定义其他值
#define TILE_FLOOR 0   // 通路/空地（可走）
#define TILE_WALL  1   // 墙壁（不可走）
#define TILE_ITEM  2   // 道具（可捡起加分）
#define TILE_GOAL  3   // 终点/出口（通关目标）
#define TILE_TRAP  4   // 陷阱（踩中扣血）

// 游戏数值平衡（总指挥可协调调整）
#define PLAYER_MAX_HEALTH 100 // 玩家最大生命值
#define ITEM_SCORE_BONUS  20  // 捡道具加分
#define TRAP_DAMAGE      20   // 陷阱伤害
#define NPC_DAMAGE       30   // NPC碰撞伤害

// ========== 第三部分：核心数据类型 (总指挥定义) ==========
// 这些类型所有人共用，确保数据格式统一

// 游戏状态：主循环状态机的依据
typedef enum {
    GAME_STATE_MENU,      // 主菜单
    GAME_STATE_PLAYING,   // 游戏进行中
    GAME_STATE_PAUSED,    // 暂停
    GAME_STATE_WIN,       // 胜利
    GAME_STATE_GAME_OVER  // 失败
} GameState;

// ========== 第四部分：模块接口声明 (各模块负责实现) ==========
// 总指挥声明接口，各模块在自己的.c文件中实现具体功能
// 注意：所有与地图格子相关的函数现在都使用 int 类型，值必须为上面的 TILE_XXX 常量

// ---------- 模块1：地图系统接口 (map.c 实现) ----------
// 负责人：游戏地基师
void Map_Init(void);                     // 初始化地图（生成迷宫）
void Map_Draw(void);                     // 绘制整个地图到屏幕
int Map_GetTile(int gridX, int gridY);   // 获取指定格子类型（返回 TILE_WALL 等值）
void Map_SetTile(int gridX, int gridY, int tileType); // 设置格子类型（用于动态事件）
bool Map_IsWalkable(int gridX, int gridY); // 判断格子是否可通行

// ---------- 模块2：玩家系统接口 (player.c 实现) ----------
// 负责人：主角操控师
void Player_Init(void);                       // 初始化玩家状态（位置、血量等）
void Player_Update(void);                     // 每帧更新：处理输入、移动、碰撞
void Player_Draw(void);                       // 绘制玩家到屏幕
int  Player_GetHealth(void);                  // 获取当前生命值（供UI显示）
int  Player_GetScore(void);                   // 获取当前积分（供UI显示）
void Player_GetPosition(int* outGridX, int* outGridY); // 获取玩家网格坐标（供碰撞检测）
void Player_TakeDamage(int damage);           // 玩家受伤害（由总指挥在碰撞时调用）
void Player_AddScore(int points);             // 增加玩家积分

// ---------- 模块3：NPC与事件系统接口 (npc.c / event.c 实现) ----------
// 负责人：难度挑战者
void NPC_Init(void);                          // 初始化所有NPC
void NPC_Update(void);                        // 每帧更新所有NPC的AI和移动
void NPC_Draw(void);                          // 绘制所有NPC
int  NPC_GetCount(void);                      // 获取NPC数量（供碰撞检测循环）
void NPC_GetPosition(int npcIndex, int* outGridX, int* outGridY); // 获取指定NPC坐标
void Event_Update(void);                      // 更新动态事件系统（定时触发事件）

// ---------- 模块4：UI系统接口 (ui.c 实现) ----------
// 负责人：界面设计师
void UI_DrawHUD(void);                        // 绘制游戏内UI（血条、分数、提示）
void UI_DrawMenu(void);                       // 绘制主菜单界面
void UI_DrawPause(void);                      // 绘制暂停界面
void UI_DrawWinScreen(void);                  // 绘制胜利界面
void UI_DrawGameOverScreen(void);             // 绘制失败界面

// ========== 第五部分：总控函数声明 (总指挥在game.c中实现) ==========
// 这些是总指挥的专属函数，用于整合调度所有模块

void Game_Init(void);                         // 初始化整个游戏（调用各模块Init）
void Game_Update(void);                       // 每帧更新游戏逻辑（状态机、碰撞检测、调度模块更新）
void Game_Draw(void);                         // 每帧绘制游戏（根据状态调度模块绘制）
GameState Game_GetCurrentState(void);         // 获取当前游戏状态
void Game_ChangeState(GameState newState);    // 改变游戏状态（如从MENU切换到PLAYING）

#endif // GAME_H