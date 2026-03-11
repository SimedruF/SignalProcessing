@echo off
REM Quick test - generate docs without full Doxygen installation
REM This script attempts to use a portable Doxygen

echo ========================================
echo Quick Documentation Generation
echo ========================================
echo.

REM Check common portable locations
set "DOXYGEN_PATH="

if exist "C:\Tools\doxygen\doxygen.exe" (
    set "DOXYGEN_PATH=C:\Tools\doxygen\doxygen.exe"
    goto :found
)
if exist "%USERPROFILE%\Downloads\doxygen\doxygen.exe" (
    set "DOXYGEN_PATH=%USERPROFILE%\Downloads\doxygen\doxygen.exe"
    goto :found
)
if exist ".\doxygen\doxygen.exe" (
    set "DOXYGEN_PATH=.\doxygen\doxygen.exe"
    goto :found
)

REM Try system PATH
where doxygen >nul 2>nul
if %ERRORLEVEL% EQU 0 (
    set "DOXYGEN_PATH=doxygen"
    goto :found
)

:found
if not defined DOXYGEN_PATH (
    echo ERROR: Doxygen not found!
    echo.
    echo Please install Doxygen:
    echo   1. WinGet:   winget install --id=DimitriVanHeesch.Doxygen -e
    echo   2. Download: https://www.doxygen.nl/download.html
    echo.
    echo Or download portable version and extract to:
    echo   - C:\Tools\doxygen\
    echo   - %USERPROFILE%\Downloads\doxygen\
    echo   - .\doxygen\ (current directory)
    echo.
    pause
    exit /b 1
)

echo Found Doxygen: %DOXYGEN_PATH%
echo.

REM Clean previous docs
if exist docs (
    echo Cleaning previous documentation...
    rmdir /s /q docs
)

REM Generate documentation
echo Generating documentation...
"%DOXYGEN_PATH%" Doxyfile

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ====================================
    echo Documentation generated successfully!
    echo ====================================
    echo.
    echo Opening: docs\html\index.html
    start docs\html\index.html
) else (
    echo.
    echo ERROR: Documentation generation failed!
    pause
    exit /b 1
)

echo.
pause
