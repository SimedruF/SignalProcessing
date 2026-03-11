@echo off
REM Simple script - uses pre-installed Doxygen from C:\Tools\doxygen

echo Generating documentation...
C:\Tools\doxygen\doxygen.exe Doxyfile

if %ERRORLEVEL% EQU 0 (
    echo.
    echo SUCCESS! Opening documentation...
    start docs\html\index.html
) else (
    echo ERROR: Generation failed
    pause
)
