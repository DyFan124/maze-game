// ui.c - 适配版本（最小改动）
#include "game.h"  // ✅ 注意小写，包含你的常量

// ===== 使用你的实际常量替换原有宏定义 =====
// 删除原来的 WINDOW_W/WINDOW_H，直接使用 SCREEN_WIDTH/SCREEN_HEIGHT
// #define WINDOW_W 2000       //  删除，使用 SCREEN_WIDTH
// #define WINDOW_H 1200       //  删除，使用 SCREEN_HEIGHT

// 保持UI布局常量（基于你的2000×1200窗口）
#define UI_PANEL_X 1700      // 面板X位置（靠右）
#define UI_PANEL_Y 50        // 面板Y位置
#define UI_PANEL_W 280       // 面板宽度
#define UI_PANEL_H 320       // 面板高度
#define HP_BAR_W 240         // 血条宽度
#define HP_BAR_H 20          // 血条高度
#define UI_TITLE_SIZE 28     // 标题字体
#define UI_TEXT_SIZE 24      // 正文字体
#define UI_SMALL_TEXT_SIZE 20 // 小字体

// ===== 内部工具函数（不变）=====
static void DrawGradientHealthBar(int x, int y, int width, int height, int currentHp, int maxHp) {
    // 完全不变...
    DrawRectangle(x, y, width, height, DARKGRAY);
    DrawRectangleLines(x, y, width, height, GRAY);

    float hpPercent = (float)currentHp / maxHp;
    int barWidth = (int)(width * hpPercent);
    if (barWidth <= 0) return;

    Color startColor, endColor;
    if (currentHp >= maxHp) {
        startColor = LIME;
        endColor = DARKGREEN;
    }
    else if (currentHp > maxHp / 2) {
        startColor = YELLOW;
        endColor = GOLD;
    }
    else {
        startColor = RED;
        endColor = MAROON;
    }

    for (int i = 0; i < barWidth; i++) {
        float t = (float)i / barWidth;
        Color color = {
            (unsigned char)(startColor.r - t * (startColor.r - endColor.r)),
            (unsigned char)(startColor.g - t * (startColor.g - endColor.g)),
            (unsigned char)(startColor.b - t * (startColor.b - endColor.b)),
            255
        };
        DrawLine(x + i, y, x + i, y + height, color);
    }
}

// ===== UI核心函数（最小改动）=====

void UI_DrawMenu(void) {
    // 渐变背景 - 使用SCREEN_WIDTH/HEIGHT
    for (int y = 0; y < SCREEN_HEIGHT; y++) {  // ✅ WINDOW_H → SCREEN_HEIGHT
        float t = (float)y / SCREEN_HEIGHT;     // ✅
        Color bgColor = t < 0.5 ? DARKGRAY : BLACK;
        DrawLine(0, y, SCREEN_WIDTH, y, bgColor);  // ✅ WINDOW_W → SCREEN_WIDTH
    }

    // 标题（位置微调，保持居中）
    const char* title = "MAZE GAME";
    int titleSize = 72;
    int titleWidth = MeasureText(title, titleSize);
    int titleX = (SCREEN_WIDTH - titleWidth) / 2;  // ✅
    DrawText(title, titleX + 4, 304, titleSize, MAROON);
    DrawText(title, titleX, 300, titleSize, RED);

    // 开始按钮（位置微调）
    Rectangle btn = { (SCREEN_WIDTH - 300) / 2, 600, 300, 80 };  // ✅
    Color btnColor = CheckCollisionPointRec(GetMousePosition(), btn) ? GRAY : DARKGRAY;
    Color btnBorderColor = CheckCollisionPointRec(GetMousePosition(), btn) ? RED : MAROON;

    DrawRectangle(btn.x, btn.y, btn.width, btn.height, btnColor);
    DrawRectangleLinesEx(btn, 3, btnBorderColor);
    const char* btnText = "START GAME";
    int btnTextWidth = MeasureText(btnText, 32);
    DrawText(btnText, btn.x + (btn.width - btnTextWidth) / 2, btn.y + 20, 32, LIGHTGRAY);

    // 点击按钮切换状态（保持不变）
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), btn)) {
        Game_ChangeState(GAME_STATE_PLAYING);
    }
}

void UI_DrawHUD(void) {
    // 获取玩家信息
    int playerHealth = Player_GetHealth();
    
    // 绘制UI面板（位置不变）
    DrawRectangle(UI_PANEL_X, UI_PANEL_Y, UI_PANEL_W, UI_PANEL_H, Fade(DARKGRAY, 0.95));
    DrawRectangleLines(UI_PANEL_X, UI_PANEL_Y, UI_PANEL_W, UI_PANEL_H, GRAY);

    // 标题栏
    DrawText("STATUS", UI_PANEL_X + 15, UI_PANEL_Y + 15, UI_TITLE_SIZE, RED);
    DrawLine(UI_PANEL_X + 15, UI_PANEL_Y + 45, UI_PANEL_X + UI_PANEL_W - 15, UI_PANEL_Y + 45, MAROON);

    // 分数（简化：只显示玩家积分）
    DrawText("Score: ", UI_PANEL_X + 25, UI_PANEL_Y + 100, UI_TEXT_SIZE, LIGHTGRAY);
    DrawText(TextFormat("%d", Player_GetScore()), UI_PANEL_X + 100, UI_PANEL_Y + 100, UI_TEXT_SIZE, GOLD);

    // 道具数量（暂时显示0）
    DrawText("Items: ", UI_PANEL_X + 25, UI_PANEL_Y + 140, UI_TEXT_SIZE, LIGHTGRAY);
    DrawText("0", UI_PANEL_X + 110, UI_PANEL_Y + 140, UI_TEXT_SIZE, YELLOW);  // ✅ 硬编码0

    // 陷阱状态（暂时显示NO）
    DrawText("Trap: ", UI_PANEL_X + 25, UI_PANEL_Y + 180, UI_TEXT_SIZE, LIGHTGRAY);
    DrawText("NO", UI_PANEL_X + 100, UI_PANEL_Y + 180, UI_TEXT_SIZE, LIME);  // ✅ 硬编码NO

    // 渐变血条（使用玩家的实际最大生命值，假设为100）
    #define PLAYER_MAX_HEALTH 100  // 与game.h一致
    DrawGradientHealthBar(UI_PANEL_X + 25, UI_PANEL_Y + 220, HP_BAR_W, HP_BAR_H, 
                         playerHealth, PLAYER_MAX_HEALTH);  // ✅ 使用实际值
    
    DrawText(TextFormat("HP: %d/%d", playerHealth, PLAYER_MAX_HEALTH), 
            UI_PANEL_X + 25, UI_PANEL_Y + 250, UI_SMALL_TEXT_SIZE, GRAY);
}

void UI_DrawPause(void) {
    // 半透明覆盖层
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 150});  // ✅
    
    // 暂停文字（居中）
    const char* pauseText = "GAME PAUSED";
    int textWidth = MeasureText(pauseText, 60);
    DrawText(pauseText, (SCREEN_WIDTH - textWidth)/2, SCREEN_HEIGHT/2 - 50, 60, YELLOW);  // ✅
    
    // 继续提示
    const char* continueText = "Press P to continue";
    int contWidth = MeasureText(continueText, 30);
    DrawText(continueText, (SCREEN_WIDTH - contWidth)/2, SCREEN_HEIGHT/2 + 20, 30, WHITE);  // ✅
}

void UI_DrawWinScreen(void) {
    // 渐变背景
    for (int y = 0; y < SCREEN_HEIGHT; y++) {  // ✅
        float t = (float)y / SCREEN_HEIGHT;
        Color bgColor = t < 0.5 ? DARKGRAY : BLACK;
        DrawLine(0, y, SCREEN_WIDTH, y, bgColor);  // ✅
    }

    // 半透明遮罩
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.5));  // ✅

    // 胜利标题（居中）
    const char* title = "YOU WIN!";
    int titleSize = 80;
    int titleWidth = MeasureText(title, titleSize);
    int titleX = (SCREEN_WIDTH - titleWidth) / 2;  // ✅
    DrawText(title, titleX + 4, 404, titleSize, Fade(LIME, 0.3));
    DrawText(title, titleX, 400, titleSize, LIME);

    // 重试按钮（位置微调）
    Rectangle btn = { (SCREEN_WIDTH - 300) / 2, 700, 300, 80 };  // ✅
    Color btnColor = CheckCollisionPointRec(GetMousePosition(), btn) ? GRAY : DARKGRAY;
    Color btnBorderColor = CheckCollisionPointRec(GetMousePosition(), btn) ? LIME : Fade(LIME, 0.7);
    DrawRectangle(btn.x, btn.y, btn.width, btn.height, btnColor);
    DrawRectangleLinesEx(btn, 3, btnBorderColor);
    DrawText("RETRY", btn.x + 80, btn.y + 20, 32, LIGHTGRAY);

    // 点击重试返回菜单（保持键盘控制兼容性）
    if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), btn)) 
        || IsKeyPressed(KEY_R)) {  // ✅ 添加R键支持
        Game_ChangeState(GAME_STATE_MENU);
    }
}

void UI_DrawGameOverScreen(void) {
    // 渐变背景
    for (int y = 0; y < SCREEN_HEIGHT; y++) {  // ✅
        float t = (float)y / SCREEN_HEIGHT;
        Color bgColor = t < 0.5 ? DARKGRAY : BLACK;
        DrawLine(0, y, SCREEN_WIDTH, y, bgColor);  // ✅
    }

    // 半透明遮罩
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.5));  // ✅

    // 失败标题（居中）
    const char* title = "GAME OVER";
    int titleSize = 80;
    int titleWidth = MeasureText(title, titleSize);
    int titleX = (SCREEN_WIDTH - titleWidth) / 2;  // ✅
    DrawText(title, titleX + 4, 404, titleSize, Fade(RED, 0.3));
    DrawText(title, titleX, 400, titleSize, RED);

    // 重试按钮（位置微调）
    Rectangle btn = { (SCREEN_WIDTH - 300) / 2, 700, 300, 80 };  // ✅
    Color btnColor = CheckCollisionPointRec(GetMousePosition(), btn) ? GRAY : DARKGRAY;
    Color btnBorderColor = CheckCollisionPointRec(GetMousePosition(), btn) ? RED : Fade(RED, 0.7);
    DrawRectangle(btn.x, btn.y, btn.width, btn.height, btnColor);
    DrawRectangleLinesEx(btn, 3, btnBorderColor);
    DrawText("RETRY", btn.x + 80, btn.y + 20, 32, LIGHTGRAY);

    // 点击重试返回菜单（保持键盘控制兼容性）
    if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), btn)) 
        || IsKeyPressed(KEY_R)) {  // ✅ 添加R键支持
        Game_ChangeState(GAME_STATE_MENU);
    }
}