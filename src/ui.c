#include "game.h"
#include "raylib.h"
#include <stdbool.h>
// -------------------------- 你的宏定义（完全不变） --------------------------
#define WINDOW_W 2000
#define WINDOW_H 1200
#define MAX_HP 3
#define UI_PANEL_X 1700
#define UI_PANEL_Y 50
#define UI_PANEL_W 280
#define UI_PANEL_H 320
#define HP_BAR_W 240
#define HP_BAR_H 20
#define UI_TITLE_SIZE 28
#define UI_TEXT_SIZE 24
#define UI_SMALL_TEXT_SIZE 20
// -------------------------- 内部工具函数（渐变血条） --------------------------
static void DrawGradientHealthBar(int x, int y, int width, int height, int currentHp, int maxHp) {
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

// -------------------------- UI核心函数实现（完全不变） --------------------------
void UI_DrawMenu(void) {
    // 渐变背景
    for (int y = 0; y < WINDOW_H; y++) {
        float t = (float)y / WINDOW_H;
        Color bgColor = t < 0.5 ? DARKGRAY : BLACK;
        DrawLine(0, y, WINDOW_W, y, bgColor);
    }

    // 标题
    const char* title = "MAZE GAME";
    int titleSize = 72;
    int titleWidth = MeasureText(title, titleSize);
    int titleX = (WINDOW_W - titleWidth) / 2;
    DrawText(title, titleX + 4, 304, titleSize, MAROON);
    DrawText(title, titleX, 300, titleSize, RED);

    // 开始按钮
    Rectangle btn = { (WINDOW_W - 300) / 2, 600, 300, 80 };
    Color btnColor = CheckCollisionPointRec(GetMousePosition(), btn) ? GRAY : DARKGRAY;
    Color btnBorderColor = CheckCollisionPointRec(GetMousePosition(), btn) ? RED : MAROON;

    DrawRectangle(btn.x, btn.y, btn.width, btn.height, btnColor);
    DrawRectangleLinesEx(btn, 3, btnBorderColor);
    const char* btnText = "START GAME";
    int btnTextWidth = MeasureText(btnText, 32);
    DrawText(btnText, btn.x + (btn.width - btnTextWidth) / 2, btn.y + 20, 32, LIGHTGRAY);

    // 点击按钮切换状态
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), btn)) {
        Game_ChangeState(GAME_STATE_PLAYING);
    }
}

void UI_DrawHUD(void) {
    // 绘制UI面板
    DrawRectangle(UI_PANEL_X, UI_PANEL_Y, UI_PANEL_W, UI_PANEL_H, Fade(DARKGRAY, 0.95));
    DrawRectangleLines(UI_PANEL_X, UI_PANEL_Y, UI_PANEL_W, UI_PANEL_H, GRAY);

    // 标题栏
    DrawText("STATUS", UI_PANEL_X + 15, UI_PANEL_Y + 15, UI_TITLE_SIZE, RED);
    DrawLine(UI_PANEL_X + 15, UI_PANEL_Y + 45, UI_PANEL_X + UI_PANEL_W - 15, UI_PANEL_Y + 45, MAROON);

    // 分数
    DrawText("Souls: ", UI_PANEL_X + 25, UI_PANEL_Y + 100, UI_TEXT_SIZE, LIGHTGRAY);
    DrawText(TextFormat("%d", Player_GetScore()), UI_PANEL_X + 100, UI_PANEL_Y + 100, UI_TEXT_SIZE, GOLD);

    // 道具数量
    DrawText("Items: ", UI_PANEL_X + 25, UI_PANEL_Y + 140, UI_TEXT_SIZE, LIGHTGRAY);
    DrawText(TextFormat("%d", itemCount), UI_PANEL_X + 110, UI_PANEL_Y + 140, UI_TEXT_SIZE, YELLOW);

    // 陷阱状态
    DrawText("Trap: ", UI_PANEL_X + 25, UI_PANEL_Y + 180, UI_TEXT_SIZE, LIGHTGRAY);
    DrawText(isTrapTriggered ? "YES" : "NO", UI_PANEL_X + 100, UI_PANEL_Y + 180, UI_TEXT_SIZE, isTrapTriggered ? RED : LIME);

    // 渐变血条
    DrawGradientHealthBar(UI_PANEL_X + 25, UI_PANEL_Y + 220, HP_BAR_W, HP_BAR_H, Player_GetHealth(), MAX_HP);
    DrawText(TextFormat("HP: %d/%d", Player_GetHealth(), MAX_HP), UI_PANEL_X + 25, UI_PANEL_Y + 250, UI_SMALL_TEXT_SIZE, GRAY);
}

void UI_DrawWinScreen(void) {
    // 渐变背景
    for (int y = 0; y < WINDOW_H; y++) {
        float t = (float)y / WINDOW_H;
        Color bgColor = t < 0.5 ? DARKGRAY : BLACK;
        DrawLine(0, y, WINDOW_W, y, bgColor);
    }

    // 半透明遮罩
    DrawRectangle(0, 0, WINDOW_W, WINDOW_H, Fade(BLACK, 0.5));

    // 胜利标题
    const char* title = "YOU WIN!";
    int titleSize = 80;
    int titleWidth = MeasureText(title, titleSize);
    int titleX = (WINDOW_W - titleWidth) / 2;
    DrawText(title, titleX + 4, 404, titleSize, Fade(LIME, 0.3));
    DrawText(title, titleX, 400, titleSize, LIME);

    // 重试按钮
    Rectangle btn = { (WINDOW_W - 300) / 2, 700, 300, 80 };
    Color btnColor = CheckCollisionPointRec(GetMousePosition(), btn) ? GRAY : DARKGRAY;
    Color btnBorderColor = CheckCollisionPointRec(GetMousePosition(), btn) ? LIME : Fade(LIME, 0.7);
    DrawRectangle(btn.x, btn.y, btn.width, btn.height, btnColor);
    DrawRectangleLinesEx(btn, 3, btnBorderColor);
    DrawText("RETRY", btn.x + 80, btn.y + 20, 32, LIGHTGRAY);

    // 点击重试返回菜单
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), btn)) {
        Game_ChangeState(GAME_STATE_MENU);
    }
}

void UI_DrawGameOverScreen(void) {
    // 渐变背景
    for (int y = 0; y < WINDOW_H; y++) {
        float t = (float)y / WINDOW_H;
        Color bgColor = t < 0.5 ? DARKGRAY : BLACK;
        DrawLine(0, y, WINDOW_W, y, bgColor);
    }

    // 半透明遮罩
    DrawRectangle(0, 0, WINDOW_W, WINDOW_H, Fade(BLACK, 0.5));

    // 失败标题
    const char* title = "GAME OVER";
    int titleSize = 80;
    int titleWidth = MeasureText(title, titleSize);
    int titleX = (WINDOW_W - titleWidth) / 2;
    DrawText(title, titleX + 4, 404, titleSize, Fade(RED, 0.3));
    DrawText(title, titleX, 400, titleSize, RED);

    // 重试按钮
    Rectangle btn = { (WINDOW_W - 300) / 2, 700, 300, 80 };
    Color btnColor = CheckCollisionPointRec(GetMousePosition(), btn) ? GRAY : DARKGRAY;
    Color btnBorderColor = CheckCollisionPointRec(GetMousePosition(), btn) ? RED : Fade(RED, 0.7);
    DrawRectangle(btn.x, btn.y, btn.width, btn.height, btnColor);
    DrawRectangleLinesEx(btn, 3, btnBorderColor);
    DrawText("RETRY", btn.x + 80, btn.y + 20, 32, LIGHTGRAY);

    // 点击重试返回菜单
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), btn)) {
        Game_ChangeState(GAME_STATE_MENU);
    }
}

