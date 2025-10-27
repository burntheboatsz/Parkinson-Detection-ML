# Cara Mendapatkan PDF Presentasi

## CARA TERCEPAT: Overleaf (30 detik!)

### Langkah Detail:

1. **Buka Overleaf**
   - Browser → https://www.overleaf.com
   - Klik "Register" (atau "Log in" jika sudah punya akun)
   - Sign up gratis dengan email/Google/GitHub

2. **Upload Project**
   - Klik tombol hijau "New Project" (pojok kiri atas)
   - Pilih "Upload Project"
   - Drag & drop file `parkinson_detection_presentation.tex`
   - ATAU klik "Select a .zip file" → upload file .tex langsung

3. **Wait for Compile**
   - Overleaf otomatis compile (~5-10 detik)
   - Lihat preview PDF di panel kanan
   - Jika ada error, akan muncul di panel bawah

4. **Download PDF**
   - Klik tombol "Download PDF" (ikon download di atas preview)
   - File `parkinson_detection_presentation.pdf` tersimpan!

5. **DONE!** ✅
   - Total waktu: ~30 detik
   - File PDF siap untuk presentasi

---

## CARA ALTERNATIF: Compile Lokal

### Windows (Memerlukan MiKTeX)

#### Install MiKTeX:
```powershell
# Download dari: https://miktex.org/download
# Ukuran: ~300MB download, ~4GB setelah install
# Waktu install: ~10-15 menit

# Setelah install, restart terminal
```

#### Compile:
```powershell
# Otomatis (recommended)
cd presentation
.\compile.bat

# Manual
cd presentation
pdflatex parkinson_detection_presentation.tex
pdflatex parkinson_detection_presentation.tex  # Run 2x!
```

#### Troubleshooting:
- **Error: "pdflatex not found"**
  → Restart terminal atau tambahkan MiKTeX ke PATH

- **Error: "Package not found"**
  → MiKTeX akan popup "Install package?" → Klik "Yes"
  → Atau setting MiKTeX: "Install packages on-the-fly: Yes"

- **Compile lambat**
  → Normal untuk first compile (download packages)
  → Compile berikutnya lebih cepat

---

### Linux

```bash
# Install TeX Live
sudo apt-get update
sudo apt-get install texlive-full

# Compile
cd presentation
pdflatex parkinson_detection_presentation.tex
pdflatex parkinson_detection_presentation.tex

# Clean auxiliary files
rm *.aux *.log *.nav *.out *.snm *.toc *.vrb
```

---

### macOS

```bash
# Install MacTeX
brew install --cask mactex

# Compile
cd presentation
pdflatex parkinson_detection_presentation.tex
pdflatex parkinson_detection_presentation.tex
```

---

## VS Code Extension (Untuk Developer)

### Setup:
1. Install extension "LaTeX Workshop" (James Yu)
2. Install LaTeX distribution (MiKTeX/TeX Live/MacTeX)
3. Buka file `.tex`
4. Press `Ctrl+Alt+B` atau klik "Build LaTeX project"

### Keuntungan:
- Auto-compile saat save
- Integrated PDF preview
- Error highlighting
- Code completion

---

## Perbandingan Metode

| Metode | Waktu Setup | Waktu Compile | Space | Difficulty |
|--------|-------------|---------------|-------|------------|
| **Overleaf** | 2 menit | 10 detik | 0 | ⭐ Easy |
| MiKTeX | 15 menit | 30 detik | 4GB | ⭐⭐ Medium |
| TeX Live | 30 menit | 30 detik | 6GB | ⭐⭐⭐ Hard |
| VS Code | 20 menit | 20 detik | 4GB | ⭐⭐ Medium |

---

## REKOMENDASI

### Untuk Sekali Pakai:
→ **OVERLEAF** (paling cepat, no install)

### Untuk Sering Pakai LaTeX:
→ **MiKTeX + VS Code** (lebih productive)

### Untuk Kolaborasi:
→ **Overleaf** (real-time collaboration)

---

## Output File

Setelah compile berhasil, Anda akan mendapatkan:

**File:** `parkinson_detection_presentation.pdf`
- Format: PDF
- Ukuran: ~500KB - 1MB
- Aspect ratio: 16:9 (landscape)
- Total slides: 39
- Ready untuk presentasi!

---

## Tips Presentasi

1. **Test di Proyektor**
   - Font size cukup besar? ✓
   - Warna kontras? ✓
   - Semua visualisasi muncul? ✓

2. **Prepare Handout**
   - Print 6 slides per halaman
   - Bagikan ke audience

3. **Timing**
   - Full presentation: 30-40 menit
   - Dengan Q&A: 45-50 menit

4. **Backup**
   - Simpan PDF di Google Drive/OneDrive
   - Siapkan USB drive backup
   - Email ke diri sendiri

---

## Troubleshooting Umum

### "Package tikz not found"
→ MiKTeX akan auto-install, klik "Yes"
→ Atau manual: MiKTeX Console → Packages → Search "tikz" → Install

### "Overfull \hbox" warning
→ Abaikan, tidak mempengaruhi visual

### PDF tidak muncul setelah compile
→ Check file `.log` untuk error messages
→ Coba compile di Overleaf untuk isolate problem

### Visualisasi tidak muncul
→ Pastikan compile 2x (untuk TikZ/PGFPlots)
→ Check log: "Output written on ..."

---

## Support

Jika masih ada masalah:
1. Check file `PANDUAN_LENGKAP.md` di folder presentation
2. Google error message + "beamer"
3. StackExchange: https://tex.stackexchange.com
4. Overleaf help: https://www.overleaf.com/learn

---

**Good luck!** 🎉
