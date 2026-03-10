@echo off
REM Build script for ML Downstream integration test (Windows)

echo Building test_ml_downstream.exe...

g++ -std=c++11 -DWINDOWS -o test_ml_downstream.exe test_ml_downstream.cpp ../source/SignalProcessing.cpp -I../source

if %ERRORLEVEL% EQU 0 (
    echo Build successful! Run with: test_ml_downstream.exe
) else (
    echo Build failed!
    exit /b 1
)
