# CURVED - Sistem Deteksi Kelainan Tulang Belakang

Sistem deteksi dini kelainan tulang belakang menggunakan 4 sensor MPU6050 dan model ANFIS (Adaptive Neuro-Fuzzy Inference System).

## 🎯 Fitur

- **Real-time Monitoring**: Pembacaan data sensor dari 4 titik tulang belakang (Cervical, Thoracal, Lumbal, Sacrum)
- **Klasifikasi ANFIS**: Deteksi 8 kondisi tulang belakang menggunakan model ANFIS
- **Tampilan Visual**: LCD ILI9341 dengan touchscreen untuk interaksi user-friendly
- **Kalibrasi Otomatis**: Kalibrasi sensor otomatis dengan progress bar

## 📊 Kondisi yang Dapat Dideteksi

1. Normal
2. Kifosis Ringan
3. Lordosis Sedang
4. Skoliosis Ringan
5. Kifosis Sedang
6. Lordosis Berat
7. Skoliosis Sedang
8. Skoliosis Berat

## 🔧 Hardware yang Dibutuhkan

- ESP32 Development Board (ESP32-DOIT-DevKit-V1)
- 4x MPU6050 IMU Sensor
- 1x TCA9548A I2C Multiplexer
- ILI9341 TFT LCD Display (320x240)
- XPT2046 Touch Controller
- Kabel jumper dan breadboard

## 📌 Koneksi Pin

### TFT LCD (ILI9341)
- CS: GPIO 13
- DC: GPIO 14
- RST: GPIO 12
- MOSI: GPIO 23
- CLK: GPIO 18
- MISO: GPIO 19
- LED: GPIO 25

### Touchscreen (XPT2046)
- CS: GPIO 15

### I2C (MPU6050 + TCA9548A)
- SDA: GPIO 21 (default)
- SCL: GPIO 22 (default)

## 🚀 Cara Menggunakan

### 1. Persiapan Hardware
- Hubungkan semua sensor MPU6050 ke multiplexer TCA9548A
- Hubungkan TCA9548A ke ESP32 via I2C
- Hubungkan LCD dan touchscreen sesuai pinout di atas

### 2. Upload Firmware
```bash
# Install PlatformIO CLI atau gunakan PlatformIO IDE
pio run --target upload
```

### 3. Upload File Sistem (LittleFS)
```bash
# Upload file anfis_export.json ke filesystem ESP32
pio run --target uploadfs
```

### 4. Operasional
1. **Power On**: ESP32 akan boot dan melakukan kalibrasi sensor
2. **Home Screen**: Tekan tombol "START" untuk mulai monitoring
3. **Monitor Screen**: Lihat data real-time dari keempat sensor
4. **Klasifikasi**: Tekan tombol "CLASSIFY" untuk menjalankan deteksi ANFIS
5. **Result Screen**: Lihat hasil diagnosis dan confidence score
6. **Back**: Tekan "BACK" untuk kembali ke monitoring

## 📁 Struktur File

```
PKM_CURVED/
├── data/
│   └── anfis_export.json       # Model ANFIS (JSON format)
├── include/
│   ├── anfis_model.h           # Header class ANFIS
│   └── features.h              # Header ekstraksi fitur
├── src/
│   ├── main.cpp                # Program utama
│   └── anfis_model.cpp         # Implementasi ANFIS
├── platformio.ini              # Konfigurasi PlatformIO
└── README.md                   # Dokumentasi ini
```

## 🧠 Model ANFIS

Model ANFIS menggunakan 6 fitur input:
- **ccx**: Cervical Curve X (pitch)
- **tcx**: Thoracal Curve X (pitch)
- **tcz**: Thoracal Curve Z (roll)
- **lcx**: Lumbal Curve X (pitch)
- **lcz**: Lumbal Curve Z (roll)
- **scx**: Sacrum Curve X (pitch)

Model memiliki:
- **64 rules** (fuzzy rules)
- **2 membership functions** per fitur (Gaussian)
- **8 output classes** dengan consequent linear

## 🔍 Cara Kerja

1. **Sensor Reading**: MPU6050 membaca data accelerometer dan gyroscope
2. **Complementary Filter**: Menggabungkan data accel + gyro untuk sudut akurat
3. **Feature Extraction**: Ekstrak 6 fitur dari 4 sensor
4. **Normalization**: Normalisasi fitur menggunakan scaler (mean & std)
5. **Fuzzy Layer**: Hitung membership degree dengan Gaussian MF
6. **Rule Evaluation**: Aktivasi 64 fuzzy rules (product t-norm)
7. **Consequent Layer**: Hitung output per kelas (TSK model)
8. **Defuzzification**: Weighted average output
9. **Softmax**: Konversi ke probability distribution
10. **Classification**: Pilih kelas dengan skor tertinggi

## 📝 Kalibrasi Sensor

Kalibrasi otomatis dilakukan saat startup:
- Letakkan pasien dalam posisi berdiri tegak
- Tunggu progress bar mencapai 100%
- Sensor akan dikalibrasi untuk offset gyro dan accelerometer

## 🐛 Troubleshooting

### Model gagal di-load
- Pastikan file `anfis_export.json` ada di folder `data/`
- Pastikan sudah menjalankan `pio run --target uploadfs`
- Cek Serial Monitor untuk pesan error

### Sensor tidak terbaca
- Cek koneksi I2C (SDA, SCL)
- Pastikan TCA9548A dan MPU6050 terhubung dengan benar
- Cek alamat I2C dengan I2C scanner

### Layar touchscreen tidak responsif
- Kalibrasi ulang konstanta touch (TS_MINX, TS_MAXX, TS_MINY, TS_MAXY)
- Pastikan pin TOUCH_CS terhubung dengan benar

## 📚 Library Dependencies

- `rfetick/MPU6050_light` - Driver MPU6050
- `adafruit/Adafruit GFX Library` - Graphics library
- `adafruit/Adafruit ILI9341` - LCD driver
- `paulstoffregen/XPT2046_Touchscreen` - Touch driver
- `bblanchon/ArduinoJson` - JSON parsing

## 📄 Lisensi

Project ini dibuat untuk keperluan penelitian PKM (Program Kreativitas Mahasiswa).

## 👥 Tim Pengembang

- **Institut Teknologi Sumatera**
- Sistem deteksi kelainan tulang belakang menggunakan ANFIS

## 📧 Kontak

Untuk pertanyaan atau saran, silakan buat issue di repository ini.

---

**Selamat menggunakan CURVED! 🎉**
