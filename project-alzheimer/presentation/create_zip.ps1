# Script untuk membuat ZIP file presentasi
# Usage: .\create_zip.ps1

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "Creating ZIP file for Overleaf Upload" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Set location to presentation folder
$presentationPath = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $presentationPath

# Output ZIP filename
$zipFileName = "parkinson_presentation.zip"
$zipPath = Join-Path $presentationPath $zipFileName

# Remove old ZIP if exists
if (Test-Path $zipPath) {
    Write-Host "Removing old ZIP file..." -ForegroundColor Yellow
    Remove-Item $zipPath -Force
}

# Create ZIP file with only .tex file
Write-Host "Creating ZIP file..." -ForegroundColor Green
Compress-Archive -Path "parkinson_detection_presentation.tex" -DestinationPath $zipPath -CompressionLevel Optimal

Write-Host ""
Write-Host "========================================" -ForegroundColor Green
Write-Host "SUCCESS!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green
Write-Host ""
Write-Host "ZIP file created: $zipFileName" -ForegroundColor White
Write-Host "Location: $zipPath" -ForegroundColor White
Write-Host ""
Write-Host "NEXT STEPS:" -ForegroundColor Cyan
Write-Host "1. Go to: https://www.overleaf.com" -ForegroundColor White
Write-Host "2. Click 'New Project' > 'Upload Project'" -ForegroundColor White
Write-Host "3. Upload file: $zipFileName" -ForegroundColor White
Write-Host "4. Wait for auto-compile (~10 seconds)" -ForegroundColor White
Write-Host "5. Download PDF!" -ForegroundColor White
Write-Host ""

# Open folder in Explorer
Write-Host "Opening folder in Explorer..." -ForegroundColor Yellow
Start-Process explorer.exe -ArgumentList $presentationPath

Write-Host ""
Write-Host "Done! File ready for upload." -ForegroundColor Green
Write-Host ""
