// Game_Init 函数实现
#include "game.h"

// 私有全局变量
static GameState currentGameState = GAME_STATE_MENU; // 初始为菜单状态
static bool isGameInitialized = false; // 防止重复初始化

void Game_Init(void)
{
    // 安全检查：避免重复初始化
    if (isGameInitialized) {
        TraceLog(LOG_WARNING, "游戏已经初始化过，跳过重复初始化");
        return;
    }
    
    TraceLog(LOG_INFO, "游戏初始化开始");
    
    // 1. 初始化各模块
    
    // 1.1 地图系统
    TraceLog(LOG_INFO, "初始化地图系统...");
    Map_Init();
    
    // 1.2 玩家系统
    TraceLog(LOG_INFO, "初始化玩家系统...");
    Player_Init();
    
    // 1.3 NPC系统
    TraceLog(LOG_INFO, "初始化NPC系统...");
    NPC_Init();
    
    // 1.4 事件系统
    // Event_Init(); // 如果模块3实现了这个函数
    
    // UI系统通常不需要显式初始化
    
    // ========== 2. 设置初始游戏状态 ==========
    currentGameState = GAME_STATE_MENU; // 从主菜单开始
    // 或者直接开始游戏：currentGameState = GAME_STATE_PLAYING;
    
    // ========== 3. 初始化游戏全局数据 ==========
    // 这里可以添加游戏特有的全局数据初始化
    // 例如：gameTime = 0.0f; difficulty = DIFFICULTY_EASY;
    
    TraceLog(LOG_INFO, "=== 游戏初始化完成 ===");
    TraceLog(LOG_INFO, "初始状态: %s", 
             (currentGameState == GAME_STATE_MENU) ? "主菜单" : "游戏中");
    
    isGameInitialized = true; // 标记为已初始化
}