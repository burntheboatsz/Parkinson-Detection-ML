# 🚀 Peningkatan Performa Model ChestMNIST

## 📊 Hasil Sebelum vs Sesudah Peningkatan

### **Model Lama (SimpleCNN Original)**
- **Architecture:** 2 Conv layers (6, 16 filters), 3 FC layers
- **Regularization:** None
- **Epochs:** 16 (fixed)
- **Batch Size:** 16
- **Learning Rate:** 0.0003 (fixed)
- **Results:**
  - Training Acc: 80.18%
  - Validation Acc: 78.15%
  - **Problem:** Slight overfitting

### **Model Baru (Improved CNN)**
- **Architecture:** 3 Conv layers (16, 32, 64 filters), 3 FC layers
- **Regularization:** 
  - Batch Normalization (setiap layer)
  - Dropout2d (0.25 untuk conv layers)
  - Dropout (0.3 untuk FC layers)
  - L2 Weight Decay (1e-4)
- **Epochs:** 30 max (dengan early stopping)
- **Batch Size:** 32 (lebih stabil)
- **Learning Rate:** 0.001 dengan ReduceLROnPlateau scheduler
- **Early Stopping:** Patience = 7 epochs
- **Results:** *(Training sedang berlangsung...)*

---

## ✨ Peningkatan yang Dilakukan

### **1. Arsitektur Model yang Lebih Dalam**
```python
# Sebelum: 2 Conv Layers
conv1: 1 → 6 channels
conv2: 6 → 16 channels

# Sesudah: 3 Conv Layers
conv1: 1 → 16 channels
conv2: 16 → 32 channels
conv3: 32 → 64 channels
```

**Benefit:** 
- Lebih banyak filter untuk menangkap fitur kompleks
- Lebih dalam = lebih powerful feature extraction

### **2. Batch Normalization**
```python
# Ditambahkan setelah setiap conv dan FC layer
self.bn1 = nn.BatchNorm2d(16)
self.bn_fc1 = nn.BatchNorm1d(256)
```

**Benefit:**
- Stabilisasi training
- Faster convergence
- Mengurangi internal covariate shift

### **3. Dropout Regularization**
```python
# Dropout2d untuk conv layers
self.dropout_conv = nn.Dropout2d(p=0.25)

# Dropout untuk FC layers
self.dropout1 = nn.Dropout(p=0.3)
self.dropout2 = nn.Dropout(p=0.3)
```

**Benefit:**
- Mencegah overfitting
- Model lebih generalize

### **4. MaxPooling vs AvgPooling**
```python
# Sebelum: AvgPool2d
# Sesudah: MaxPool2d
self.pool = nn.MaxPool2d(2, 2)
```

**Benefit:**
- MaxPool lebih baik untuk fitur penting (edges, textures)
- Lebih robust terhadap noise

### **5. Learning Rate Scheduler**
```python
scheduler = optim.lr_scheduler.ReduceLROnPlateau(
    optimizer, mode='min', factor=0.5, patience=3, min_lr=1e-6
)
```

**Benefit:**
- Otomatis mengurangi LR saat validation loss plateau
- Membantu model converge lebih baik
- Mencegah oscillation di akhir training

### **6. Early Stopping**
```python
patience = 7 epochs
```

**Benefit:**
- Stop training saat tidak ada improvement
- Menghemat waktu
- Mencegah overfitting

### **7. L2 Regularization (Weight Decay)**
```python
optimizer = optim.Adam(model.parameters(), lr=0.001, weight_decay=1e-4)
```

**Benefit:**
- Penalti untuk weight yang terlalu besar
- Mencegah overfitting
- Model lebih smooth

### **8. Model Saving**
```python
torch.save({
    'model_state_dict': model.state_dict(),
    'best_val_acc': best_val_acc,
}, 'best_model.pth')
```

**Benefit:**
- Menyimpan model terbaik otomatis
- Bisa digunakan untuk inference nanti
- Tidak kehilangan best model jika training terlalu lama

---

## 📈 Hyperparameter Tuning

| Parameter | Lama | Baru | Alasan |
|-----------|------|------|--------|
| Epochs | 16 | 30 | Lebih banyak dengan early stopping |
| Batch Size | 16 | 32 | Lebih stabil, GPU lebih optimal |
| Learning Rate | 0.0003 | 0.001 | Start lebih tinggi, turun otomatis |
| Dropout | 0 | 0.3 | Regularisasi |
| Weight Decay | 0 | 1e-4 | L2 regularization |
| LR Scheduler | No | Yes | Adaptive learning |
| Early Stop | No | Yes (7) | Efisien & prevent overfit |

---

## 🎯 Target Peningkatan

**Target Validation Accuracy:** 82-85%

**Strategi Jika Masih Belum Optimal:**
1. ✅ Data Augmentation (flip, rotate, brightness)
2. ✅ Ensemble methods (multiple models)
3. ✅ Transfer learning (ResNet, EfficientNet)
4. ✅ Class balancing (karena Pneumothorax > Cardiomegaly)
5. ✅ Cross-validation

---

## 📝 Monitoring Training

**Indikator Training Baik:**
- ✅ Train loss & val loss turun bersama
- ✅ Gap train-val acc < 5%
- ✅ Val acc meningkat stabil
- ✅ LR turun saat plateau

**Indikator Overfitting:**
- ❌ Train acc >> Val acc (gap > 10%)
- ❌ Val loss naik sementara train loss turun
- ❌ Val acc turun di epoch akhir

**Indikator Underfitting:**
- ❌ Train acc & val acc sama-sama rendah
- ❌ Losses tidak turun signifikan
- ❌ Model terlalu simple

---

## 🔄 Next Steps (Jika Perlu)

### **Jika Val Acc 82-85%:** ✅ Bagus!
- Simpan model
- Test di data baru
- Deploy untuk produksi

### **Jika Val Acc 78-82%:** 📊 Coba Data Augmentation
```python
transforms.Compose([
    transforms.RandomRotation(10),
    transforms.RandomAffine(0, translate=(0.1, 0.1)),
    transforms.ToTensor(),
])
```

### **Jika Val Acc < 78%:** 🔧 Coba Transfer Learning
- Gunakan pre-trained ResNet/EfficientNet
- Fine-tune pada ChestMNIST data
- Potential Val Acc: 85-90%

---

## 💾 Files Generated

- ✅ `best_model.pth` - Best model checkpoint
- ✅ `training_history.png` - Loss & accuracy curves
- ✅ `val_predictions.png` - Prediction visualizations

---

## 🚀 Cara Menggunakan Model yang Sudah Di-Train

```python
import torch
from model import SimpleCNN

# Load model
model = SimpleCNN(in_channels=1, num_classes=2, dropout_rate=0.3)
checkpoint = torch.load('best_model.pth')
model.load_state_dict(checkpoint['model_state_dict'])
model.eval()

# Inference
device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
model = model.to(device)

with torch.no_grad():
    output = model(input_tensor.to(device))
    prediction = torch.sigmoid(output)
```

---

**Updated:** November 5, 2025  
**Status:** Training in progress with improved architecture 🚀
