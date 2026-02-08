@echo off
REM Build script for test_ml_features on Windows

echo Building test_ml_features...

g++ -std=c++11 -DWINDOWS -o test_ml_features.exe test_ml_features.cpp ../source/SignalProcessing.cpp -I../source

if %errorlevel% equ 0 (
    echo Build successful!
    echo Run with: test_ml_features.exe
) else (
    echo Build failed!
    exit /b 1
)
