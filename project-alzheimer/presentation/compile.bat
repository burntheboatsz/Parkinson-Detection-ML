@echo off
REM Script untuk compile Beamer LaTeX presentation
REM Requirements: MiKTeX atau TeX Live terinstall

echo ========================================
echo Compiling Beamer LaTeX Presentation
echo ========================================
echo.

cd /d "%~dp0"

REM Check if pdflatex exists
where pdflatex >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: pdflatex tidak ditemukan!
    echo.
    echo Silakan install LaTeX distribution:
    echo 1. MiKTeX: https://miktex.org/download
    echo 2. TeX Live: https://www.tug.org/texlive/
    echo.
    echo ATAU compile online di: https://www.overleaf.com
    pause
    exit /b 1
)

echo [1/3] First compilation...
pdflatex -interaction=nonstopmode parkinson_detection_presentation.tex

echo.
echo [2/3] Second compilation (untuk references)...
pdflatex -interaction=nonstopmode parkinson_detection_presentation.tex

echo.
echo [3/3] Cleaning auxiliary files...
del /Q *.aux *.log *.nav *.out *.snm *.toc *.vrb 2>nul

echo.
echo ========================================
echo COMPILATION COMPLETE!
echo ========================================
echo.
echo Output: parkinson_detection_presentation.pdf
echo.
echo Buka file PDF untuk melihat hasil presentasi.
echo.
pause
