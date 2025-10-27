# Panduan Lengkap: Membuat dan Menggunakan Presentasi

## 📖 Overview
Presentasi Beamer LaTeX komprehensif tentang **Sistem Deteksi Penyakit Parkinson menggunakan Machine Learning**. Mencakup seluruh proses dari EDA hingga deployment dengan 33 slides profesional.

---

## 🚀 Quick Start

### Cara Tercepat: Overleaf (Online)
1. Buka https://www.overleaf.com (gratis)
2. Login atau buat akun
3. Klik **"New Project"** → **"Upload Project"**
4. Upload file `parkinson_detection_presentation.tex`
5. Klik **"Recompile"**
6. Download PDF yang dihasilkan

✅ **Keuntungan:** Tidak perlu install apapun, langsung jadi PDF!

---

## 💻 Compile Lokal

### Windows

#### Opsi 1: Menggunakan Batch Script (Otomatis)
```cmd
# Di folder presentation
.\compile.bat
```

#### Opsi 2: Manual dengan MiKTeX
1. Download dan install MiKTeX: https://miktex.org/download
2. Buka Command Prompt di folder `presentation`
3. Run:
```cmd
pdflatex parkinson_detection_presentation.tex
pdflatex parkinson_detection_presentation.tex
```

### Linux / Mac
```bash
# Install TeX Live
sudo apt-get install texlive-full  # Ubuntu/Debian
brew install --cask mactex          # macOS

# Compile
cd presentation
pdflatex parkinson_detection_presentation.tex
pdflatex parkinson_detection_presentation.tex
```

### VS Code (Recommended untuk Developer)
1. Install extension: **"LaTeX Workshop"** oleh James Yu
2. Buka file `parkinson_detection_presentation.tex`
3. Tekan `Ctrl+Alt+B` atau klik "Build LaTeX project"
4. PDF akan muncul di samping

---

## 📊 Isi Presentasi (33 Slides)

### Section 1: Pendahuluan (Slides 1-4)
- **Slide 1:** Title page dengan judul, subtitle, author
- **Slide 2:** Table of contents
- **Slide 3:** Latar belakang penyakit Parkinson & solusi ML
- **Slide 4:** Tujuan penelitian (5 poin utama)

### Section 2: Dataset & Metodologi (Slides 5-8)
- **Slide 5:** Deskripsi dataset (195 samples, 24 features, distribusi kelas)
- **Slide 6:** Tabel 22 fitur biomarker suara (frequency, variation, ratio, nonlinear)
- **Slide 7:** Metodologi eksperimen (6-step flowchart dengan TikZ)
- **Slide 8:** Statistik deskriptif dataset

### Section 3: EDA (Slides 9-12)
- **Slide 9:** Statistik deskriptif lengkap
- **Slide 10:** Visualisasi distribusi target (bar chart: 48 sehat vs 147 Parkinson)
- **Slide 11:** Insight dari EDA
- **Slide 12:** Kesimpulan EDA

### Section 4: Training & Evaluasi (Slides 13-17)
- **Slide 13:** Daftar 10 model ML yang diuji
- **Slide 14:** **Bar chart horizontal** akurasi semua model (XGBoost highlighted)
- **Slide 15:** Tabel top 5 models dengan 5 metrics
- **Slide 16:** **Confusion matrix XGBoost** (TikZ visualization)
- **Slide 17:** Interpretasi hasil XGBoost

### Section 5: Strategi Optimasi (Slides 18-24)
- **Slide 18:** Tujuan optimasi (94.87% → 95%+)
- **Slide 19:** Strategi 1 - Hyperparameter Tuning (WORSE: 92.31%)
- **Slide 20:** Strategi 2 - Voting Ensemble (flowchart, NO CHANGE)
- **Slide 21:** Strategi 3 - Feature Engineering (NO CHANGE)
- **Slide 22:** Strategi 4 - Feature Selection (line chart, NO CHANGE)
- **Slide 23:** Strategi 5 - Stacking Ensemble (architecture diagram, NO CHANGE)
- **Slide 24:** **Tabel ringkasan** semua strategi optimasi
- **Slide 25:** Analisis mendalam: Mengapa tidak bisa ditingkatkan?

### Section 6: Deployment (Slides 26-28)
- **Slide 26:** Tools untuk deployment (CLI, Web App, Python API)
- **Slide 27:** Contoh code penggunaan Python API
- **Slide 28:** Confidence thresholds untuk clinical use
- **Slide 29:** File-file model untuk production

### Section 7: Kesimpulan (Slides 30-33)
- **Slide 30:** Ringkasan pencapaian (6 checklist)
- **Slide 31:** Limitasi penelitian (4 poin)
- **Slide 32:** Rekomendasi pengembangan (TINGGI & MEDIUM priority)
- **Slide 33:** Rekomendasi deployment (3 immediate actions)
- **Slide 34:** Future research directions (4 areas)
- **Slide 35:** Impact dan kontribusi (scientific, practical, clinical)
- **Slide 36:** **Kesimpulan akhir** (highlighted key metrics)

### Appendix (Slides 37-39)
- **Slide 37:** Thank you & contact
- **Slide 38:** Technical specifications
- **Slide 39:** Dataset features details (full table)

---

## 🎨 Customization

### Ganti Warna Theme
Edit di bagian awal `.tex`:
```latex
\definecolor{darkblue}{RGB}{0,51,102}    % Warna utama
\definecolor{lightblue}{RGB}{51,153,255} % Warna aksen
\definecolor{darkgreen}{RGB}{0,128,0}    % Warna sukses
\definecolor{darkred}{RGB}{178,34,34}    % Warna error
```

### Ganti Beamer Theme
```latex
\usetheme{Madrid}  % Ganti dengan: Berlin, Copenhagen, Warsaw, Singapore, dll.
```

### Tambah Logo Institusi
```latex
\logo{\includegraphics[height=1cm]{path/to/logo.png}}
```

### Edit Author/Institusi
```latex
\author{Nama Anda}
\institute{Universitas/Institusi Anda}
```

---

## 📐 Features & Visualizations

### TikZ Diagrams
1. **Flowchart metodologi** (Slide 7)
2. **Bar chart distribusi** (Slide 10)
3. **Horizontal bar chart akurasi** (Slide 14)
4. **Confusion matrix** (Slide 16)
5. **Ensemble architecture** (Slides 20, 23)

### PGFPlots
1. **Line chart** feature selection (Slide 22)

### Tables
1. Fitur biomarker (Slide 6)
2. Top 5 models (Slide 15)
3. Improvement strategies (Slide 24)
4. Technical specs (Slide 38)
5. Full features details (Slide 39)

---

## 🎤 Tips Presentasi

### Timing (Total: 30-40 menit)
- **Pendahuluan:** 5 menit (motivasi & tujuan)
- **Metodologi:** 5 menit (dataset & preprocessing)
- **EDA:** 3 menit (highlight insights)
- **Training:** 8 menit (fokus pada perbandingan 10 model)
- **Optimasi:** 10 menit (explain mengapa tidak bisa improve - PENTING!)
- **Deployment:** 4 menit (show tools)
- **Kesimpulan:** 5 menit (rekomendasi & future work)

### Key Points untuk Ditekankan
1. **Slide 14:** XGBoost menang dengan 94.87% (visual impactful)
2. **Slide 24:** **SEMUA optimasi gagal** → model sudah optimal!
3. **Slide 25:** Analisis mendalam (dataset kecil = batasan)
4. **Slide 36:** Kesimpulan: Siap deploy, tapi perlu more data untuk improve

### Saran Flow
```
1. Hook audience dengan masalah (Slide 3)
   → "Parkinson sulit dideteksi dini, butuh expertise"

2. Show data (Slide 5-6)
   → "Kita punya 195 pasien, 22 biomarker suara"

3. Build suspense (Slide 13-14)
   → "Kita uji 10 model... mana yang terbaik?"

4. Reveal winner (Slide 15)
   → "XGBoost: 94.87% accuracy, 96.90% ROC-AUC!"

5. Challenge (Slide 18)
   → "Bisa lebih baik? Mari coba 5 strategi..."

6. Plot twist (Slide 24)
   → "Ternyata... TIDAK BISA ditingkatkan!"

7. Explain why (Slide 25)
   → "Dataset kecil, model sudah optimal"

8. Happy ending (Slide 36)
   → "94.87% sudah excellent, deploy sekarang, collect data untuk v2"
```

---

## 📤 Export & Sharing

### Format Output
- **PDF:** Landscape 16:9 (optimal untuk proyektor)
- **Size:** ~500KB - 1MB
- **Compatibility:** Universal (bisa dibuka di semua OS)

### Sharing Options
1. **Email:** Langsung attach PDF
2. **Google Drive/Dropbox:** Upload dan share link
3. **Print:** Cetak sebagai handout (6 slides per page)
4. **Video:** Record presentasi dengan OBS Studio

---

## 🔧 Troubleshooting

### Error: "pdflatex command not found"
**Solusi:** Install LaTeX distribution:
- Windows: MiKTeX (https://miktex.org)
- Mac: MacTeX (https://www.tug.org/mactex/)
- Linux: `sudo apt-get install texlive-full`

### Error: "Package tikz not found"
**Solusi:** Install package manager akan auto-download
- MiKTeX: Akan popup "Install package?" → Klik Yes
- TeX Live: Sudah include semua packages

### Warning: "Overfull \hbox"
**Solusi:** Abaikan saja, tidak mempengaruhi output visual

### Compile lambat
**Solusi:** 
- Gunakan draft mode: `\documentclass[draft]{beamer}`
- Compile sekali saja (skip second run)

### Hasil PDF tidak muncul
**Solusi:**
- Check output folder (same directory as .tex)
- Cek log file untuk error messages
- Gunakan Overleaf jika masih error

---

## 📚 Resources

### Belajar Beamer
- Official docs: https://ctan.org/pkg/beamer
- Overleaf tutorial: https://www.overleaf.com/learn/latex/Beamer

### Belajar TikZ
- TikZ manual: https://tikz.dev
- Examples: https://texample.net/tikz/

### Template & Themes
- Beamer themes: https://hartwork.org/beamer-theme-matrix/
- Color schemes: https://www.latexcolor.com

---

## 📞 Support

Jika ada masalah:
1. Check `README.md` di folder presentation
2. Review error di log file (`.log`)
3. Coba compile di Overleaf untuk isolate masalah
4. Google error message (biasanya ada solusi di StackExchange)

---

## ✅ Checklist Sebelum Presentasi

- [ ] PDF berhasil di-compile
- [ ] Semua visualisasi muncul dengan baik
- [ ] Typo sudah dicek
- [ ] Author/institusi sudah diganti sesuai
- [ ] Test di proyektor (font size cukup besar)
- [ ] Backup PDF di cloud (Google Drive/OneDrive)
- [ ] Print handout (optional)
- [ ] Siapkan pointer/clicker
- [ ] Practice timing (aim 30-35 menit)
- [ ] Siapkan Q&A answers untuk:
  - "Mengapa tidak improve?"
  - "Apakah bisa untuk production?"
  - "Berapa data yang dibutuhkan untuk improve?"

---

**Good luck dengan presentasinya!** 🎉
