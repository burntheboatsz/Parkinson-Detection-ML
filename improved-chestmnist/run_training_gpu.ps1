# Training Script dengan GPU
# Jalankan dengan: .\run_training_gpu.ps1

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Training ChestMNIST dengan GPU" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Path ke Python di virtual environment
$pythonPath = "D:\vscode\improved-chestmnist\.venv311\Scripts\python.exe"
$projectPath = "D:\vscode\improved-chestmnist\chest-mnist-classification"

# Cek apakah Python ada
if (-not (Test-Path $pythonPath)) {
    Write-Host "ERROR: Python virtual environment tidak ditemukan!" -ForegroundColor Red
    Write-Host "Path: $pythonPath" -ForegroundColor Yellow
    exit 1
}

# Verifikasi GPU
Write-Host "Mengecek GPU..." -ForegroundColor Yellow
& $pythonPath -c "import torch; print('CUDA Available:', torch.cuda.is_available()); print('GPU:', torch.cuda.get_device_name(0) if torch.cuda.is_available() else 'N/A')"

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: Gagal mengecek GPU!" -ForegroundColor Red
    exit 1
}

Write-Host ""
Write-Host "Memulai training..." -ForegroundColor Green
Write-Host ""

# Pindah ke folder project dan jalankan training
Set-Location $projectPath
& $pythonPath train.py

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Training Selesai!" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
