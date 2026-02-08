@echo off
echo Building test_correlation...
g++ -std=c++11 -D WINDOWS -o test_correlation.exe test_correlation.cpp ../source/SignalProcessing.cpp -I../source
if %ERRORLEVEL% == 0 (
    echo Build successful! Running test...
    echo.
    test_correlation.exe
) else (
    echo Build failed!
)
