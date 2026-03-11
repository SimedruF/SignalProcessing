@echo off
REM Install Graphviz manually for diagram generation

echo ================================================
echo Graphviz Installation for Doxygen Diagrams
echo ================================================
echo.

echo Step 1: Download Graphviz
echo --------------------------
echo Opening download page...
start https://gitlab.com/api/v4/projects/4207231/packages/generic/graphviz-releases/14.1.3/windows_10_cmake_Release_graphviz-install-14.1.3-win64.exe
echo.
echo Please:
echo   1. Download the installer
echo   2. Run the .exe file
echo   3. During installation, check "Add to PATH"
echo   4. Finish installation
echo   5. Close and reopen this terminal
echo.

pause

echo.
echo Step 2: Verify Installation
echo ---------------------------
where dot >nul 2>nul
if %ERRORLEVEL% EQU 0 (
    echo SUCCESS: Graphviz is installed!
    dot -V
    echo.
    echo Step 3: Regenerate Documentation
    echo --------------------------------
    echo Running docs_quick.bat to regenerate with diagrams...
    call docs_quick.bat
) else (
    echo Graphviz not found yet.
    echo Please close this terminal and open a new one after installation.
    echo Then run: docs_quick.bat
)

echo.
pause
