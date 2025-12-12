@echo off
cd /d "C:\Users\lenovo\Desktop\WazeGame\src"

set PATH=C:\mingw64\bin;%PATH%
set RAYLIB=C:\Users\lenovo\Desktop\raylib-4.5.0_win64_mingw-w64

echo 正在编译...
rem 使用这个命令来避免 Clamp 重定义
gcc -DRAYMATH_STATIC_INLINE *.c -o ../game.exe ^
    -I"%RAYLIB%\include" ^
    -I. ^
    -L"%RAYLIB%\lib" ^
    -lraylib ^
    -lopengl32 ^
    -lgdi32 ^
    -lwinmm

if %errorlevel% equ 0 (
    echo 编译成功！
    if exist "%RAYLIB%\lib\raylib.dll" (
        copy "%RAYLIB%\lib\raylib.dll" ..\
    )
    cd ..
    game.exe
) else (
    echo 编译失败！
)

pause