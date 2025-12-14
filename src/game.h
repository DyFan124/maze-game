#ifndef GAME_H
#define GAME_H

#include "raylib.h"  
#include <stdbool.h>  


//游戏全局配置
// 这些常量所有人共用
#define SCREEN_WIDTH  2000    
#define SCREEN_HEIGHT 1200    
#define TARGET_FPS    60      
#define CELL_SIZE     40      
#define MAZE_WIDTH    50     
#define MAZE_HEIGHT   30      


//地图定义 
//全组必须统一使用这些常量值，不可自行定义其他值
#define TILE_FLOOR 0   // 通路/空地（可走）
#define TILE_WALL  1   // 墙壁（不可走）
#define TILE_ITEM  2   // 道具（可捡起加分）
#define TILE_GOAL  3   // 终点
#define TILE_TRAP  4   // 陷阱（踩中扣血）

// 游戏数值平衡
#define PLAYER_MAX_HEALTH 100 //玩家最大生命值
#define ITEM_SCORE_BONUS  20  //捡道具加分
#define TRAP_DAMAGE       20  //陷阱伤害
#define NPC_DAMAGE       30   //NPC碰撞伤害



// 游戏状态：主循环状态
typedef enum {
    GAME_STATE_MENU,      // 主菜单
    GAME_STATE_PLAYING,   // 游戏进行中
    GAME_STATE_PAUSED,    // 暂停
    GAME_STATE_WIN,       // 胜利
    GAME_STATE_GAME_OVER  // 失败
} GameState;

typedef struct {
    int gridX;      // 网格坐标X
    int gridY;      // 网格坐标Y
    int health;     // 生命值
    int score;      // 分数
} PlayerData;//玩家数据

// 模块接口声明 (各模块负责实现)
// 已声明接口，各自在自己的c文件中实现功能


//模块1：地图系统接口 (map.c 实现) 
// 负责人:蒋天宇
void Map_Init(void);                     // 初始化地图（生成迷宫）
void Map_Draw(void);                     // 绘制整个地图到屏幕
int Map_GetTile(int gridX, int gridY);   // 获取指定格子类型（返回 TILE_WALL 等值）
bool Map_IsWalkable(int gridX, int gridY); // 判断格子是否可通行
void Map_SetTile(int gridX, int gridY, int tileType); // 设置格子类型（用于动态事件）
int* Map_GetData(void);                   //获取地图数据指针

//  模块2：玩家系统接口 (player.c 实现) 
// 负责人：梁梦淇
void Player_Init(void);                       // 初始化玩家状态（位置、血量等）
void Player_Update(void);                     // 每帧更新：处理输入、移动、碰撞
void Player_Draw(void);                       // 绘制玩家到屏幕
int  Player_GetHealth(void);                  // 获取当前生命值（供UI显示）
int  Player_GetScore(void);                   // 获取当前积分（供UI显示）
void Player_GetPosition(int* outGridX, int* outGridY); // 获取玩家坐标（供碰撞检测）
void Player_TakeDamage(int damage);           // 玩家受伤害
void Player_AddScore(int points);             // 增加玩家积分
PlayerData* Player_GetData(void);             // 获取玩家数据

// 模块3：NPC与事件系统接口 (npc.c 实现) 
// 负责人：黄绍爵
void NPC_Init(void);                          // 初始化所有NPC
void NPC_Update(void);                        // 每帧更新所有NPC的移动
void NPC_Draw(void);                          // 绘制所有NPC
int  NPC_GetCount(void);                      // 获取NPC数量（供碰撞检测循环）
void NPC_GetPosition(int npcIndex, int* outGridX, int* outGridY); // 获取指定NPC坐标
void Event_Update(void);                      // 更新动态事件系统

//陷阱系统接口 (trap.c 实现) 
void Trap_Init(void);      // 初始化所有陷阱
void Trap_Update(void);    // 每帧更新陷阱状态
void Trap_Draw(void);      // 绘制所有陷阱

// 积分道具系统接口 (item.c 实现) 
// 道具系统
void Item_Init(void);      // 初始化道具
void Item_Update(void);    // 更新道具
void Item_Draw(void);      // 绘制道具


// 模块4：UI系统接口 (ui.c )
// 负责人：邓刘壹
void UI_DrawHUD(void);                        // 绘制游戏内UI（血条、分数、提示）
void UI_DrawMenu(void);                       // 绘制主菜单界面
void UI_DrawPause(void);                      // 绘制暂停界面
void UI_DrawWinScreen(void);                  // 绘制胜利界面
void UI_DrawGameOverScreen(void);             // 绘制失败界面
int  Player_GetScore(void);                   // 玩家得分

// 第五部分：总控函数声明 (在game.c中实现) 
// 这些是用于整合调度所有模块的函数

void Game_Init(void);                         // 初始化整个游戏
void Game_Update(void);                       // 更新游戏状态
void Game_Draw(void);                         // 每帧绘制游戏
GameState Game_GetCurrentState(void);         // 获取当前游戏状态
void Game_ChangeState(GameState newState);    // 改变游戏状态


// ========== 音频类型定义（和你的TILE_XXX、GAME_STATE_XXX风格统一） ==========
// 音效枚举：全组统一使用，修改需通知界面设计师
// 音频类型定义
typedef enum {
    SFX_START,       // 菜单背景音乐/开始音效
    SFX_POINT,       // 加分/道具音效
    SFX_WIN,         // 胜利音效
    SFX_GAMEOVER,    // 失败音效
    SFX_DAMAGE,      // 受击/陷阱音效
    SFX_SCORE,       // 得分音效
    SFX_COUNT        // 总音效数（用于数组长度，不可修改）
} SoundType;

// ========== 音频模块接口声明（对应game.h的模块4/5风格） ==========
// 负责人：界面设计师
// 音频模块接口声明
void Audio_Init(void);                          // 初始化音频设备+加载所有音效
void Audio_PlaySound(SoundType type);           // 播放指定音效
void Audio_SetSfxVolume(float volume);          // 设置音效音量（0.0~1.0）
void Audio_SetMusicVolume(float volume);        // 设置背景音乐音量（0.0~1.0）
float Audio_GetSfxVolume(void);                 // 获取当前音效音量（供UI显示）
float Audio_GetMusicVolume(void);               // 获取当前音乐音量（供UI显示）
void Audio_UpdateMusic(void);                   // 更新音乐流（每帧调用）
void Audio_Unload(void);                        // 释放音频资源

#endif 
