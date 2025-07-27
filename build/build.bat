@REM Build for Visual Studio compiler. Run your copy of vcvars32.bat or vcvarsall.bat to setup command-line compiler.
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"  
@REM gcc SignalProcessing.cpp test.cpp -o test.exe --verbose
@set OUT_DIR=Debug
@set OUT_EXE=test
@set INCLUDES=/I "%DXSDK_DIR%/Include"
@set SOURCES= test.cpp SignalProcessing.cpp
@set LIBS=/LIBPATH:"%DXSDK_DIR%/Lib/x86" d3d9.lib
mkdir %OUT_DIR%
cl /nologo /Zi /MD /utf-8 %INCLUDES% /D UNICODE /D _UNICODE %SOURCES% /Fe%OUT_DIR%/%OUT_EXE%.exe /Fo%OUT_DIR%/ /link %LIBS%
                                                                                                                                