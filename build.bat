@echo off
REM ============================================================
REM  build.bat — 一鍵編譯並啟動「商店庫存管理系統」
REM  使用方式：直接雙擊本檔，或在命令提示字元執行 build.bat
REM ============================================================
chcp 65001 >nul
cd /d "%~dp0"

REM 優先使用 PATH 中的 g++；若找不到，改用 C:\mingw64\bin\g++
set "GPP=g++"
where g++ >nul 2>nul || set "GPP=C:\mingw64\bin\g++"

echo [1/2] 編譯中...
"%GPP%" -std=c++14 -Isrc src\Product.cpp src\Food.cpp src\Electronics.cpp src\Clothing.cpp src\Inventory.cpp src\main.cpp -o inventory.exe
if errorlevel 1 (
    echo.
    echo [編譯失敗] 請確認已安裝 g++ 編譯器 ^(MinGW-w64^)。
    echo            預期路徑：C:\mingw64\bin\g++.exe
    pause
    exit /b 1
)

echo [2/2] 編譯成功，啟動程式...
echo.
inventory.exe

echo.
echo （程式已結束）
pause
