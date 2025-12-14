// ui.c - 游戏所有界面的绘制代码
#include "game.h"  // 引入游戏公共数据和函数声明

// 界面布局参数 
// 右侧信息面板的位置和大小
#define UI_PANEL_X 1700      // 面板在屏幕上的X坐标（横向位置）
#define UI_PANEL_Y 50        // 面板在屏幕上的Y坐标（纵向位置）
#define UI_PANEL_W 280       // 面板的宽度
#define UI_PANEL_H 320       // 面板的高度
// 血条的尺寸
#define HP_BAR_W 240         // 血条的宽度
#define HP_BAR_H 20          // 血条的高度
// 文字大小
#define UI_TITLE_SIZE 28     // 面板标题的字体大小
#define UI_TEXT_SIZE 24      // 面板正文的字体大小
#define UI_SMALL_TEXT_SIZE 20 // 面板小字体的大小

// 内部工具函数
// 功能：画一个渐变颜色的血条
// 参数说明：
// x,y = 血条在屏幕上的左上角坐标
// width,height = 血条的宽和高
// currentHp = 玩家当前生命值
// maxHp = 玩家最大生命值
static void DrawGradientHealthBar(int x, int y, int width, int height, int currentHp, int maxHp) {
    // 第一步：画血条的灰色背景
    DrawRectangle(x, y, width, height, DARKGRAY);
    // 第二步：给血条画灰色的边框
    DrawRectangleLines(x, y, width, height, GRAY);

    // 计算当前血量占最大血量的百分比（比如当前50血，最大100血，就是0.5）
    float hpPercent = (float)currentHp / maxHp;
    // 根据百分比算出实际要显示的血条宽度（比如宽240的血条，50%就是120）
    int barWidth = (int)(width * hpPercent);
    // 如果血量为0，就不用画彩色部分了，直接退出函数
    // 如果血量为0，就不用画彩色部分，直接退出函数
    if (barWidth <= 0) return;

    // 根据血量多少设置血条的渐变颜色
    Color startColor, endColor;
    if (currentHp >= maxHp) {  // 满血状态：绿色渐变
        startColor = LIME;      // 亮绿色
        endColor = DARKGREEN;   // 深绿色
    }
    else if (currentHp > maxHp / 2) {  // 血量过半：黄色渐变
        startColor = YELLOW;    // 黄色
        endColor = GOLD;        // 金色
    }
    else {  // 血量不足一半：红色渐变
        startColor = RED;       // 红色
        endColor = MAROON;      // 深红色
    }

    // 逐像素画渐变效果（把血条分成一个个像素，每个像素颜色慢慢变化）
    for (int i = 0; i < barWidth; i++) {
        // 计算当前像素在血条中的比例（0到1）
        float t = (float)i / barWidth;
        // 计算当前像素的颜色（从startColor过渡到endColor）
        Color color = {
            (unsigned char)(startColor.r - t * (startColor.r - endColor.r)),
            (unsigned char)(startColor.g - t * (startColor.g - endColor.g)),
            (unsigned char)(startColor.b - t * (startColor.b - endColor.b)),
            255  // 不透明
        };
        // 画一条竖线（一个像素宽），拼出血条
        DrawLine(x + i, y, x + i, y + height, color);
    }
}

// 主界面绘制函数

// 功能：绘制游戏主菜单界面
void UI_DrawMenu(void) {
    // 画渐变背景（上半部分深灰色，下半部分黑色）
    // 遍历屏幕的每一行像素
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        // 计算当前行在屏幕高度中的比例（0到1）
        float t = (float)y / SCREEN_HEIGHT;
        // 上半屏用深灰色，下半屏用黑色
        Color bgColor = t < 0.5 ? DARKGRAY : BLACK;
        // 画一行像素（横向整行）
        DrawLine(0, y, SCREEN_WIDTH, y, bgColor);
    }

    // 绘制游戏标题 "MAZE GAME"
    const char* title = "MAZE GAME";
    int titleSize = 72;  // 标题字体大小
    // 计算标题文字的宽度（方便居中）
    int titleWidth = MeasureText(title, titleSize);
    // 计算标题的X坐标（让标题在屏幕水平居中）
    int titleX = (SCREEN_WIDTH - titleWidth) / 2;
    // 先画暗红色阴影（让标题有立体感）
    DrawText(title, titleX + 4, 304, titleSize, MAROON);
    // 再画红色标题（叠在阴影上）
    DrawText(title, titleX, 300, titleSize, RED);

    // 绘制开始游戏按钮
    // 按钮位置：水平居中，Y坐标600，宽300，高80
    Rectangle btn = { (SCREEN_WIDTH - 300) / 2, 600, 300, 80 };
    // 按钮颜色：鼠标移上去是灰色，否则是深灰色
    Color btnColor = CheckCollisionPointRec(GetMousePosition(), btn) ? GRAY : DARKGRAY;
    // 按钮边框颜色：鼠标移上去是红色，否则是暗红色
    Color btnBorderColor = CheckCollisionPointRec(GetMousePosition(), btn) ? RED : MAROON;

    // 画按钮的底色
    DrawRectangle(btn.x, btn.y, btn.width, btn.height, btnColor);
    // 画按钮的边框（线宽3像素）
    DrawRectangleLinesEx(btn, 3, btnBorderColor);
    // 按钮上的文字 "START GAME"
    const char* btnText = "START GAME";
    // 计算按钮文字的宽度（方便居中）
    int btnTextWidth = MeasureText(btnText, 32);
    // 把文字画在按钮正中间
    DrawText(btnText, btn.x + (btn.width - btnTextWidth) / 2, btn.y + 20, 32, LIGHTGRAY);

    // 点击按钮时，切换到游戏运行状态
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), btn)) {
        Game_ChangeState(GAME_STATE_PLAYING);
    }
}

// 功能：绘制游戏中的信息面板（血条、分数、音量等）
void UI_DrawHUD(void) {
    // 获取玩家当前的生命值
    int playerHealth = Player_GetHealth();

    // 绘制右侧信息面板的底色（半透明深灰色）
    DrawRectangle(UI_PANEL_X, UI_PANEL_Y, UI_PANEL_W, UI_PANEL_H, Fade(DARKGRAY, 0.95));
    // 给面板画灰色边框
    DrawRectangleLines(UI_PANEL_X, UI_PANEL_Y, UI_PANEL_W, UI_PANEL_H, GRAY);

    // 绘制面板标题 "STATUS"
    DrawText("STATUS", UI_PANEL_X + 15, UI_PANEL_Y + 15, UI_TITLE_SIZE, RED);
    // 标题下方画一条分隔线（暗红色）
    DrawLine(UI_PANEL_X + 15, UI_PANEL_Y + 45, UI_PANEL_X + UI_PANEL_W - 15, UI_PANEL_Y + 45, MAROON);

    // 绘制分数文字和数值
    DrawText("Score: ", UI_PANEL_X + 25, UI_PANEL_Y + 100, UI_TEXT_SIZE, LIGHTGRAY);
    // 显示玩家当前分数（从玩家数据中获取）
    DrawText(TextFormat("%d", Player_GetScore()), UI_PANEL_X + 100, UI_PANEL_Y + 100, UI_TEXT_SIZE, GOLD);

    // 绘制道具数量文字和数值
    DrawText("Items: ", UI_PANEL_X + 25, UI_PANEL_Y + 140, UI_TEXT_SIZE, LIGHTGRAY);
    DrawText("0", UI_PANEL_X + 110, UI_PANEL_Y + 140, UI_TEXT_SIZE, YELLOW);

    // 绘制陷阱状态文字和数值
    DrawText("Trap: ", UI_PANEL_X + 25, UI_PANEL_Y + 180, UI_TEXT_SIZE, LIGHTGRAY);
    DrawText("NO", UI_PANEL_X + 100, UI_PANEL_Y + 180, UI_TEXT_SIZE, LIME);

    // 定义玩家最大生命值（和游戏全局设置一致）
    #define PLAYER_MAX_HEALTH 100
    // 绘制渐变血条
    DrawGradientHealthBar(UI_PANEL_X + 25, UI_PANEL_Y + 220, HP_BAR_W, HP_BAR_H, 
                         playerHealth, PLAYER_MAX_HEALTH);

    // 绘制生命值文字（比如 HP: 80/100）
    DrawText(TextFormat("HP: %d/%d", playerHealth, PLAYER_MAX_HEALTH), 
            UI_PANEL_X + 25, UI_PANEL_Y + 250, UI_SMALL_TEXT_SIZE, GRAY);

    // 绘制音乐音量（左下角）
    DrawText(TextFormat("音乐音量：%.0f%%", Audio_GetMusicVolume()*100), 
             20, SCREEN_HEIGHT - 70, 20, LIGHTGRAY);
    // 绘制音效音量（左下角）
    DrawText(TextFormat("音效音量：%.0f%%", Audio_GetSfxVolume()*100), 
             20, SCREEN_HEIGHT - 40, 20, LIGHTGRAY);
    // 绘制音量调节提示（左下角）
    DrawText("↑↓调节音乐 | ←→调节音效", 20, SCREEN_HEIGHT - 100, 20, Fade(WHITE, 0.7f));
}

// 功能：绘制游戏暂停界面
void UI_DrawPause(void) {
    // 画半透明黑色覆盖层（让游戏画面变暗）
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 150});

    // 绘制 "GAME PAUSED" 文字（居中显示）
    const char* pauseText = "GAME PAUSED";
    // 计算文字宽度（方便居中）
    int textWidth = MeasureText(pauseText, 60);
    // 文字位置：水平居中，垂直在屏幕中间偏上50像素
    DrawText(pauseText, (SCREEN_WIDTH - textWidth)/2, SCREEN_HEIGHT/2 - 50, 60, YELLOW);

    // 绘制继续游戏的提示文字
    const char* continueText = "Press P to continue";
    // 计算提示文字宽度（方便居中）
    int contWidth = MeasureText(continueText, 30);
    // 提示文字位置：水平居中，垂直在暂停文字下方20像素
    DrawText(continueText, (SCREEN_WIDTH - contWidth)/2, SCREEN_HEIGHT/2 + 20, 30, WHITE);
}

// 功能：绘制游戏胜利界面
void UI_DrawWinScreen(void) {
    // 画渐变背景（上半部分深灰色，下半部分黑色）
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        float t = (float)y / SCREEN_HEIGHT;
        Color bgColor = t < 0.5 ? DARKGRAY : BLACK;
        DrawLine(0, y, SCREEN_WIDTH, y, bgColor);
    }

    // 画半透明黑色遮罩（让背景变暗）
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.5));

    // 绘制胜利标题 "YOU WIN!"
    const char* title = "YOU WIN!";
    int titleSize = 80;
    // 计算标题宽度（方便居中）
    int titleWidth = MeasureText(title, titleSize);
    // 标题水平居中
    int titleX = (SCREEN_WIDTH - titleWidth) / 2;
    // 先画浅绿色阴影（有立体感）
    DrawText(title, titleX + 4, 404, titleSize, Fade(LIME, 0.3));
    // 再画浅绿色标题（叠在阴影上）
    DrawText(title, titleX, 400, titleSize, LIME);

    // 绘制重试按钮
    // 按钮位置：水平居中，Y坐标700，宽300，高80
    Rectangle btn = { (SCREEN_WIDTH - 300) / 2, 700, 300, 80 };
    // 按钮颜色：鼠标移上去是灰色，否则是深灰色
    Color btnColor = CheckCollisionPointRec(GetMousePosition(), btn) ? GRAY : DARKGRAY;
    // 按钮边框颜色：鼠标移上去是浅绿色，否则是浅一点的浅绿色
    Color btnBorderColor = CheckCollisionPointRec(GetMousePosition(), btn) ? LIME : Fade(LIME, 0.7);
    // 画按钮底色
    DrawRectangle(btn.x, btn.y, btn.width, btn.height, btnColor);
    // 画按钮边框（线宽3像素）
    DrawRectangleLinesEx(btn, 3, btnBorderColor);
    // 按钮上的文字 "RETRY"
    DrawText("RETRY", btn.x + 80, btn.y + 20, 32, LIGHTGRAY);

    // 点击按钮或按R键，返回游戏主菜单
    if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), btn)) 
        || IsKeyPressed(KEY_R)) {
        Game_ChangeState(GAME_STATE_MENU);
    }
}

// 功能：绘制游戏失败界面
void UI_DrawGameOverScreen(void) {
    // 画渐变背景（上半部分深灰色，下半部分黑色）
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        float t = (float)y / SCREEN_HEIGHT;
        Color bgColor = t < 0.5 ? DARKGRAY : BLACK;
        DrawLine(0, y, SCREEN_WIDTH, y, bgColor);
    }

    // 画半透明黑色遮罩（让背景变暗）
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.5));

    // 绘制失败标题 "GAME OVER"
    const char* title = "GAME OVER";
    int titleSize = 80;
    // 计算标题宽度（方便居中）
    int titleWidth = MeasureText(title, titleSize);
    // 标题水平居中
    int titleX = (SCREEN_WIDTH - titleWidth) / 2;
    // 先画红色阴影（有立体感）
    DrawText(title, titleX + 4, 404, titleSize, Fade(RED, 0.3));
    // 再画红色标题（叠在阴影上）
    DrawText(title, titleX, 400, titleSize, RED);

    // 绘制重试按钮
    // 按钮位置：水平居中，Y坐标700，宽300，高80
    Rectangle btn = { (SCREEN_WIDTH - 300) / 2, 700, 300, 80 };
    // 按钮颜色：鼠标移上去是灰色，否则是深灰色
    Color btnColor = CheckCollisionPointRec(GetMousePosition(), btn) ? GRAY : DARKGRAY;
    // 按钮边框颜色：鼠标移上去是红色，否则是浅一点的红色
    Color btnBorderColor = CheckCollisionPointRec(GetMousePosition(), btn) ? RED : Fade(RED, 0.7);
    // 画按钮底色
    DrawRectangle(btn.x, btn.y, btn.width, btn.height, btnColor);
    // 画按钮边框（线宽3像素）
    DrawRectangleLinesEx(btn, 3, btnBorderColor);
    // 按钮上的文字 "RETRY"
    DrawText("RETRY", btn.x + 80, btn.y + 20, 32, LIGHTGRAY);

    // 点击按钮或按R键，返回游戏主菜单
    if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), btn)) 
        || IsKeyPressed(KEY_R)) {
        Game_ChangeState(GAME_STATE_MENU);
    }
}