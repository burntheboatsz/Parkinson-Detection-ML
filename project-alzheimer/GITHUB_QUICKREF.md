# 🚀 GitHub Quick Reference

## Pertama Kali Upload

### Otomatis (MUDAH):
```powershell
.\upload_to_github.bat
```

### Manual:
```powershell
git add .
git commit -m "Initial commit: Parkinson Detection ML - Nafiz Ahmadin Harily (122430051)"
git remote add origin https://github.com/[USERNAME]/parkinson-detection-ml.git
git push -u origin main
```

---

## Update Setelah Perubahan

### Otomatis (MUDAH):
```powershell
.\update_github.bat
```

### Manual:
```powershell
git add .
git commit -m "Update: deskripsi perubahan"
git push
```

---

## Command Penting

```powershell
# Cek status
git status

# Lihat history
git log --oneline

# Batalkan perubahan (sebelum commit)
git restore .

# Lihat remote URL
git remote -v

# Pull perubahan dari GitHub
git pull

# Clone repository
git clone https://github.com/[USERNAME]/[REPO].git
```

---

## Troubleshooting Cepat

### "Permission denied"
→ Gunakan Personal Access Token sebagai password

### "Remote origin already exists"
```powershell
git remote remove origin
git remote add origin [URL]
```

### "File too large"
→ Tambahkan ke `.gitignore` atau gunakan Git LFS

---

## Resources

- 📖 Panduan lengkap: `PANDUAN_GITHUB.md`
- 🌐 GitHub: https://github.com
- 💬 Help: https://docs.github.com
