@echo off
REM Generate Doxygen documentation for SignalProcessing library

echo ====================================
echo SignalProcessing Documentation Build
echo ====================================
echo.

REM Check if Doxygen is installed
where doxygen >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Doxygen not found!
    echo.
    echo Please install Doxygen using one of these methods:
    echo.
    echo 1. Direct Download ^(Recommended^):
    echo    https://www.doxygen.nl/download.html
    echo    Download: doxygen-X.X.X-setup.exe
    echo.
    echo 2. WinGet ^(Windows Package Manager^):
    echo    winget install --id=DimitriVanHeesch.Doxygen -e
    echo.
    echo 3. Scoop:
    echo    scoop install doxygen
    echo.
    echo 4. Manual ZIP:
    echo    Extract to C:\Tools\doxygen and add to PATH
    echo.
    echo See INSTALL_DOXYGEN_WINDOWS.md for detailed instructions
    echo.
    pause
    exit /b 1
)

echo Doxygen found: 
doxygen --version
echo.

REM Check if Graphviz is installed (optional but recommended)
where dot >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo WARNING: Graphviz not found - UML diagrams will not be generated
    echo   Documentation will still work, but without visual diagrams
    echo.
    echo   To install Graphviz:
    echo     - Download: https://graphviz.org/download/
    echo     - WinGet: winget install --id=Graphviz.Graphviz -e
    echo     - Scoop: scoop install graphviz
    echo.
) else (
    echo Graphviz found - UML diagrams will be generated
    echo.
)

REM Clean previous documentation
if exist docs (
    echo Cleaning previous documentation...
    rmdir /s /q docs
)

REM Generate documentation
echo Generating documentation...
doxygen Doxyfile

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ====================================
    echo Documentation generated successfully!
    echo ====================================
    echo.
    echo Output location: docs\html\index.html
    echo.
    echo Opening documentation in browser...
    start docs\html\index.html
) else (
    echo.
    echo ERROR: Documentation generation failed!
    echo Check the warnings above.
)

echo.
pause
