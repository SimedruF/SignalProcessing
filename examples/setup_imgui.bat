@echo off
echo Setting up ImGui Demo for SignalProcessing
echo ===========================================
echo.

REM Check if git is installed
git --version >nul 2>&1
if %errorlevel% neq 0 (
    echo Error: git is not installed. Please install git first.
    exit /b 1
)

REM Clone ImGui if not present
if not exist "imgui" (
    echo Cloning ImGui...
    git clone --depth 1 https://github.com/ocornut/imgui.git
    echo ✓ ImGui cloned successfully
) else (
    echo ✓ ImGui already exists
)

REM Clone ImPlot if not present
if not exist "implot" (
    echo Cloning ImPlot...
    git clone --depth 1 https://github.com/epezent/implot.git
    echo ✓ ImPlot cloned successfully
) else (
    echo ✓ ImPlot already exists
)

echo.
echo Checking dependencies...
echo You need to install GLFW3 and OpenGL
echo.
echo Recommended: Use vcpkg
echo   vcpkg install glfw3:x64-windows
echo   vcpkg integrate install
echo.
echo Or download GLFW from: https://www.glfw.org/download.html
echo.

REM Create build directory
if not exist "build" mkdir build

echo.
echo Setup complete!
echo.
echo To build with CMake:
echo   cd build
echo   cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake
echo   cmake --build .
echo   .\Debug\imgui_demo.exe
echo.
echo Or use Visual Studio to open CMakeLists.txt directly
echo.

pause
