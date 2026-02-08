@echo off
REM Build script for test_frequency_analysis on Windows

echo Building test_frequency_analysis...

g++ -std=c++11 -DWINDOWS -o test_frequency_analysis.exe test_frequency_analysis.cpp ../source/SignalProcessing.cpp -I../source

if %errorlevel% equ 0 (
    echo Build successful!
    echo Run with: test_frequency_analysis.exe
) else (
    echo Build failed!
    exit /b 1
)
