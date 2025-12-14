@echo off
chcp 65001 >nul
cd /d "C:\Users\lenovo\Desktop\WazeGame\src"
gcc -DRAYMATH_STATIC_INLINE *.c -o ../game.exe -I"C:\Users\lenovo\Desktop\raylib-4.5.0_win64_mingw-w64\include" -L"C:\Users\lenovo\Desktop\raylib-4.5.0_win64_mingw-w64\lib" -lraylib -lopengl32 -lgdi32 -lwinmm
cd .. && game.exe
pause