#include "game.h"
#include <stdio.h>

// 全局状态
static GameState currentState = GAME_STATE_PLAYING;

// 初始化
void Game_Init(void) {
    currentState = GAME_STATE_PLAYING;
    
    // 初始化各模块
    Map_Init();      // 地图
    Player_Init();   // 玩家（正式的）
    
    printf("=== 游戏初始化完成 ===\n");
    printf("地图系统: 已加载\n");
    printf("玩家系统: 已加载\n");
    printf("当前状态: 游戏中\n");
}

// 更新
void Game_Update(void) {
    if (currentState == GAME_STATE_PLAYING) {
        // 更新玩家（处理输入和移动）
        Player_Update();
        
        // 这里将来可以添加：
        // 1. NPC更新
        // 2. 事件更新
        // 3. 碰撞检测
        // 4. 胜负判断
    }
    
    // 状态切换逻辑（暂时简单处理）
    if (IsKeyPressed(KEY_P)) {
        currentState = (currentState == GAME_STATE_PLAYING) 
                      ? GAME_STATE_PAUSED 
                      : GAME_STATE_PLAYING;
    }
}

// 绘制
void Game_Draw(void) {
    // 使用深色背景（看起来像黑暗的迷宫）
    ClearBackground((Color){20, 20, 30, 255});  // 深蓝灰背景
    
    switch (currentState) {
        case GAME_STATE_PLAYING:
            Map_Draw();     // 只画墙壁，不画网格
            Player_Draw();  // 画缩小后的小人
            UI_DrawHUD();
            break;
        // ... 其他状态不变
    }
}

// 状态管理函数
GameState Game_GetCurrentState(void) { 
    return currentState; 
}

void Game_ChangeState(GameState newState) { 
    currentState = newState; 
}