# 🚀 Setup GPU untuk Training

## ✅ Status GPU
- **GPU**: NVIDIA GeForce RTX 3050 Laptop GPU
- **CUDA Version**: 13.0
- **PyTorch CUDA**: 11.8
- **Python Version**: 3.11.9

---

## 📦 Virtual Environment dengan GPU

Virtual environment yang **sudah dikonfigurasi dengan GPU**:
```
D:\vscode\improved-chestmnist\.venv311
```

### Python Interpreter:
```
D:\vscode\improved-chestmnist\.venv311\Scripts\python.exe
```

---

## 🔧 Setup Pertama Kali (Sudah Selesai ✅)

Jika perlu setup ulang di komputer lain:

### 1. Buat Virtual Environment dengan Python 3.11
```powershell
py -3.11 -m venv .venv311
```

### 2. Aktivasi Virtual Environment
```powershell
.venv311\Scripts\Activate.ps1
```

### 3. Install PyTorch dengan CUDA 11.8
```powershell
pip install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu118
```

### 4. Install Dependencies Lainnya
```powershell
pip install -r chest-mnist-classification/requirements.txt
```

### 5. Verifikasi GPU
```powershell
python -c "import torch; print('CUDA:', torch.cuda.is_available()); print('GPU:', torch.cuda.get_device_name(0) if torch.cuda.is_available() else 'N/A')"
```

---

## 🎯 Cara Menggunakan GPU untuk Training

### **Opsi 1: Dengan Aktivasi Virtual Environment**
```powershell
# Aktivasi venv
D:\vscode\improved-chestmnist\.venv311\Scripts\Activate.ps1

# Pindah ke folder project
cd D:\vscode\improved-chestmnist\chest-mnist-classification

# Jalankan training (GPU otomatis terdeteksi)
python train.py
```

### **Opsi 2: Langsung Tanpa Aktivasi**
```powershell
cd D:\vscode\improved-chestmnist\chest-mnist-classification
D:\vscode\improved-chestmnist\.venv311\Scripts\python.exe train.py
```

### **Opsi 3: Dari VS Code**
1. Tekan `Ctrl+Shift+P`
2. Ketik: `Python: Select Interpreter`
3. Pilih: `D:\vscode\improved-chestmnist\.venv311\Scripts\python.exe`
4. Jalankan `train.py` dari VS Code

Output akan menampilkan:
```
Menggunakan device: cuda
GPU: NVIDIA GeForce RTX 3050 Laptop GPU
```

---

## 📋 Package yang Terinstall

```
torch==2.7.1+cu118
torchvision==0.22.1+cu118
torchaudio==2.7.1+cu118
```

Plus semua dependencies dari `requirements.txt`

---

## ⚠️ Troubleshooting

### Jika GPU tidak terdeteksi:
1. Pastikan menggunakan `.venv311`
2. Cek PyTorch CUDA:
   ```powershell
   D:\vscode\improved-chestmnist\.venv311\Scripts\python.exe -m pip list | Select-String "torch"
   ```
   Harus ada `+cu118` di versinya

3. Jika torch versi CPU terinstall:
   ```powershell
   D:\vscode\improved-chestmnist\.venv311\Scripts\python.exe -m pip uninstall torch torchvision torchaudio -y
   D:\vscode\improved-chestmnist\.venv311\Scripts\python.exe -m pip install torch torchvision torchaudio --index-url https://download.pytorch.org/whl/cu118
   ```

### Jika error `fbgemm.dll`:
Install Microsoft Visual C++ Redistributable:
```
https://aka.ms/vs/17/release/vc_redist.x64.exe
```

---

## 💾 Konfigurasi Tersimpan

Konfigurasi GPU **sudah permanen** dalam:
- ✅ Virtual environment `.venv311`
- ✅ PyTorch CUDA packages
- ✅ Code `train.py` (auto-detect GPU)

**Tidak perlu setup ulang setiap kali!** GPU akan otomatis terdeteksi selama menggunakan `.venv311`.

---

## 🎉 Hasil Training dengan GPU

Training terakhir dengan GPU:
- Training Accuracy: 80.18%
- Validation Accuracy: 78.15%
- Device: NVIDIA GeForce RTX 3050 Laptop GPU
- Status: ✅ Success
