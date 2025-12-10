@echo off
cd /d "C:\Users\lenovo\Desktop\WazeGame\src"

set PATH=C:\mingw64\bin;%PATH%
set RAYLIB=C:\Users\lenovo\Desktop\raylib-4.5.0_win64_mingw-w64

gcc *.c -o ../game.exe ^
    -I"%RAYLIB%\include" ^
    -L"%RAYLIB%\lib" ^
    -lraylib -lopengl32 -lgdi32 -lwinmm

if exist ..\game.exe (
    copy "%RAYLIB%\lib\raylib.dll" ..\
    cd ..
    game.exe
)
pause