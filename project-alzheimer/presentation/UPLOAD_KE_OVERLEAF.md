# 🎯 CARA CEPAT DAPATKAN PDF PRESENTASI

## ✅ FILE ZIP SUDAH SIAP!

**Lokasi:** `D:\vscode\project-alzheimer\presentation\parkinson_presentation.zip`

---

## 📤 LANGKAH UPLOAD KE OVERLEAF (30 DETIK!)

### 1️⃣ Buka Overleaf
```
https://www.overleaf.com
```

### 2️⃣ Sign Up / Login
- Klik "Register" (pojok kanan atas)
- Gunakan Google Account atau Email
- **GRATIS!**

### 3️⃣ Upload Project
1. Klik tombol hijau **"New Project"** (pojok kiri atas)
2. Pilih **"Upload Project"**
3. Klik **"Select a .zip file"**
4. Pilih file: `parkinson_presentation.zip`

### 4️⃣ Tunggu Compile
- Overleaf otomatis compile (~10 detik)
- Preview PDF muncul di panel kanan
- Jika ada warning/error, abaikan (normal)

### 5️⃣ Download PDF
- Klik tombol **"Download PDF"** (ikon download di atas preview)
- File `parkinson_detection_presentation.pdf` tersimpan!

---

## 🎉 SELESAI!

**Total waktu:** ~30 detik  
**Output:** PDF presentasi siap pakai (39 slides)  
**Format:** Landscape 16:9  
**Ukuran:** ~500KB - 1MB  

---

## 🔧 ALTERNATIF: Compile Lokal (Jika Sudah Ada MiKTeX)

### Jika MiKTeX Sudah Terinstall:

```powershell
# Di folder presentation
cd D:\vscode\project-alzheimer\presentation

# Compile (run 2x untuk references)
pdflatex parkinson_detection_presentation.tex
pdflatex parkinson_detection_presentation.tex

# Atau pakai script otomatis
.\compile.bat
```

### Install MiKTeX (Jika Belum Ada):
1. Download: https://miktex.org/download
2. Install (~10-15 menit, 4GB space)
3. Restart terminal
4. Run compile command di atas

---

## 📊 Isi Presentasi (39 Slides)

1. **Title & Outline** (2 slides)
2. **Pendahuluan** (2 slides) - Latar belakang & tujuan
3. **Dataset & Metodologi** (4 slides) - 195 samples, 22 features
4. **EDA** (4 slides) - Statistik & visualisasi
5. **Training & Evaluasi** (5 slides) - 10 models, XGBoost best (94.87%)
6. **Strategi Optimasi** (7 slides) - 5 strategies tested, all NO IMPROVEMENT
7. **Deployment** (4 slides) - CLI, Web App, Python API
8. **Kesimpulan** (6 slides) - Limitasi, rekomendasi, future work
9. **Thank You** (1 slide)
10. **Appendix** (2 slides) - Technical specs

---

## 🎨 Highlights Presentasi

✅ **6 TikZ Diagrams:**
- Metodologi flowchart
- Bar chart distribusi kelas
- Horizontal bar chart akurasi 10 models
- Confusion matrix XGBoost
- Voting ensemble architecture
- Stacking ensemble architecture

✅ **1 PGFPlots:**
- Line chart feature selection

✅ **5 Professional Tables:**
- Features biomarker
- Top 5 models comparison
- Improvement strategies summary
- Technical specifications
- Full features details

✅ **Color-Coded Results:**
- 🟢 Green = Best (XGBoost)
- 🔴 Red = Worst/Error
- 🟠 Orange = Warning/Same
- 🔵 Blue = Standard

---

## 💡 Tips Presentasi

### Durasi:
- Full presentation: **30-40 menit**
- Dengan Q&A: **45-50 menit**

### Key Messages:
1. **Slide 15:** "XGBoost = 94.87% (BEST!)"
2. **Slide 24:** "5 optimasi → SEMUA GAGAL!"
3. **Slide 25:** "Dataset kecil = performance ceiling"
4. **Slide 36:** "Ready deploy! Perlu more data untuk improve"

### Saran Flow:
```
Opening (5 min)    → Parkinson problem
Dataset (5 min)    → 195 samples, 22 features
Training (8 min)   → 10 models, XGBoost wins
Optimasi (10 min)  → 5 strategies, PLOT TWIST: gagal semua!
Deployment (4 min) → Tools ready
Closing (5 min)    → Limitasi & rekomendasi
```

---

## 🆘 Troubleshooting

### "Upload failed" di Overleaf
→ File .zip terlalu besar? Tidak, cuma ~5KB
→ Try upload lagi atau drag & drop

### "Compile error" di Overleaf
→ Normal! Package akan auto-download
→ Tunggu 10-20 detik, refresh page

### "PDF tidak muncul"
→ Check tab "Logs & Output" untuk error
→ Biasanya auto-resolve setelah package download

### "Font/visualisasi aneh"
→ Overleaf pakai TeXLive (beda dari MiKTeX)
→ Should be fine, coba compile 2x

---

## 📞 Need Help?

1. Check `PANDUAN_LENGKAP.md` di folder presentation
2. Overleaf help: https://www.overleaf.com/learn
3. LaTeX help: https://tex.stackexchange.com

---

## ✅ Checklist

- [x] File ZIP dibuat: `parkinson_presentation.zip`
- [x] Lokasi: `D:\vscode\project-alzheimer\presentation\`
- [ ] Upload ke Overleaf
- [ ] Download PDF
- [ ] Test di proyektor
- [ ] Siap presentasi!

---

**Good luck! 🚀**

File ZIP sudah siap di folder presentation.  
Tinggal upload ke Overleaf dan download PDF!
