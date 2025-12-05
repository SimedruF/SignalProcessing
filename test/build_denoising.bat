@echo off
REM Build script for test_denoising on Windows

echo Building test_denoising...

g++ -std=c++11 -DWINDOWS -o test_denoising.exe test_denoising.cpp ../source/SignalProcessing.cpp -I../source

if %errorlevel% equ 0 (
    echo Build successful!
    echo Run with: test_denoising.exe
) else (
    echo Build failed!
    exit /b 1
)
