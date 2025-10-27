@echo off
REM =========================================
REM Script Upload Project ke GitHub
REM Author: Nafiz Ahmadin Harily
REM =========================================

echo.
echo ========================================
echo   UPLOAD PROJECT KE GITHUB
echo ========================================
echo.

REM Check if git is installed
where git >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Git belum terinstall!
    echo Silakan download di: https://git-scm.com/download/win
    pause
    exit /b 1
)

echo [1/6] Cek status repository...
git status

echo.
echo ========================================
echo.
echo Apakah Anda sudah membuat repository di GitHub?
echo (https://github.com/new)
echo.
set /p confirm="Sudah? (y/n): "

if /i not "%confirm%"=="y" (
    echo.
    echo Silakan buat repository dulu di: https://github.com/new
    echo Lalu jalankan script ini lagi.
    pause
    exit /b 0
)

echo.
echo ========================================
echo.
echo Masukkan URL repository GitHub Anda:
echo Contoh: https://github.com/nafizahmadharily/parkinson-detection-ml.git
echo.
set /p repo_url="URL: "

if "%repo_url%"=="" (
    echo [ERROR] URL tidak boleh kosong!
    pause
    exit /b 1
)

echo.
echo [2/6] Add semua file ke staging...
git add .

echo.
echo [3/6] Commit perubahan...
git commit -m "Initial commit: Parkinson Detection ML System - XGBoost 94.87%% accuracy by Nafiz Ahmadin Harily (122430051)"

echo.
echo [4/6] Setup remote repository...
git remote remove origin 2>nul
git remote add origin %repo_url%

echo.
echo [5/6] Verify remote...
git remote -v

echo.
echo [6/6] Push ke GitHub...
git branch -M main
git push -u origin main

echo.
echo ========================================
echo   SELESAI!
echo ========================================
echo.
echo Project berhasil diupload ke GitHub!
echo URL: %repo_url:~0,-4%
echo.
echo Buka browser dan cek repository Anda!
echo.
pause
