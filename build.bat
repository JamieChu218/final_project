@echo off
REM ============================================================
REM  build.bat - compile and run the Store Inventory System
REM  Just double-click this file, or run it from a command prompt.
REM ============================================================
chcp 65001 >nul
cd /d "%~dp0"

REM Use g++ from PATH if available, otherwise fall back to C:\mingw64\bin
set "GPP=g++"
where g++ >nul 2>nul || set "GPP=C:\mingw64\bin\g++"

echo [1/2] Compiling with "%GPP%" ...
"%GPP%" -std=c++14 -Isrc src\Product.cpp src\Food.cpp src\Electronics.cpp src\Clothing.cpp src\Inventory.cpp src\main.cpp -o inventory.exe

if not exist inventory.exe (
    echo.
    echo [BUILD FAILED] inventory.exe was not produced.
    echo  - Make sure g++ exists. Expected: C:\mingw64\bin\g++.exe
    echo  - Or add your MinGW bin folder to PATH.
    echo.
    pause
    goto :eof
)

echo [2/2] Build OK. Starting program...
echo.
inventory.exe

echo.
echo (Program ended.)
pause
