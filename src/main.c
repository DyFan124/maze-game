// main.c - 只加2行
#include "game.h"

int main(void)
{
    // 1. 引擎启动 
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "迷宫大冒险");
    SetTargetFPS(TARGET_FPS);
    
    // 2. 游戏初始化
    Game_Init();
    
    // 3. 核心游戏循环 
    while (!WindowShouldClose())
    {
        Game_Update();
        
        BeginDrawing();
        {
            ClearBackground(BLACK);
            Game_Draw();
            DrawFPS(10, SCREEN_HEIGHT - 30);
        }
        EndDrawing();
    }
    
    // 4. 游戏结束，清理资源
    // 关键：卸载纹理（防止内存泄漏）
    UnloadTexture(playerTexture);  // 添加这一行
    
    CloseWindow();
    
    return 0;
}