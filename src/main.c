
#include "game.h"

int main(void)
{
    // 1. 创建游戏窗口
    //InitWindow是raylib库包含的函数
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "MAZEGAME");
    // 设置目标帧率
    SetTargetFPS(TARGET_FPS);
    
    //  2. 游戏初始化
    // 调用 Game_Init()，初始化所有模块
    Game_Init();
    
    // 3. 核心游戏循环 
    while (!WindowShouldClose())  // 检测ESC键或窗口关闭按钮
    {
        // 调用Game_Update()
        // 根据当前状态调用对应模块的更新函数
        // 处理碰撞检测
        // 管理游戏状态切换
        Game_Update();
        
        //绘制阶段
        BeginDrawing();
        //BeginDrawing()是raylib库包含的函数告诉系统开始绘图！！必须配套使用
        {
            // 用黑色清空上一帧的画面
            ClearBackground(BLACK);
            
            // 调用Game_Draw()
            // 根据当前状态调用对应模块的绘制函数
            // 绘制所有元素（先背景后前景）
            Game_Draw();

        }
        // 结束绘制，这一帧的内容现在显示到窗口上
        EndDrawing();
    }
    
    //  4. 游戏结束
        CloseWindow();// 关闭窗口

    return 0;
}