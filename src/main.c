
#include "game.h"

int main(void)
{
    // ========== 1. 引擎启动 ==========
    // 创建游戏窗口，这是Raylib程序的起点
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "迷宫大冒险");
    // 设置目标帧率，控制游戏速度
    SetTargetFPS(TARGET_FPS);
    
    // ========== 2. 游戏初始化 ==========
    // 调用你写的 Game_Init()，它会初始化所有模块
    Game_Init();
    
    // ========== 3. 核心游戏循环 ==========
    // 这个 while 循环是游戏的心脏，每秒运行 TARGET_FPS 次（默认60次）
    // 每次循环称为一“帧”(Frame)
    while (!WindowShouldClose())  // 检测ESC键或窗口关闭按钮
    {
        // ----- 3.1 更新阶段：处理逻辑 -----
        // 调用你写的 Game_Update()，它会：
        // - 根据当前状态调用对应模块的更新函数
        // - 处理碰撞检测
        // - 管理游戏状态切换
        Game_Update();
        
        // ----- 3.2 绘制阶段：渲染画面 -----
        // 开始绘制新的一帧
        BeginDrawing();
        {
            // 用黑色清空上一帧的画面（避免拖影）
            ClearBackground(BLACK);
            
            // 调用你写的 Game_Draw()，它会：
            // - 根据当前状态调用对应模块的绘制函数
            // - 按正确顺序绘制所有元素（先背景后前景）
            Game_Draw();
            
            // 可选：显示帧率（调试用）
            DrawFPS(10, SCREEN_HEIGHT - 30);
        }
        // 结束绘制，这一帧的内容现在显示到窗口上
        EndDrawing();
    }
    
    // ========== 4. 游戏结束，清理资源 ==========
    // 关闭窗口，释放Raylib占用的所有资源
    CloseWindow();
    
    // 程序正常退出
    return 0;
}