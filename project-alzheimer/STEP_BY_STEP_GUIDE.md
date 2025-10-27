# PANDUAN LENGKAP - Parkinson Disease Detection System

## 📚 STEP BY STEP GUIDE

Berikut adalah panduan lengkap langkah demi langkah untuk menggunakan sistem ini:

---

## 🎯 LANGKAH 1: Persiapan Environment

### 1.1 Install Python
Pastikan Python 3.8+ sudah terinstall di komputer Anda.

```powershell
# Cek versi Python
python --version
```

### 1.2 Setup Virtual Environment

```powershell
# Buat virtual environment
python -m venv venv

# Aktifkan virtual environment
.\venv\Scripts\Activate.ps1

# Jika ada error execution policy, jalankan:
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

### 1.3 Install Dependencies

```powershell
# Install semua library yang diperlukan
pip install -r requirements.txt

# Tunggu hingga selesai (mungkin butuh beberapa menit)
```

---

## 📊 LANGKAH 2: Persiapan Dataset

### 2.1 Letakkan Dataset

1. Copy file dataset Parkinson Anda
2. Paste ke folder `data/`
3. Rename menjadi `parkinsons.csv`

**Struktur file:**
```
data/
└── parkinsons.csv  <- Dataset Anda di sini
```

### 2.2 Format Dataset yang Dibutuhkan

Dataset harus berupa CSV dengan:
- ✅ Header (nama kolom)
- ✅ Kolom target (contoh: `status`, `target`, `label`)
- ✅ Fitur-fitur numerik

**Contoh struktur:**
```csv
name,MDVP:Fo(Hz),MDVP:Fhi(Hz),...,status
Patient1,197.076,206.896,...,1
Patient2,162.568,198.346,...,0
```

---

## 🔍 LANGKAH 3: Exploratory Data Analysis (EDA)

### 3.1 Buka Jupyter Notebook

```powershell
# Jalankan Jupyter
jupyter notebook
```

Browser akan terbuka otomatis.

### 3.2 Jalankan Notebook EDA

1. Buka `notebooks/00_how_to_use.ipynb` (untuk panduan)
2. Buka `notebooks/01_exploratory_data_analysis.ipynb`
3. Jalankan cell per cell (Shift + Enter)
4. Perhatikan output setiap cell

**Yang akan Anda lihat:**
- ✓ Informasi dataset (jumlah baris, kolom)
- ✓ Missing values (jika ada)
- ✓ Distribusi data
- ✓ Korelasi antar features
- ✓ Visualisasi data

### 3.3 Sesuaikan Konfigurasi

Di cell pertama, sesuaikan jika perlu:

```python
# Jika nama file berbeda
DATA_PATH = '../data/nama_file_anda.csv'

# Jika kolom target berbeda
TARGET_COL = 'nama_kolom_target_anda'
```

---

## 🤖 LANGKAH 4: Training Model

### 4.1 Buka Notebook Training

1. Buka `notebooks/02_model_training.ipynb`
2. Pastikan sudah menjalankan EDA terlebih dahulu

### 4.2 Konfigurasi Preprocessing

Cell preprocessing, Anda bisa sesuaikan:

```python
X_train, X_test, y_train, y_test = preprocessor.prepare_data(
    df=df,
    target_col=TARGET_COL,
    drop_cols=['name'],      # Kolom yang tidak diperlukan
    test_size=0.2,           # 20% untuk testing
    random_state=42,
    scale=True,              # Standardisasi data
    balance=False,           # True jika data imbalanced
    balance_method='smote'   # smote/undersample/smotetomek
)
```

### 4.3 Jalankan Training

Jalankan semua cell. Proses akan:
1. ✓ Load dan preprocess data
2. ✓ Train 10 model berbeda:
   - Logistic Regression
   - Decision Tree
   - Random Forest ⭐
   - SVM
   - KNN
   - Naive Bayes
   - Gradient Boosting
   - XGBoost ⭐
   - LightGBM ⭐
   - CatBoost ⭐

3. ✓ Evaluasi semua model
4. ✓ Save model terbaik

**Waktu training:** 1-5 menit tergantung ukuran dataset

### 4.4 Lihat Hasil Evaluasi

Anda akan mendapatkan tabel perbandingan:

```
Model               Accuracy  Precision  Recall  F1-Score  ROC-AUC
------------------------------------------------------------------
Random Forest       0.9487    0.9500     0.9487  0.9488    0.9850
XGBoost            0.9231    0.9250     0.9231  0.9235    0.9750
SVM                0.8974    0.9000     0.8974  0.8980    0.9600
...
```

---

## 🎯 LANGKAH 5: Menggunakan Model untuk Prediksi

### Opsi 1: Menggunakan Script CLI

```powershell
# Jalankan script prediksi
python src/predict.py

# Pilih opsi yang tersedia:
# 1. Predict from CSV file
# 2. Predict single patient (manual input)
# 3. Exit
```

### Opsi 2: Menggunakan Code

Buat file Python baru atau gunakan di notebook:

```python
import sys
sys.path.append('src')

from model_utils import ModelUtils

# 1. Load model
model = ModelUtils.load_model('models/random_forest.pkl')
scaler, features = ModelUtils.load_preprocessing_params('models')

# 2. Prepare patient data
patient_data = {
    'MDVP:Fo(Hz)': 197.076,
    'MDVP:Fhi(Hz)': 206.896,
    # ... semua features lainnya
}

# 3. Predict
result = ModelUtils.predict_single_patient(model, patient_data, scaler, features)

# 4. Lihat hasil
print(f"Prediction: {result['prediction_label']}")
print(f"Probability: {result['probability_parkinson']:.2%}")
```

### Opsi 3: Batch Prediction dari CSV

```python
from model_utils import ModelUtils
import pandas as pd

# Load model
model = ModelUtils.load_model('models/random_forest.pkl')
scaler, features = ModelUtils.load_preprocessing_params('models')

# Load new patients data
new_patients = pd.read_csv('data/new_patients.csv')

# Predict
predictions, probabilities = ModelUtils.predict(
    model, 
    new_patients[features], 
    scaler, 
    features
)

# Add to dataframe
new_patients['prediction'] = predictions
new_patients['probability'] = probabilities[:, 1]

# Save results
new_patients.to_csv('results/predictions.csv', index=False)
```

---

## 📈 LANGKAH 6: Interpretasi Hasil

### Memahami Output

```python
{
    'prediction': 1,
    'prediction_label': 'Parkinson',
    'probability_healthy': 0.15,
    'probability_parkinson': 0.85
}
```

**Interpretasi:**
- `prediction`: 0 = Healthy, 1 = Parkinson
- `prediction_label`: Label dalam text
- `probability_parkinson`: Tingkat kepercayaan model (0-1)

### Threshold Confidence

Anda bisa set threshold:

```python
result = ModelUtils.predict_single_patient(model, data, scaler, features)

if result['probability_parkinson'] >= 0.8:
    print("HIGH RISK - Probability:", result['probability_parkinson'])
elif result['probability_parkinson'] >= 0.5:
    print("MEDIUM RISK - Probability:", result['probability_parkinson'])
else:
    print("LOW RISK - Probability:", result['probability_parkinson'])
```

---

## 🔧 TROUBLESHOOTING

### Problem 1: Import Error

```
ImportError: No module named 'pandas'
```

**Solution:**
```powershell
pip install pandas numpy scikit-learn
```

### Problem 2: Dataset Not Found

```
FileNotFoundError: [Errno 2] No such file or directory: '../data/parkinsons.csv'
```

**Solution:**
1. Cek apakah file ada di folder `data/`
2. Cek nama file sudah benar
3. Sesuaikan path di notebook

### Problem 3: Model Not Found

```
FileNotFoundError: No such file or directory: '../models/random_forest.pkl'
```

**Solution:**
Train model terlebih dahulu dengan menjalankan `02_model_training.ipynb`

### Problem 4: Feature Mismatch

```
ValueError: Number of features doesn't match
```

**Solution:**
Pastikan data untuk prediksi memiliki semua features yang sama dengan training data.

---

## 💡 TIPS & BEST PRACTICES

### 1. Data Quality
- ✅ Pastikan tidak ada missing values
- ✅ Cek outliers dan handle dengan benar
- ✅ Pastikan data sudah clean

### 2. Model Selection
- ✅ Random Forest biasanya bagus untuk data tabel
- ✅ XGBoost/LightGBM untuk performa terbaik
- ✅ SVM bagus untuk dataset kecil-medium

### 3. Hyperparameter Tuning
Untuk hasil lebih baik, tune hyperparameters:

```python
from sklearn.model_selection import GridSearchCV

param_grid = {
    'n_estimators': [100, 200, 300],
    'max_depth': [10, 20, 30],
    'min_samples_split': [2, 5, 10]
}

grid_search = GridSearchCV(RandomForestClassifier(), param_grid, cv=5)
grid_search.fit(X_train, y_train)

best_model = grid_search.best_estimator_
```

### 4. Cross-Validation
Gunakan cross-validation untuk evaluasi lebih robust:

```python
from sklearn.model_selection import cross_val_score

scores = cross_val_score(model, X, y, cv=5, scoring='accuracy')
print(f"CV Accuracy: {scores.mean():.4f} (+/- {scores.std():.4f})")
```

---

## 🚀 NEXT LEVEL

### 1. Deploy as Web App

Install Streamlit:
```powershell
pip install streamlit
```

Buat file `app.py`:
```python
import streamlit as st
from model_utils import ModelUtils

st.title('🏥 Parkinson Disease Detector')

# Load model
model = ModelUtils.load_model('models/random_forest.pkl')
scaler, features = ModelUtils.load_preprocessing_params('models')

# Input form
st.header('Enter Patient Data')
data = {}
for feature in features:
    data[feature] = st.number_input(feature, value=0.0)

if st.button('Predict'):
    result = ModelUtils.predict_single_patient(model, data, scaler, features)
    
    st.success(f"Prediction: {result['prediction_label']}")
    st.info(f"Confidence: {result['probability_parkinson']:.2%}")
```

Jalankan:
```powershell
streamlit run app.py
```

### 2. Deploy as API

Install FastAPI:
```powershell
pip install fastapi uvicorn
```

Buat file `api.py`:
```python
from fastapi import FastAPI
from pydantic import BaseModel
from model_utils import ModelUtils

app = FastAPI()

# Load model at startup
model = ModelUtils.load_model('models/random_forest.pkl')
scaler, features = ModelUtils.load_preprocessing_params('models')

class PatientData(BaseModel):
    # Define all features
    pass

@app.post('/predict')
def predict(data: PatientData):
    result = ModelUtils.predict_single_patient(
        model, data.dict(), scaler, features
    )
    return result
```

Jalankan:
```powershell
uvicorn api:app --reload
```

---

## 📞 SUPPORT

Jika masih ada kesulitan:
1. Cek dokumentasi di README.md
2. Lihat example di `src/example_usage.py`
3. Review notebook `00_how_to_use.ipynb`

---

**Selamat mencoba! 🎉**

Jangan lupa untuk save model terbaik Anda dan backup dataset!
