@echo off
REM Build script for ML HDF5 export test (Windows)

echo Building test_ml_h5export.exe...

REM Note: Requires HDF5 C++ library installed
REM Set HDF5_DIR to your HDF5 installation path if needed
REM Example: set HDF5_DIR=C:\Program Files\HDF5\1.12.0

g++ -std=c++11 -DWINDOWS -DUSE_HDF5 -o test_ml_h5export.exe test_ml_h5export.cpp ../source/SignalProcessing.cpp -I../source -lhdf5_cpp -lhdf5

if %ERRORLEVEL% EQU 0 (
    echo Build successful! Run with: test_ml_h5export.exe
    echo.
    echo Note: Make sure HDF5 libraries are in your PATH
) else (
    echo Build failed!
    echo.
    echo Make sure HDF5 C++ library is installed and accessible.
    echo You may need to specify include/lib paths manually:
    echo   -I"path\to\hdf5\include" -L"path\to\hdf5\lib"
    exit /b 1
)
