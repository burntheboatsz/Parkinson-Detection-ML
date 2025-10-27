# 📤 Panduan Upload Project ke GitHub

## Persiapan

### ✅ Yang Sudah Disiapkan:
- ✓ Git repository sudah diinisialisasi
- ✓ File `.gitignore` sudah ada
- ✓ File `README.md` sudah ada
- ✓ Project structure lengkap

---

## 🚀 Langkah-Langkah Upload ke GitHub

### **STEP 1: Buat Repository di GitHub**

1. **Buka browser** → https://github.com
2. **Login** dengan akun GitHub Anda
3. Klik tombol **"+" (pojok kanan atas)** → **"New repository"**
4. **Isi form:**
   - Repository name: `parkinson-detection-ml` (atau nama lain)
   - Description: `Sistem Deteksi Penyakit Parkinson Menggunakan Machine Learning - Teknik Biomedis ITERA`
   - Visibility: 
     - ✅ **Public** (jika ingin dibagikan ke publik)
     - ⚠️ **Private** (jika hanya untuk pribadi/tugas)
   - ❌ **JANGAN** centang "Add a README file" (karena sudah ada)
   - ❌ **JANGAN** pilih .gitignore (karena sudah ada)
   - ❌ **JANGAN** pilih license (bisa ditambah nanti)
5. Klik **"Create repository"**
6. **COPY URL repository** yang muncul, contoh:
   ```
   https://github.com/nafizahmadharily/parkinson-detection-ml.git
   ```

---

### **STEP 2: Setup Git Configuration (Sekali Saja)**

Buka **PowerShell** di folder project (`D:\vscode\project-alzheimer`):

```powershell
# Set nama Anda (akan muncul di commit history)
git config --global user.name "Nafiz Ahmadin Harily"

# Set email GitHub Anda
git config --global user.email "nafiz.122430051@student.itera.ac.id"

# Cek konfigurasi
git config --global --list
```

---

### **STEP 3: Add, Commit, dan Push**

Jalankan command ini **satu per satu** di PowerShell:

```powershell
# 1. Masuk ke folder project
cd D:\vscode\project-alzheimer

# 2. Cek status (lihat file yang akan di-commit)
git status

# 3. Add semua file ke staging area
git add .

# 4. Cek status lagi (file sekarang berwarna hijau)
git status

# 5. Commit dengan message yang jelas
git commit -m "Initial commit: Parkinson Detection ML System - XGBoost 94.87% accuracy"

# 6. Link ke repository GitHub (GANTI URL dengan URL repository Anda!)
git remote add origin https://github.com/[USERNAME]/parkinson-detection-ml.git

# 7. Cek remote (pastikan URL benar)
git remote -v

# 8. Push ke GitHub (branch main)
git branch -M main
git push -u origin main
```

**⚠️ PENTING:** Ganti `[USERNAME]` dengan username GitHub Anda!

---

### **STEP 4: Verifikasi Upload**

1. Buka browser → GitHub repository Anda
2. **Refresh** halaman
3. ✅ Pastikan semua file sudah terupload:
   - ✓ `data/parkinsons.csv`
   - ✓ `notebooks/01_eda.ipynb`
   - ✓ `notebooks/02_train_models.ipynb`
   - ✓ `notebooks/03_improve_model.ipynb`
   - ✓ `src/` folder
   - ✓ `presentation/` folder
   - ✓ `README.md`
   - ✓ `.gitignore`

---

## 🔐 Authentication

Jika diminta **username dan password** saat push:

### Opsi 1: Personal Access Token (PAT) - RECOMMENDED

1. **GitHub** → Settings → Developer settings → Personal access tokens → Tokens (classic)
2. Klik **"Generate new token"** → **"Generate new token (classic)"**
3. Isi:
   - Note: `ITERA Laptop`
   - Expiration: `90 days` (atau sesuai kebutuhan)
   - Centang: ✅ **repo** (semua sub-checkbox)
4. Klik **"Generate token"**
5. **COPY token** (hanya muncul sekali!)
6. Saat push, masukkan:
   - Username: `nafizahmadharily` (username GitHub Anda)
   - Password: **[PASTE TOKEN]** (bukan password biasa!)

### Opsi 2: GitHub CLI (gh)

```powershell
# Install GitHub CLI (jika belum)
winget install --id GitHub.cli

# Login
gh auth login

# Pilih: GitHub.com → HTTPS → Yes → Login with a web browser
```

---

## 📝 Command Cheat Sheet

### Pertama Kali Upload:
```powershell
git add .
git commit -m "Initial commit: message"
git remote add origin https://github.com/[USERNAME]/[REPO].git
git push -u origin main
```

### Update Setelah Perubahan:
```powershell
git add .
git commit -m "Update: deskripsi perubahan"
git push
```

### Cek Status:
```powershell
git status              # Lihat file yang berubah
git log --oneline       # Lihat commit history
git remote -v           # Lihat URL repository
```

### Batalkan Perubahan (Sebelum Commit):
```powershell
git restore [filename]  # Batalkan perubahan 1 file
git restore .           # Batalkan semua perubahan
```

---

## 🎯 Tips & Best Practices

### 1. **Commit Message yang Baik**
```powershell
# ❌ BAD
git commit -m "update"
git commit -m "fix"

# ✅ GOOD
git commit -m "Add: Feature engineering strategy with 20 new features"
git commit -m "Fix: Hyperparameter tuning bug in RandomizedSearchCV"
git commit -m "Update: README with installation instructions"
git commit -m "Docs: Add presentation slides (39 slides, Beamer LaTeX)"
```

### 2. **Commit Frequently**
- Commit setiap kali ada progress signifikan
- Jangan menunggu sampai selesai semua

### 3. **File Besar**
Jika ada error "file too large" (>100MB):
```powershell
# Install Git LFS
git lfs install

# Track file besar (contoh: model .pkl)
git lfs track "*.pkl"
git add .gitattributes
git add models/xgboost.pkl
git commit -m "Add: XGBoost model with Git LFS"
git push
```

### 4. **Branching (Advanced)**
```powershell
# Buat branch baru untuk eksperimen
git checkout -b experiment-deep-learning

# Kembali ke main
git checkout main

# Merge branch
git merge experiment-deep-learning
```

---

## 🆘 Troubleshooting

### Error: "Permission denied (publickey)"
**Solusi:** Gunakan HTTPS, bukan SSH. URL harus:
```
https://github.com/[USERNAME]/[REPO].git
```
Bukan:
```
git@github.com:[USERNAME]/[REPO].git
```

### Error: "remote origin already exists"
**Solusi:**
```powershell
git remote remove origin
git remote add origin https://github.com/[USERNAME]/[REPO].git
```

### Error: "failed to push some refs"
**Solusi 1 (HATI-HATI!):** Force push (jika yakin local lebih baru)
```powershell
git push -f origin main
```

**Solusi 2 (AMAN):** Pull dulu, lalu push
```powershell
git pull origin main --allow-unrelated-histories
git push origin main
```

### Error: "File too large" (>100MB)
**Solusi:** Gunakan Git LFS atau hapus dari tracking
```powershell
# Hapus dari Git (tapi tetap ada di local)
git rm --cached [large-file]
git commit -m "Remove large file from tracking"
git push
```

---

## ✅ Checklist Upload

Sebelum push, pastikan:
- [ ] `.gitignore` sudah benar (file sensitif tidak terupload)
- [ ] `README.md` sudah lengkap dan informatif
- [ ] Model files (`.pkl`) sudah di-ignore jika terlalu besar
- [ ] Tidak ada credentials/password di code
- [ ] Commit message jelas dan deskriptif
- [ ] Repository visibility sudah sesuai (public/private)

---

## 🎓 Resources

- [GitHub Docs](https://docs.github.com)
- [Git Cheat Sheet](https://education.github.com/git-cheat-sheet-education.pdf)
- [Markdown Guide](https://www.markdownguide.org/)
- [Conventional Commits](https://www.conventionalcommits.org/)

---

## 📧 Butuh Bantuan?

Jika ada masalah, jangan ragu untuk:
1. Baca error message dengan teliti
2. Google error message tersebut
3. Tanya di Stack Overflow
4. Konsultasi dengan dosen/asisten lab

---

**Good luck! 🚀**
