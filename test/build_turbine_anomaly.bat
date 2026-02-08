@echo off
REM Build script for test_turbine_anomaly on Windows

echo Building test_turbine_anomaly...

g++ -std=c++11 -DWINDOWS -o test_turbine_anomaly.exe test_turbine_anomaly.cpp ../source/SignalProcessing.cpp -I../source

if %errorlevel% equ 0 (
    echo Build successful!
    echo Run with: test_turbine_anomaly.exe
) else (
    echo Build failed!
    exit /b 1
)
