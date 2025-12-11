@echo off
REM Build script for ImGui demo on Windows with g++

echo ========================================
echo Building SignalProcessing ImGui Demo
echo ========================================
echo.

REM Check if imgui exists
if not exist "imgui" (
    echo ERROR: ImGui not found!
    echo Please run setup_imgui.bat first
    echo.
    pause
    exit /b 1
)

REM Check if implot exists
if not exist "implot" (
    echo ERROR: ImPlot not found!
    echo Please run setup_imgui.bat first
    echo.
    pause
    exit /b 1
)

echo Step 1: Checking for GLFW...
echo.
echo GLFW is required for this demo.
echo.
echo Install options:
echo   1. Using MSYS2 (recommended):
echo      pacman -S mingw-w64-x86_64-glfw
echo.
echo   2. Using vcpkg:
echo      vcpkg install glfw3:x64-windows
echo.
echo   3. Manual download from: https://www.glfw.org/download.html
echo.
echo After installing GLFW, make sure it's in your PATH or specify location below.
echo.

REM Try to compile
echo Step 2: Attempting to compile...
echo.

g++ -std=c++11 -DWINDOWS -o imgui_demo.exe ^
    imgui_demo.cpp ^
    ../source/SignalProcessing.cpp ^
    imgui/imgui.cpp ^
    imgui/imgui_draw.cpp ^
    imgui/imgui_tables.cpp ^
    imgui/imgui_widgets.cpp ^
    imgui/backends/imgui_impl_glfw.cpp ^
    imgui/backends/imgui_impl_opengl3.cpp ^
    implot/implot.cpp ^
    implot/implot_items.cpp ^
    -I../source ^
    -Iimgui ^
    -Iimgui/backends ^
    -Iimplot ^
    -lglfw3 ^
    -lopengl32 ^
    -lgdi32

if %errorlevel% equ 0 (
    echo.
    echo ========================================
    echo Build successful!
    echo ========================================
    echo.
    echo Run with: imgui_demo.exe
    echo.
) else (
    echo.
    echo ========================================
    echo Build failed!
    echo ========================================
    echo.
    echo Common issues:
    echo   1. GLFW not installed - install via MSYS2 or vcpkg
    echo   2. Wrong compiler - make sure you're using MinGW g++
    echo   3. Missing dependencies - install OpenGL development files
    echo.
    echo For MSYS2 users, run in MSYS2 MinGW terminal:
    echo   pacman -S mingw-w64-x86_64-gcc
    echo   pacman -S mingw-w64-x86_64-glfw
    echo.
    exit /b 1
)

pause
