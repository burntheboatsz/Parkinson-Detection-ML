#include <LittleFS.h>
#include <ArduinoJson.h>
#include "anfis_model.h"

bool ANFIS::beginFromFile(const char *path) {
    // Mount LittleFS filesystem (expect pre-uploaded image)
    if (!LittleFS.begin()) {
        Serial.println("[ANFIS] LittleFS mount failed. Upload data dengan `pio run -t uploadfs`.");
        return false;
    }
    
    // Open JSON file
    File f = LittleFS.open(path, "r");
    if (!f) { 
        Serial.println("[ANFIS] JSON file open failed"); 
        return false; 
    }

    // Allocate JSON document (32KB, sesuaikan jika file lebih besar)
    StaticJsonDocument<32*1024> doc;
    DeserializationError err = deserializeJson(doc, f);
    f.close();
    
    if (err) {
        Serial.printf("[ANFIS] JSON parse error: %s\n", err.c_str());
        return false;
    }

    // Load class labels
    classes.clear();
    JsonArray classesArray = doc["classes"];
    for (JsonVariant v : classesArray) {
        classes.emplace_back(String(v.as<const char*>()));
    }

    // Load scaler parameters (mean dan scale untuk normalisasi)
    for (int i = 0; i < 6; i++) {
        mean[i] = doc["scaler_mean"][i].as<float>();
        scale[i] = doc["scaler_scale"][i].as<float>();
    }

    // Load premise parameters (center dan sigma untuk Gaussian MF)
    for (int i = 0; i < 6; i++) {
        for (int m = 0; m < 2; m++) {
            center[i][m] = doc["premise"]["center"][i][m].as<float>();
            sigma[i][m] = doc["premise"]["sigma"][i][m].as<float>();
        }
    }

    // Load rule indices
    rule_idx.clear();
    JsonArray rulesArray = doc["rules"]["index"];
    for (JsonVariant r : rulesArray) {
        std::array<uint8_t, 6> rr{};
        for (int i = 0; i < 6; i++) {
            rr[i] = r[i].as<uint8_t>();
        }
        rule_idx.push_back(rr);
    }
    const size_t R = rule_idx.size();

    // Load consequent parameters: cons[C][R] of (b, w[6])
    cons.assign(classes.size(), std::vector<Lin>(R));
    for (size_t c = 0; c < classes.size(); ++c) {
        size_t r_idx = 0;
        JsonArray consequentArray = doc["consequents"][c];
        for (JsonVariant rv : consequentArray) {
            ANFIS::Lin lin{};
            // Format: [bias, w1, w2, w3, w4, w5, w6]
            lin.b = rv[0].as<float>();
            for (int k = 0; k < 6; k++) {
                lin.w[k] = rv[k+1].as<float>();
            }
            cons[c][r_idx++] = lin;
            if (r_idx >= R) break; // safety
        }
    }

    Serial.printf("[ANFIS] Model loaded: %d classes, %d rules\n", classes.size(), R);
    Serial.print("[ANFIS] Classes: ");
    for (size_t i = 0; i < classes.size(); i++) {
        Serial.print(classes[i]);
        if (i < classes.size() - 1) Serial.print(", ");
    }
    Serial.println();
    
    return true;
}

int ANFIS::predict(const float x_raw[6], float *scores_out, bool do_softmax) const {
    if (classes.empty() || rule_idx.empty()) {
        Serial.println("[ANFIS] Model not loaded");
        return -1;
    }

    // Step 1: Normalisasi input menggunakan scaler
    float x[6];
    for (int i = 0; i < 6; i++) {
        x[i] = (x_raw[i] - mean[i]) / scale[i];
    }

    // Step 2: Hitung membership degree untuk setiap rule (firing strength)
    const size_t R = rule_idx.size();
    float w[R];      // rule activation strengths
    float w_sum = 0.0f;
    
    for (size_t r = 0; r < R; r++) {
        w[r] = 1.0f;
        // Untuk setiap fitur di rule ini
        for (int i = 0; i < 6; i++) {
            uint8_t mf_idx = rule_idx[r][i];  // MF mana yang digunakan (0 atau 1)
            float mu = gauss(x[i], center[i][mf_idx], sigma[i][mf_idx]);
            w[r] *= mu;  // AND operation (product t-norm)
        }
        w_sum += w[r];
    }

    // Step 3: Hitung output untuk setiap kelas
    float raw_scores[classes.size()];
    for (size_t c = 0; c < classes.size(); c++) {
        float numerator = 0.0f;
        
        for (size_t r = 0; r < R; r++) {
            if (w_sum > 1e-10f) {  // Hindari pembagian dengan nol
                // Hitung linear consequent untuk rule dan kelas ini
                float consequent = cons[c][r].b;  // bias
                for (int i = 0; i < 6; i++) {
                    consequent += cons[c][r].w[i] * x[i];  // weighted inputs
                }
                numerator += w[r] * consequent;
            }
        }
        
        // TSK output: weighted average
        raw_scores[c] = (w_sum > 1e-10f) ? (numerator / w_sum) : 0.0f;
    }

    // Step 4: Apply softmax jika diminta (untuk mengkonversi ke probabilitas)
    if (do_softmax) {
        // Cari max untuk numerical stability
        float max_score = raw_scores[0];
        for (size_t c = 1; c < classes.size(); c++) {
            if (raw_scores[c] > max_score) max_score = raw_scores[c];
        }
        
        // Hitung softmax
        float exp_sum = 0.0f;
        for (size_t c = 0; c < classes.size(); c++) {
            raw_scores[c] = expf(raw_scores[c] - max_score);
            exp_sum += raw_scores[c];
        }
        
        if (exp_sum > 1e-10f) {
            for (size_t c = 0; c < classes.size(); c++) {
                raw_scores[c] /= exp_sum;
            }
        }
    }

    // Step 5: Temukan kelas dengan skor tertinggi
    int predicted_class = 0;
    float max_score = raw_scores[0];
    for (size_t c = 1; c < classes.size(); c++) {
        if (raw_scores[c] > max_score) {
            max_score = raw_scores[c];
            predicted_class = c;
        }
    }

    // Copy skor ke output buffer jika disediakan
    if (scores_out) {
        for (size_t c = 0; c < classes.size(); c++) {
            scores_out[c] = raw_scores[c];
        }
    }

    return predicted_class;
}

String ANFIS::className(size_t idx) const {
    if (idx >= classes.size()) return String("Unknown");
    return classes[idx];
}
