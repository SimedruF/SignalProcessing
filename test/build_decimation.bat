@echo off
echo Building test_decimation...
g++ -std=c++11 -D WINDOWS -o test_decimation.exe test_decimation.cpp ../source/SignalProcessing.cpp -I../source
if %ERRORLEVEL% == 0 (
    echo Build successful! Running test...
    echo.
    test_decimation.exe
) else (
    echo Build failed!
)
