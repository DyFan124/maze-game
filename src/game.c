#include "game.h"
#include <stdio.h>

static GameState currentState = GAME_STATE_MENU;

// 该函数检测玩家是否到达终点
static bool CheckPlayerReachedGoal(void) {
    int playerGridX, playerGridY;
    Player_GetPosition(&playerGridX, &playerGridY);
    
    // 获取玩家当前位置的地图格子类型
    int tileType = Map_GetTile(playerGridX, playerGridY);
    
    // 如果玩家站在终点格子上
    if (tileType == TILE_GOAL) {
        printf("玩家到达终点！胜利！\n");
        return true;
    }
    
   else return false;
}

void Game_Init(void) {
    currentState = GAME_STATE_MENU;  // 从菜单开始

    // 初始化各模块
    Audio_Init();
    Map_Init();
    Player_Init();
    NPC_Init();
    Trap_Init();
    Item_Init(); 

    printf("游戏初始化完成\n");
}

void Game_Update(void) {

     // 新增：每帧更新音乐流（必须）
    Audio_UpdateMusic();
    
    // 新增：音量调节逻辑（全局生效，和暂停键逻辑同级）
    if (IsKeyPressed(KEY_UP)) {
        Audio_SetMusicVolume(Audio_GetMusicVolume() + 0.1f);
        printf("音乐音量：%.0f%%\n", Audio_GetMusicVolume() * 100);
    }
    if (IsKeyPressed(KEY_DOWN)) {
        Audio_SetMusicVolume(Audio_GetMusicVolume() - 0.1f);
        printf("音乐音量：%.0f%%\n", Audio_GetMusicVolume() * 100);
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        Audio_SetSfxVolume(Audio_GetSfxVolume() + 0.1f);
        printf("音效音量：%.0f%%\n", Audio_GetSfxVolume() * 100);
    }
    if (IsKeyPressed(KEY_LEFT)) {
        Audio_SetSfxVolume(Audio_GetSfxVolume() - 0.1f);
        printf("音效音量：%.0f%%\n", Audio_GetSfxVolume() * 100);
    }

    switch (currentState) {
        case GAME_STATE_MENU:
            // 菜单状态，等待开始
            //enter键可以进入游戏
            if (IsKeyPressed(KEY_ENTER)) {
                currentState = GAME_STATE_PLAYING;
                 Audio_PlaySound(SFX_POINT); // ← 新增：按回车开始时播放音效  
                printf("游戏开始！\n");
            }
            break;
          //IsKeyPressed包含于raylib库

        case GAME_STATE_PLAYING:
            // 更新玩家（处理输入和移动）和npc
            Player_Update();
            Trap_Update(); 
            Item_Update(); 
            NPC_Update(); 

            // 检查是否到达终点
            if (CheckPlayerReachedGoal()) {
                currentState = GAME_STATE_WIN;
                 Audio_PlaySound(SFX_WIN);  // ← 新增：胜利时播放音效
                printf("游戏胜利！\n");
            }
            
            // 检查玩家生命值
            if (Player_GetHealth() <= 0) {
                currentState = GAME_STATE_GAME_OVER;
                Audio_PlaySound(SFX_GAMEOVER);  // ← 新增：失败时播放音效
                printf("游戏失败！\n");
            }
            
            // 按P键暂停
            if (IsKeyPressed(KEY_P)) {
                currentState = GAME_STATE_PAUSED;
                printf("游戏暂停\n");
            }
            break;
            
        case GAME_STATE_PAUSED:
            // 按P键继续
            if (IsKeyPressed(KEY_P)) {
                currentState = GAME_STATE_PLAYING;
                printf("游戏继续\n");
            }
            break;
            
        case GAME_STATE_WIN:
            // R键重新开始
            if (IsKeyPressed(KEY_R)) {
                // 重置游戏
                Game_Init();
                currentState = GAME_STATE_PLAYING;
                Audio_PlaySound(SFX_POINT);  // ← 新增：重新开始播放音效
                printf("重新开始游戏\n");
            }
            break;
            
        case GAME_STATE_GAME_OVER:
            // R键重新开始
            if (IsKeyPressed(KEY_R)) {
                Player_Init();
                currentState = GAME_STATE_PLAYING;
                Audio_PlaySound(SFX_POINT);  // ← 新增：重新开始播放音 
                printf("重新开始游戏\n");
            }
            break;
    }
}

void Game_Draw(void) {
    // 深色背景
    ClearBackground((Color){20, 20, 30, 255});
    
    switch (currentState) {
        case GAME_STATE_MENU:
            UI_DrawMenu();  // 显示主菜单
            break;
            
        case GAME_STATE_PLAYING:
            Map_Draw();
            NPC_Draw(); 
            Trap_Draw(); 
            Item_Draw();  
            Player_Draw();
            UI_DrawHUD();
            break;
            
        case GAME_STATE_PAUSED:
            Map_Draw();
            Player_Draw();
            UI_DrawHUD();
            UI_DrawPause();
            break;
            
        case GAME_STATE_WIN:
            Map_Draw();
            Player_Draw();
            UI_DrawWinScreen();
            break;
            
        case GAME_STATE_GAME_OVER:
            Map_Draw();
            Player_Draw();
            UI_DrawGameOverScreen();
            break;
    }
}

GameState Game_GetCurrentState(void) { 
    return currentState; 
}

void Game_ChangeState(GameState newState) { 
    currentState = newState; 
}