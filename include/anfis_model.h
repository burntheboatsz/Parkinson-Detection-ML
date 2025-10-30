#pragma once
#include <Arduino.h>
#include <vector>

// Class untuk evaluasi model ANFIS Sugeno multi-class
// Model di-load dari file JSON di LittleFS
// Struktur JSON berisi:
// - features[], classes[], scaler_mean[], scaler_scale[]
// - premise.center[6][2], premise.sigma[6][2]
// - rules.index[R][6] (R=64)
// - consequents[NUM_CLASSES][R][7] -> [bias, w1..w6]

class ANFIS {
public:
    // Memuat model dari file JSON di LittleFS
    bool beginFromFile(const char *path="/anfis_export.json");
    
    // Melakukan prediksi berdasarkan 6 fitur input
    // x_raw: array 6 fitur mentah (akan dinormalisasi otomatis)
    // scores_out: optional, untuk menyimpan skor semua kelas
    // do_softmax: apakah menggunakan softmax pada output
    // return: index kelas dengan skor tertinggi
    int predict(const float x_raw[6], float *scores_out=nullptr, bool do_softmax=true) const;
    
    // Mendapatkan nama kelas berdasarkan index
    String className(size_t idx) const;
    
    // Mendapatkan jumlah kelas
    size_t numClasses() const { return classes.size(); }

private:
    // Scaler parameters untuk normalisasi input
    float mean[6]{};
    float scale[6]{};

    // Premise parameters (2 membership functions per feature)
    float center[6][2]{};  // center untuk gaussian MF
    float sigma[6][2]{};   // sigma untuk gaussian MF

    // Rule index (R x 6), menunjukkan MF mana yang digunakan per fitur
    std::vector<std::array<uint8_t,6>> rule_idx;

    // Consequent parameters: C x R x 7 (bias + 6 weights)
    struct Lin {
        float b;      // bias
        float w[6];   // weights untuk 6 fitur
    };
    std::vector<String> classes;           // label kelas
    std::vector<std::vector<Lin>> cons;    // [C][R] consequent parameters

    // Helper function untuk menghitung membership degree dengan Gaussian
    static inline float gauss(float x, float c, float s) {
        float z = (x - c) / s;
        return expf(-0.5f * z * z);
    }
};
