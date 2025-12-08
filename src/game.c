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
    switch (currentState) {
        case GAME_STATE_MENU:
            UI_DrawMenu();           // 菜单界面
            break;
            
        case GAME_STATE_PLAYING:
            Map_Draw();              // 地图
            Player_Draw();           // 玩家
            UI_DrawHUD();            // 游戏内UI
            break;
            
        case GAME_STATE_PAUSED:      // 新增暂停状态
            Map_Draw();              // 先画游戏画面
            Player_Draw();
            UI_DrawHUD();
            UI_DrawPause();          // 再画暂停覆盖层
            break;
            
        case GAME_STATE_WIN:
            Map_Draw();              // 底层游戏画面
            Player_Draw();
            UI_DrawWinScreen();      // 胜利界面
            break;
            
        case GAME_STATE_GAME_OVER:
            Map_Draw();              // 底层游戏画面
            Player_Draw();
            UI_DrawGameOverScreen(); // 失败界面
            break;
    }
}

// 状态管理函数
GameState Game_GetCurrentState(void) { 
    return currentState; 
}

void Game_ChangeState(GameState newState) { 
    currentState = newState; 
}