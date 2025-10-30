// =====================================================
// CURVED MONITORING + 4x MPU6050 via TCA9548A + ILI9341 + XPT2046 + ANFIS
// =====================================================
// Sistem deteksi kelainan tulang belakang menggunakan ANFIS
// Layout dan tampilan sesuai versi sebelumnya
// Sensor data: real-time Pitch (X), Roll (Y), Yaw (Z)
// =====================================================

#include <Wire.h>
#include <SPI.h>
#include <LittleFS.h>
#include <MPU6050_light.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>
#include <math.h>
#include <esp_task_wdt.h>
#include "anfis_model.h"
#include "features.h"

// Uncomment to simulate sensor data when hardware is unavailable
//#define SIMULATE_SENSORS

// ==== PIN DEFINISI ====
#define TFT_CS   13
#define TFT_DC   14
#define TFT_RST  12
#define TFT_MOSI 23
#define TFT_CLK  18
#define TFT_MISO 19
#define TFT_LED  25
#define TOUCH_CS 15
#define TOUCH_IRQ -1

// ==== TOUCH CALIBRATION ====
#define TS_MINX 580
#define TS_MAXX 3712
#define TS_MINY 512
#define TS_MAXY 3641

// ==== OBJEK TFT & TOUCH ====
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen ts(TOUCH_CS, TOUCH_IRQ);

enum ScreenState { HOME, MONITOR, RESULT };
ScreenState currentScreen = HOME;

// ==== SENSOR SETUP ====
#define NUM_SENSORS 4
#define TCAADDR 0x70

constexpr uint8_t I2C_SDA_PIN = 21;
constexpr uint8_t I2C_SCL_PIN = 22;
constexpr uint32_t I2C_CLOCK_HZ = 400000;
constexpr uint8_t MPU_ADDR = 0x68;

MPU6050 mpu0(Wire);
MPU6050 mpu1(Wire);
MPU6050 mpu2(Wire);
MPU6050 mpu3(Wire);
MPU6050* mpu[NUM_SENSORS] = { &mpu0, &mpu1, &mpu2, &mpu3 };

const char* label[NUM_SENSORS] = { "CERVICAL", "THORACAL", "LUMBAL", "SACRUM" };

float angleX[NUM_SENSORS], angleY[NUM_SENSORS], angleZ[NUM_SENSORS];
float preInterval[NUM_SENSORS];
float filterGyroCoef = 0.98;
bool sensorReady[NUM_SENSORS] = {false};
bool tcaAvailable = false;
uint8_t tcaAddress = TCAADDR;

// ==== ANFIS MODEL ====
ANFIS anfisModel;
String lastPrediction = "";
float lastScores[8] = {0};  // 8 kelas

// ==== MULTIPLEXER ====
bool tcaSelect(uint8_t i) {
  if (i > 7) return false;
  Wire.beginTransmission(tcaAddress);
  Wire.write(1 << i);
  bool ok = (Wire.endTransmission() == 0);
  if (ok) {
    delayMicroseconds(300);  // gives the mux time to switch channels
  }
  return ok;
}

void scanI2CBus() {
  Serial.println("-- I2C scan mulai --");
  int found = 0;
  for (uint8_t address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      Serial.printf("  Ditemukan perangkat di 0x%02X\n", address);
      found++;
    }
  }
  if (found == 0) {
    Serial.println("  Tidak ada perangkat I2C terdeteksi.");
  }
  Serial.println("-- I2C scan selesai --\n");
}

// ==== TOUCH BUTTON ====
bool isTouchedButton(int x, int y, int w, int h, TS_Point p) {
  int tx = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  int ty = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
  return (tx > x && tx < x + w && ty > y && ty < y + h);
}

// ==== HALAMAN: KALIBRASI ====
void showCalibrationScreen() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(2);
  tft.setCursor(40, 60);
  tft.println("Kalibrasi sensor...");
  tft.setTextSize(1);
  tft.setCursor(40, 85);
  tft.println("Harap tunggu beberapa saat");

  // Progress bar
  int barX = 30, barY = 120, barWidth = 260, barHeight = 20;
  tft.drawRect(barX, barY, barWidth, barHeight, ILI9341_WHITE);
}

// ==== UPDATE PROGRESS BAR ====
void updateProgressBar(int progress) {
  int barX = 30, barY = 120, barWidth = 260, barHeight = 20;
  int innerWidth = barWidth - 4;
  progress = constrain(progress, 0, 100);
  int filledWidth = map(progress, 0, 100, 0, innerWidth);

  // Clear previous fill area before drawing the new width
  tft.fillRect(barX + 2, barY + 2, innerWidth, barHeight - 4, ILI9341_BLACK);
  if (filledWidth > 0) {
    tft.fillRect(barX + 2, barY + 2, filledWidth, barHeight - 4, ILI9341_CYAN);
  }

  // Tampilkan persentase
  tft.fillRect(barX + 120, barY + 35, 50, 15, ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.setCursor(barX + 130, barY + 37);
  tft.printf("%d%%", progress);
}

// ==== FUNGSI KALIBRASI SEMUA SENSOR ====
void calibrateAllSensors() {
#ifdef SIMULATE_SENSORS
  Serial.println("SIMULATE_SENSORS: kalibrasi dilewati.");
  showCalibrationScreen();
  updateProgressBar(100);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setCursor(40, 150);
  tft.print("Mode simulasi aktif");
  delay(800);
  return;
#endif

  Serial.println("=== Kalibrasi semua sensor ===");
  showCalibrationScreen();
  updateProgressBar(0);

  if (!tcaAvailable) {
    Serial.println("TCA9548A tidak terdeteksi, kalibrasi dilewati.");
    tft.setTextColor(ILI9341_RED);
    tft.setCursor(40, 150);
    tft.print("Multiplexer tidak terdeteksi");
    delay(1200);
    return;
  }

  int readyIdx[NUM_SENSORS];
  int readyCount = 0;
  for (int i = 0; i < NUM_SENSORS; i++) {
    if (sensorReady[i]) {
      readyIdx[readyCount++] = i;
    }
  }

  if (readyCount == 0) {
    Serial.println("Tidak ada sensor yang siap untuk kalibrasi.");
    tft.setTextColor(ILI9341_RED);
    tft.setCursor(40, 150);
    tft.print("Sensor tidak terdeteksi");
    delay(1200);
    return;
  }

  for (int idx = 0; idx < readyCount; idx++) {
    int sensorIndex = readyIdx[idx];
    Serial.printf("Kalibrasi %s...\n", label[sensorIndex]);

    if (!tcaSelect(sensorIndex)) {
      Serial.printf("  -> Gagal memilih channel untuk %s\n", label[sensorIndex]);
      sensorReady[sensorIndex] = false;
    } else {
      mpu[sensorIndex]->calcOffsets(true, true);
    }

    int targetProgress = ((idx + 1) * 100) / readyCount;
    updateProgressBar(targetProgress);
    delay(10);
  }

  updateProgressBar(100);
  Serial.println("Kalibrasi selesai!");
  delay(700);
}

// ==== DRAW HOME ====
void drawHome() {
  tft.fillScreen(ILI9341_WHITE);
  int16_t x1, y1; uint16_t w, h;

  String title1 = "WELCOME";
  tft.setTextColor(ILI9341_DARKGREY); tft.setTextSize(3);
  tft.getTextBounds(title1, 0, 0, &x1, &y1, &w, &h);
  tft.setCursor((tft.width() - w) / 2, 20); tft.print(title1);

  String title2 = "CURVED";
  tft.setTextColor(ILI9341_BLACK); tft.setTextSize(4);
  tft.getTextBounds(title2, 0, 0, &x1, &y1, &w, &h);
  tft.setCursor((tft.width() - w) / 2, 70); tft.print(title2);

  String subtitle1 = "Curvature Disorder", subtitle2 = "Early Detection";
  tft.setTextColor(ILI9341_DARKGREY); tft.setTextSize(2);
  tft.getTextBounds(subtitle1, 0, 0, &x1, &y1, &w, &h);
  tft.setCursor((tft.width() - w) / 2, 130); tft.print(subtitle1);
  tft.getTextBounds(subtitle2, 0, 0, &x1, &y1, &w, &h);
  tft.setCursor((tft.width() - w) / 2, 150); tft.print(subtitle2);

  int btnW = 120, btnH = 45, btnX = (tft.width() - btnW) / 2, btnY = 185;
  tft.fillRoundRect(btnX, btnY, btnW, btnH, 8, ILI9341_BLUE);
  tft.drawRoundRect(btnX, btnY, btnW, btnH, 8, ILI9341_NAVY);
  tft.setTextColor(ILI9341_WHITE); tft.setTextSize(2);
  String btnLabel = "START";
  tft.getTextBounds(btnLabel, 0, 0, &x1, &y1, &w, &h);
  tft.setCursor((tft.width() - w) / 2, btnY + (btnH - h) / 2 + 4);
  tft.print(btnLabel);
}

// ==== DRAW MONITOR ====
void drawMonitoring(float data[NUM_SENSORS][3]) {
  int16_t x1, y1; uint16_t w, h;
  tft.fillScreen(ILI9341_WHITE);
  tft.fillRect(0, 0, tft.width(), 35, ILI9341_BLACK);
  String header = "DATA MONITORING";
  tft.setTextColor(ILI9341_WHITE); tft.setTextSize(2);
  tft.getTextBounds(header, 0, 0, &x1, &y1, &w, &h);
  tft.setCursor((tft.width() - w) / 2, 10); tft.print(header);

  int boxW = 140, boxH = 70, gapX = 10, gapY = 5;
  int startX1 = 20, startX2 = startX1 + boxW + gapX;
  int startY1 = 50, startY2 = startY1 + boxH + gapY;

  auto drawBox = [&](int x, int y, String title, float xVal, float yVal, float zVal) {
    tft.drawRect(x, y, boxW, boxH, ILI9341_BLACK);
    tft.setTextColor(ILI9341_BLACK); tft.setTextSize(1);
    tft.getTextBounds(title, 0, 0, &x1, &y1, &w, &h);
    tft.setCursor(x + (boxW - w) / 2, y + 3); tft.print(title);
    tft.drawLine(x, y + 15, x + boxW, y + 15, ILI9341_BLACK);
    auto printAxis = [&](int16_t offsetY, char axis, float value) {
      tft.setCursor(x + 8, y + offsetY);
      if (isnan(value)) {
        tft.printf("%c: --", axis);
      } else {
        tft.printf("%c: %.2f", axis, value);
      }
    };
    printAxis(25, 'x', xVal);
    printAxis(38, 'y', yVal);
    printAxis(51, 'z', zVal);
  };

  drawBox(startX1, startY1, "CERVICAL", data[0][0], data[0][1], data[0][2]);
  drawBox(startX2, startY1, "THORACAL", data[1][0], data[1][1], data[1][2]);
  drawBox(startX1, startY2, "LUMBAL",   data[2][0], data[2][1], data[2][2]);
  drawBox(startX2, startY2, "SACRUM",   data[3][0], data[3][1], data[3][2]);

  int btnW = 110, btnH = 35, btnX = (tft.width() - btnW) / 2, btnY = 200;
  tft.fillRoundRect(btnX, btnY, btnW, btnH, 8, ILI9341_BLUE);
  tft.drawRoundRect(btnX, btnY, btnW, btnH, 8, ILI9341_NAVY);
  String btnLabel = "CLASSIFY";
  tft.setTextColor(ILI9341_WHITE); tft.setTextSize(2);
  tft.getTextBounds(btnLabel, 0, 0, &x1, &y1, &w, &h);
  tft.setCursor((tft.width() - w) / 2, btnY + (btnH - h) / 2 + 3); tft.print(btnLabel);
}

// ==== DRAW RESULT SCREEN ====
void drawResultScreen(String prediction, float scores[], int numClasses) {
  int16_t x1, y1; uint16_t w, h;
  tft.fillScreen(ILI9341_WHITE);
  
  // Header
  tft.fillRect(0, 0, tft.width(), 35, ILI9341_BLACK);
  String header = "HASIL KLASIFIKASI";
  tft.setTextColor(ILI9341_WHITE); tft.setTextSize(2);
  tft.getTextBounds(header, 0, 0, &x1, &y1, &w, &h);
  tft.setCursor((tft.width() - w) / 2, 10); tft.print(header);

  // Prediction result box
  int boxX = 20, boxY = 50, boxW = 280, boxH = 80;
  tft.fillRoundRect(boxX, boxY, boxW, boxH, 8, ILI9341_LIGHTGREY);
  tft.drawRoundRect(boxX, boxY, boxW, boxH, 8, ILI9341_BLACK);
  
  tft.setTextColor(ILI9341_BLACK); tft.setTextSize(1);
  tft.setCursor(boxX + 10, boxY + 10);
  tft.print("Diagnosis:");
  
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_RED);
  
  // Word wrap untuk nama panjang
  int cursorY = boxY + 30;
  int maxWidth = boxW - 20;
  String words[10];
  int wordCount = 0;
  
  // Split prediction by spaces
  int lastSpace = 0;
  for (int i = 0; i <= prediction.length(); i++) {
    if (i == prediction.length() || prediction[i] == ' ') {
      if (i > lastSpace) {
        words[wordCount++] = prediction.substring(lastSpace, i);
      }
      lastSpace = i + 1;
    }
  }
  
  String line = "";
  for (int i = 0; i < wordCount; i++) {
    String testLine = line + (line.length() > 0 ? " " : "") + words[i];
    tft.getTextBounds(testLine, 0, 0, &x1, &y1, &w, &h);
    
    if (w > maxWidth && line.length() > 0) {
      tft.setCursor(boxX + 10, cursorY);
      tft.print(line);
      cursorY += h + 5;
      line = words[i];
    } else {
      line = testLine;
    }
  }
  
  if (line.length() > 0) {
    tft.setCursor(boxX + 10, cursorY);
    tft.print(line);
  }

  // Confidence scores (top 3)
  tft.setTextColor(ILI9341_BLACK); tft.setTextSize(1);
  tft.setCursor(20, 145);
  tft.print("Confidence Score (Top 3):");

  // Find top scores while guarding against <3 classes
  int maxTop = min(3, numClasses);
  int topIdx[3] = {0, 0, 0};
  for (int i = 0; i < maxTop; i++) {
    topIdx[i] = i;
  }
  for (int i = 0; i < numClasses; i++) {
    for (int j = 0; j < maxTop; j++) {
      if (scores[i] > scores[topIdx[j]]) {
        for (int k = maxTop - 1; k > j; k--) topIdx[k] = topIdx[k-1];
        topIdx[j] = i;
        break;
      }
    }
  }

  int barY = 165;
  for (int i = 0; i < maxTop; i++) {
    int idx = topIdx[i];
    String className = anfisModel.className(idx);
    float score = constrain(scores[idx], 0.0f, 1.0f) * 100.0f;

    tft.setCursor(25, barY);
    tft.printf("%d. %s", i + 1, className.c_str());

    // Progress bar
    int barX = 25, barLen = 200, barH = 8;
    tft.drawRect(barX, barY + 12, barLen, barH, ILI9341_BLACK);
    int rawFill = (int)(barLen * constrain(scores[idx], 0.0f, 1.0f));
    uint16_t color = (i == 0) ? ILI9341_RED : ILI9341_BLUE;
    if (rawFill > 2) {
      tft.fillRect(barX + 1, barY + 13, rawFill - 2, barH - 2, color);
    }

    tft.setCursor(barX + barLen + 5, barY + 10);
    tft.printf("%.1f%%", score);

    barY += 25;
  }

  // Back button
  int btnW = 80, btnH = 35, btnX = (tft.width() - btnW) / 2, btnY = 280;
  tft.fillRoundRect(btnX, btnY, btnW, btnH, 8, ILI9341_BLUE);
  tft.drawRoundRect(btnX, btnY, btnW, btnH, 8, ILI9341_NAVY);
  String btnLabel = "BACK";
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.getTextBounds(btnLabel, 0, 0, &x1, &y1, &w, &h);
  tft.setCursor((tft.width() - w) / 2, btnY + (btnH - h) / 2 + 3);
  tft.print(btnLabel);  
}

// ==== SENSOR INIT ====
void initSensors() {
#ifdef SIMULATE_SENSORS
  Serial.println("SIMULATE_SENSORS: melewati inisialisasi hardware.");
  for (int i = 0; i < NUM_SENSORS; i++) {
    sensorReady[i] = true;
    angleX[i] = angleY[i] = angleZ[i] = 0.0f;
    preInterval[i] = millis();
  }
  tcaAvailable = false;
  return;
#endif

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Wire.setClock(I2C_CLOCK_HZ);
  Serial.println("Inisialisasi sensor...");

  for (int i = 0; i < NUM_SENSORS; i++) {
    sensorReady[i] = false;
    angleX[i] = angleY[i] = angleZ[i] = 0.0f;
    preInterval[i] = millis();
  }

  tcaAvailable = false;
  tcaAddress = TCAADDR;
  Wire.beginTransmission(tcaAddress);
  if (Wire.endTransmission() == 0) {
    tcaAvailable = true;
  } else {
    // Coba alamat alternatif 0x70-0x77
    for (uint8_t addr = 0x70; addr <= 0x77; addr++) {
      Wire.beginTransmission(addr);
      if (Wire.endTransmission() == 0) {
        tcaAddress = addr;
        tcaAvailable = true;
        Serial.printf("TCA9548A ditemukan di alamat alternatif 0x%02X\n", addr);
        break;
      }
    }
  }

  if (!tcaAvailable) {
    Serial.println("TCA9548A tidak ditemukan di jalur I2C. Pastikan koneksi sudah benar.");
    scanI2CBus();
    return;
  }

  for (int i = 0; i < NUM_SENSORS; i++) {
    if (!tcaSelect(i)) {
      Serial.printf("%s gagal dipilih melalui TCA.\n", label[i]);
      continue;
    }

    // Add small delay for channel switching
    delay(10);
    
    Wire.beginTransmission(MPU_ADDR);
    if (Wire.endTransmission(true) != 0) {
      Serial.printf("%s tidak merespon di alamat 0x%02X.\n", label[i], MPU_ADDR);
      
      // Try alternative I2C address (AD0 = HIGH)
      Wire.beginTransmission(0x69);
      if (Wire.endTransmission(true) == 0) {
        Serial.printf("%s ditemukan di alamat alternatif 0x69.\n", label[i]);
        // Note: You'd need to modify MPU6050_light library to use 0x69
      }
      continue;
    }

    byte status = mpu[i]->begin();
    if (status == 0) {
      sensorReady[i] = true;
      Serial.printf("%s siap!\n", label[i]);
    } else {
      Serial.printf("%s gagal! Kode: %d\n", label[i], status);
    }
    preInterval[i] = millis();
  }

  int readyCount = 0;
  for (int i = 0; i < NUM_SENSORS; i++) {
    if (sensorReady[i]) readyCount++;
  }
  Serial.printf("Sensor terdeteksi: %d dari %d\n\n", readyCount, NUM_SENSORS);
}

// ==== BACA SENSOR ====
void readSensors(float data[NUM_SENSORS][3]) {
#ifdef SIMULATE_SENSORS
  static float phase = 0.0f;
  for (int i = 0; i < NUM_SENSORS; i++) {
    float localPhase = phase + i * 0.6f;
    angleX[i] = 5.0f * sinf(localPhase);
    angleY[i] = 3.0f * cosf(localPhase * 0.9f);
    angleZ[i] = 10.0f * sinf(localPhase * 0.5f);
    data[i][0] = angleX[i];
    data[i][1] = angleY[i];
    data[i][2] = angleZ[i];
  }
  phase += 0.05f;
  if (phase > 6.283185f) phase -= 6.283185f;
  return;
#endif

  for (int i = 0; i < NUM_SENSORS; i++) {
    if (!tcaAvailable || !sensorReady[i]) {
      data[i][0] = data[i][1] = data[i][2] = NAN;
      continue;
    }

    if (!tcaSelect(i)) {
      Serial.printf("%s: gagal memilih channel TCA, tandai sensor offline.\n", label[i]);
      sensorReady[i] = false;
      angleX[i] = angleY[i] = angleZ[i] = 0.0f;
      data[i][0] = data[i][1] = data[i][2] = NAN;
      continue;
    }

    Wire.beginTransmission(MPU_ADDR);
    if (Wire.endTransmission(true) != 0) {
      Serial.printf("%s: tidak merespon, periksa kabel/power.\n", label[i]);
      sensorReady[i] = false;
      angleX[i] = angleY[i] = angleZ[i] = 0.0f;
      data[i][0] = data[i][1] = data[i][2] = NAN;
      continue;
    }

    mpu[i]->fetchData();

    float accX = mpu[i]->getAccX();
    float accY = mpu[i]->getAccY();
    float accZ = mpu[i]->getAccZ();
    float gyroX = mpu[i]->getGyroX();
    float gyroY = mpu[i]->getGyroY();
    float gyroZ = mpu[i]->getGyroZ();

    float sgZ = (accZ >= 0) - (accZ < 0);
    float angleAccX = atan2(accY, sgZ * sqrt(accZ * accZ + accX * accX)) * RAD_TO_DEG;
    float angleAccY = atan2(accX, sqrt(accZ * accZ + accY * accY)) * RAD_TO_DEG;

    unsigned long Tnew = millis();
    float dt = (Tnew - preInterval[i]) * 1e-3;
    preInterval[i] = Tnew;

    angleX[i] = filterGyroCoef * (angleX[i] + gyroX * dt) + (1 - filterGyroCoef) * angleAccX;
    angleY[i] = filterGyroCoef * (angleY[i] + gyroY * dt) + (1 - filterGyroCoef) * angleAccY;
    angleZ[i] += gyroZ * dt;
    // Keep yaw within [-180, 180] for display stability
    if (angleZ[i] > 180.0f || angleZ[i] < -180.0f) {
      angleZ[i] = fmodf(angleZ[i] + 180.0f, 360.0f);
      if (angleZ[i] < 0.0f) angleZ[i] += 360.0f;
      angleZ[i] -= 180.0f;
    }

    data[i][0] = angleX[i];
    data[i][1] = angleY[i];
    data[i][2] = angleZ[i];
  }
}

// ==== PERFORM CLASSIFICATION ====
void performClassification() {
  Serial.println("\n=== Melakukan Klasifikasi ===");

  // Check sensor status
  int missingSensors = 0;
  for (int i = 0; i < NUM_SENSORS; i++) {
    if (!sensorReady[i]) missingSensors++;
  }

  if (missingSensors > 0) {
    Serial.printf("Klasifikasi dibatalkan: %d sensor offline.\n", missingSensors);
    
    // Show error on screen
    tft.fillRect(20, 170, 280, 30, ILI9341_WHITE);
    tft.setTextColor(ILI9341_RED);
    tft.setTextSize(1);
    tft.setCursor(25, 175);
    tft.printf("Error: %d sensor offline", missingSensors);
    tft.setCursor(25, 190);
    tft.print("Check MPU6050 connections");
    delay(2000);
    
    // Clear error message
    tft.fillRect(20, 170, 280, 30, ILI9341_WHITE);
    return;
  }

  // ANFIS model is temporarily disabled
  Serial.println("Klasifikasi dibatalkan: ANFIS model dinonaktifkan sementara.");
  
  // Show info message
  tft.fillRect(20, 170, 280, 30, ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLUE);
  tft.setTextSize(1);
  tft.setCursor(25, 175);
  tft.print("Classification temporarily disabled");
  tft.setCursor(25, 190);
  tft.print("Sensor data shown below:");
  
  // Show current sensor values
  tft.setCursor(25, 205);
  tft.printf("C:%.1f L:%.1f S:%.1f", angleX[0], angleX[2], angleX[3]);
  
  delay(3000);
  
  // Clear message
  tft.fillRect(20, 170, 280, 50, ILI9341_WHITE);
  return;
  
  // Show processing message
  tft.fillRect(20, 170, 280, 30, ILI9341_WHITE);
  tft.setTextColor(ILI9341_BLUE);
  tft.setTextSize(1);
  tft.setCursor(25, 175);
  tft.print("Processing classification...");
  
  // Ekstrak fitur dari data sensor
  SpineFeatures features = FeatureExtractor::fromSensorData(angleX, angleY, angleZ);
  
  Serial.println("Fitur yang diekstrak:");
  Serial.printf("  ccx (Cervical X): %.2f\n", features.ccx);
  Serial.printf("  tcx (Thoracal X): %.2f\n", features.tcx);
  Serial.printf("  tcz (Thoracal Z): %.2f\n", features.tcz);
  Serial.printf("  lcx (Lumbal X): %.2f\n", features.lcx);
  Serial.printf("  lcz (Lumbal Z): %.2f\n", features.lcz);
  Serial.printf("  scx (Sacrum X): %.2f\n", features.scx);
  
  // Konversi ke array
  float featureArray[6];
  features.toArray(featureArray);
  
  // Prediksi menggunakan ANFIS
  int predictedClass = anfisModel.predict(featureArray, lastScores, true);
  
  if (predictedClass >= 0) {
    lastPrediction = anfisModel.className(predictedClass);
    Serial.printf("\nHasil Prediksi: %s\n", lastPrediction.c_str());
    Serial.println("\nSkor semua kelas:");
    for (size_t i = 0; i < anfisModel.numClasses(); i++) {
      Serial.printf("  %s: %.4f (%.2f%%)\n", 
                    anfisModel.className(i).c_str(), 
                    lastScores[i],
                    lastScores[i] * 100.0f);
    }
    
    // Pindah ke layar hasil
    currentScreen = RESULT;
    drawResultScreen(lastPrediction, lastScores, anfisModel.numClasses());
  } else {
    Serial.println("Error: Prediksi gagal!");
    
    // Show prediction error
    tft.fillRect(20, 170, 280, 30, ILI9341_WHITE);
    tft.setTextColor(ILI9341_RED);
    tft.setTextSize(1);
    tft.setCursor(25, 175);
    tft.print("Error: Classification failed");
    tft.setCursor(25, 190);
    tft.print("Check sensor data quality");
    delay(2000);
    
    // Clear error message
    tft.fillRect(20, 170, 280, 30, ILI9341_WHITE);
  }
}

// ==== SETUP ====
void setup() {
  Serial.begin(115200);
  delay(1000); // Give serial time to initialize
  
  Serial.println("\n=== CURVED - Spine Disorder Detection ===");
  Serial.println("Integrating LCD + MPU + ANFIS Model");
  
  // Init TFT & Touch
  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH);
  
  SPI.begin(TFT_CLK, TFT_MISO, TFT_MOSI);
  pinMode(TFT_CS, OUTPUT);    digitalWrite(TFT_CS, HIGH);
  pinMode(TOUCH_CS, OUTPUT);  digitalWrite(TOUCH_CS, HIGH);
  
  tft.begin();
  // LCD OK, skip test
  tft.fillScreen(ILI9341_BLACK);
  ts.begin();
  ts.setRotation(1);
  tft.setRotation(1);
  
  // Init Sensors
  initSensors();
  
  // Kalibrasi sensors
  calibrateAllSensors();
  
  // ANFIS Model Loading dengan debugging lengkap
  Serial.println("\n=== ANFIS Model Loading ===");
  Serial.println("Step 1: Checking LittleFS...");
  
  if (!LittleFS.begin()) {
    Serial.println("✗ LittleFS mount failed!");
    Serial.println("→ Jalankan: pio run -t uploadfs");
    Serial.println("→ Untuk upload file anfis_export.json ke ESP32");
  } else {
    Serial.println("✓ LittleFS mounted successfully");
    
    // List files in LittleFS
    Serial.println("Files in LittleFS:");
    File root = LittleFS.open("/");
    File file = root.openNextFile();
    while(file) {
      Serial.printf("  - %s (%d bytes)\n", file.name(), file.size());
      file = root.openNextFile();
    }
    
    // Try loading ANFIS model
    Serial.println("Step 2: Loading ANFIS model...");
    if (anfisModel.beginFromFile("/anfis_export.json")) {
      Serial.println("✓ ANFIS model loaded successfully!");
      Serial.printf("✓ Model has %d classes\n", anfisModel.numClasses());
      for (size_t i = 0; i < anfisModel.numClasses(); i++) {
        Serial.printf("  - Class %d: %s\n", i, anfisModel.className(i).c_str());
      }
      Serial.println("✓ Classification system ready");
    } else {
      Serial.println("✗ ANFIS model loading failed");
      Serial.println("→ Check if anfis_export.json exists in data folder");
      Serial.println("→ Run: pio run -t uploadfs to upload filesystem");
    }
  }
  
  // Initialize LCD home screen
  Serial.println("Initializing LCD home screen...");
  currentScreen = HOME;
  drawHome();
  
  Serial.println("Setup complete - system ready!");
  Serial.println("Touch screen to interact with device");
}

// ==== LOOP ====
void loop() {
  static float sensorData[NUM_SENSORS][3];
  static unsigned long lastUpdate = 0;
  static unsigned long lastLCDUpdate = 0;
  unsigned long currentTime = millis();

  // Read sensor data
  if (currentTime - lastUpdate >= 500) {
    readSensors(sensorData);
    
    // Print sensor data to Serial for debugging
    Serial.printf("[%8lu] ", currentTime);
    for (int i = 0; i < NUM_SENSORS; i++) {
      if (sensorReady[i]) {
        Serial.printf("%s(%.1f,%.1f,%.1f) ", label[i], 
                     sensorData[i][0], sensorData[i][1], sensorData[i][2]);
      } else {
        Serial.printf("%s(OFF) ", label[i]);
      }
    }
    Serial.println();
    
    lastUpdate = currentTime;
  }
  
  // Handle touch input and screen updates
  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    delay(100); // Debounce
    
    if (currentScreen == HOME) {
      // Check if START button is pressed (approximate button area)
      int btnX = (tft.width() - 120) / 2, btnY = 185, btnW = 120, btnH = 45;
      if (isTouchedButton(btnX, btnY, btnW, btnH, p)) {
        currentScreen = MONITOR;
        drawMonitoring(sensorData);
        Serial.println("Switched to MONITOR screen");
      }
    } else if (currentScreen == MONITOR) {
      // Check if CLASSIFY button is pressed
      int btnX = (tft.width() - 110) / 2, btnY = 200, btnW = 110, btnH = 35;
      if (isTouchedButton(btnX, btnY, btnW, btnH, p)) {
        performClassification();
      }
    } else if (currentScreen == RESULT) {
      // Check if BACK button is pressed
      int btnX = (tft.width() - 100) / 2, btnY = 200, btnW = 100, btnH = 35;
      if (isTouchedButton(btnX, btnY, btnW, btnH, p)) {
        currentScreen = MONITOR;
        drawMonitoring(sensorData);
        Serial.println("Returned to MONITOR screen");
      }
    }
  }
  
  // Update monitor screen if currently displaying
  if (currentScreen == MONITOR && currentTime - lastLCDUpdate >= 1000) {
    drawMonitoring(sensorData);
    lastLCDUpdate = currentTime;
  }
  
  // Periodic status update to Serial
  if (currentTime - lastUpdate >= 5000) {
    Serial.println("\n=== CURVED System Status ===");
    Serial.printf("Current Screen: %s\n", 
                  currentScreen == HOME ? "HOME" : 
                  currentScreen == MONITOR ? "MONITOR" : "RESULT");
    Serial.printf("ANFIS Model: %s\n", 
                  anfisModel.numClasses() > 0 ? "LOADED" : "NOT LOADED");
    Serial.printf("Active Sensors: %d/%d\n", 
                  (sensorReady[0] ? 1 : 0) + (sensorReady[1] ? 1 : 0) + 
                  (sensorReady[2] ? 1 : 0) + (sensorReady[3] ? 1 : 0), NUM_SENSORS);
    Serial.println("===============================\n");
  }
}