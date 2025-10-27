@echo off
REM =========================================
REM Script Update Repository GitHub
REM Author: Nafiz Ahmadin Harily
REM =========================================

echo.
echo ========================================
echo   UPDATE GITHUB REPOSITORY
echo ========================================
echo.

echo [1/4] Cek file yang berubah...
git status

echo.
echo ========================================
echo.
echo Masukkan deskripsi perubahan:
echo Contoh: "Add confusion matrix visualization"
echo.
set /p message="Commit message: "

if "%message%"=="" (
    set message="Update project files"
)

echo.
echo [2/4] Add perubahan...
git add .

echo.
echo [3/4] Commit dengan message: "%message%"
git commit -m "%message%"

echo.
echo [4/4] Push ke GitHub...
git push

echo.
echo ========================================
echo   SELESAI!
echo ========================================
echo.
echo Perubahan berhasil diupload!
echo.
pause
