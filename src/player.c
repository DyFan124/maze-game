#include "raylib.h"

int main(void)
{
    // ��ʼ������
    const int screenWidth = 2000;
    const int screenHeight = 1200;
    InitWindow(screenWidth, screenHeight, "�����ƽ�ɫ�ƶ�");

    // ��ɫ����
    Vector2 playerPosition = { screenWidth / 2.0f, screenHeight / 2.0f }; // ��ʼλ������Ļ����
    const float playerRadius = 15.0f; // ��ɫ�뾶
    const float moveSpeed = 6.0f;     // �ƶ��ٶ�

    // ����Ŀ��֡��
    SetTargetFPS(60);

    // ����Ϸѭ��
    while (!WindowShouldClose()) // ��ⴰ�ڹرհ�ť��ESC��
    {
        // ��ȡ���λ��
        Vector2 mousePosition = GetMousePosition();

        // �����ɫ�������ķ�������
        Vector2 direction = {
            mousePosition.x - playerPosition.x,
            mousePosition.y - playerPosition.y
        };

        // ��׼������������ת��Ϊ��λ������
        float length = sqrtf(direction.x * direction.x + direction.y * direction.y);
        if (length > 0) {
            direction.x /= length;
            direction.y /= length;
        }

        // ���½�ɫλ�ã�����귽���ƶ���
        playerPosition.x += direction.x * moveSpeed;
        playerPosition.y += direction.y * moveSpeed;

        // ��ʼ����
        BeginDrawing();
        ClearBackground(RAYWHITE); // ��ձ���

        // ���ƽ�ɫ��Բ�Σ�
        DrawCircleV(playerPosition, playerRadius, RED);

        // ��ʾ����˵��
        DrawText(TextFormat("position: (%.0f, %.0f)", playerPosition.x, playerPosition.y), 10, 10, 20, DARKGRAY);

        EndDrawing();
    }

    // �رմ���
    CloseWindow();

    return 0;
}
