@echo off
REM Simple build script for Visual Studio without CMake
REM This uses cl.exe (Visual Studio compiler) directly

echo ========================================
echo Building ImGui Demo with Visual Studio
echo ========================================
echo.

REM Setup Visual Studio environment
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

REM Check if imgui exists
if not exist "imgui" (
    echo ERROR: ImGui not found! Run setup_imgui.bat first
    pause
    exit /b 1
)

if not exist "implot" (
    echo ERROR: ImPlot not found! Run setup_imgui.bat first
    pause
    exit /b 1
)

REM Download GLFW pre-compiled binaries if not present
if not exist "glfw" (
    echo Downloading GLFW pre-compiled binaries...
    powershell -Command "Invoke-WebRequest -Uri 'https://github.com/glfw/glfw/releases/download/3.3.8/glfw-3.3.8.bin.WIN64.zip' -OutFile 'glfw.zip'"
    powershell -Command "Expand-Archive -Path 'glfw.zip' -DestinationPath '.'"
    move glfw-3.3.8.bin.WIN64 glfw
    del glfw.zip
    echo ✓ GLFW downloaded
)

REM Create output directory
if not exist "build" mkdir build

echo.
echo Compiling...
echo.

REM Compile all source files
cl.exe /nologo /EHsc /std:c++11 /MD /O2 ^
    /D "WINDOWS" ^
    /I "..\source" ^
    /I "imgui" ^
    /I "imgui\backends" ^
    /I "implot" ^
    /I "glfw\include" ^
    imgui_demo.cpp ^
    ..\source\SignalProcessing.cpp ^
    imgui\imgui.cpp ^
    imgui\imgui_draw.cpp ^
    imgui\imgui_tables.cpp ^
    imgui\imgui_widgets.cpp ^
    imgui\backends\imgui_impl_glfw.cpp ^
    imgui\backends\imgui_impl_opengl3.cpp ^
    implot\implot.cpp ^
    implot\implot_items.cpp ^
    /Fe"build\imgui_demo.exe" ^
    /link ^
    /LIBPATH:"glfw\lib-vc2022" ^
    glfw3.lib ^
    opengl32.lib ^
    gdi32.lib ^
    shell32.lib

if %errorlevel% equ 0 (
    echo.
    echo ========================================
    echo Build successful!
    echo ========================================
    echo.
    echo Executable: build\imgui_demo.exe
    echo.
    echo Run with: cd build ^&^& imgui_demo.exe
    echo.
) else (
    echo.
    echo ========================================
    echo Build failed!
    echo ========================================
    echo.
    pause
    exit /b 1
)

pause
