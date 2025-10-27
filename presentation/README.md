# Presentasi Beamer LaTeX: Deteksi Parkinson

## File
- `parkinson_detection_presentation.tex` - Source LaTeX Beamer

## Cara Compile

### Opsi 1: Menggunakan Online Editor (Tercepat)
1. Buka [Overleaf.com](https://www.overleaf.com)
2. Klik "New Project" → "Upload Project"
3. Upload file `parkinson_detection_presentation.tex`
4. Klik "Recompile" untuk generate PDF
5. Download PDF hasil kompilasi

### Opsi 2: Menggunakan LaTeX Lokal
```bash
# Install MiKTeX (Windows) atau TeX Live (Linux/Mac)

# Compile dengan pdflatex
pdflatex parkinson_detection_presentation.tex
pdflatex parkinson_detection_presentation.tex  # Run twice untuk references

# Atau dengan latexmk
latexmk -pdf parkinson_detection_presentation.tex
```

### Opsi 3: Menggunakan VS Code
1. Install extension: "LaTeX Workshop"
2. Buka file `.tex`
3. Klik "Build LaTeX project" atau `Ctrl+Alt+B`

## Isi Presentasi

### Struktur (33 Slides)
1. **Title & Outline** (2 slides)
2. **Pendahuluan** (2 slides)
   - Latar belakang penyakit Parkinson
   - Tujuan penelitian
3. **Dataset & Metodologi** (4 slides)
   - Deskripsi dataset (195 samples, 24 features)
   - Fitur biomarker suara
   - Metodologi eksperimen
4. **Exploratory Data Analysis** (4 slides)
   - Statistik deskriptif
   - Distribusi target (imbalanced 3:1)
   - Visualisasi
   - Insight dari EDA
5. **Training & Evaluasi Model** (5 slides)
   - 10 model ML yang diuji
   - Perbandingan akurasi (bar chart)
   - Tabel hasil lengkap
   - Confusion matrix XGBoost
   - Interpretasi hasil
6. **Strategi Optimasi** (7 slides)
   - Tujuan optimasi (target 95%+)
   - 5 strategi: Hyperparameter Tuning, Voting Ensemble, Feature Engineering, Feature Selection, Stacking Ensemble
   - Hasil optimasi (semua strategi: NO IMPROVEMENT)
   - Analisis mengapa tidak bisa ditingkatkan
7. **Deployment** (3 slides)
   - Tools: CLI, Web App, Python API
   - Contoh penggunaan
   - Confidence thresholds
   - File model untuk production
8. **Kesimpulan & Rekomendasi** (6 slides)
   - Ringkasan pencapaian
   - Limitasi penelitian
   - Rekomendasi pengembangan
   - Future research directions
   - Impact dan kontribusi
   - Kesimpulan akhir

### Highlights
- ✅ Visualisasi dengan TikZ dan PGFPlots
- ✅ Bar chart perbandingan akurasi 10 model
- ✅ Confusion matrix XGBoost
- ✅ Line chart feature selection
- ✅ Diagram alur ensemble methods
- ✅ Tabel perbandingan komprehensif
- ✅ Color-coded results (green=best, red=worst, orange=same)
- ✅ Penjelasan detail step-by-step dari awal
- ✅ Bahasa Indonesia lengkap

## Customization

### Ganti Warna Theme
```latex
\definecolor{darkblue}{RGB}{0,51,102}  % Ganti RGB sesuai keinginan
```

### Ganti Theme Beamer
```latex
\usetheme{Madrid}  % Options: Berlin, Copenhagen, Warsaw, etc.
```

### Tambah Logo
```latex
\logo{\includegraphics[height=1cm]{logo.png}}
```

## Output
- Format: PDF (Landscape 16:9)
- Total slides: 33
- Estimasi durasi presentasi: 30-40 menit

## Dependencies
- pdflatex atau xelatex
- Packages: beamer, babel (indonesian), tikz, pgfplots, booktabs, amsmath

## Tips Presentasi
1. Mulai dengan motivasi (slide 3-4)
2. Fokus pada metodologi (slide 7-8)
3. Highlight hasil training (slide 13-14)
4. Tekankan hasil optimasi: "Model sudah optimal" (slide 22)
5. Tutup dengan rekomendasi deployment (slide 28)
