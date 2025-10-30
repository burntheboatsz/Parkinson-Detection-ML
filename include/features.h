#pragma once
#include <Arduino.h>

// Fitur yang digunakan oleh model ANFIS: ['ccx','tcx','tcz','lcx','lcz','scx']
// Mapping dari data sensor:
// - ccx: Cervical Curve X (pitch sensor cervical)
// - tcx: Thoracal Curve X (pitch sensor thoracal)
// - tcz: Thoracal Curve Z (roll sensor thoracal)
// - lcx: Lumbal Curve X (pitch sensor lumbal)
// - lcz: Lumbal Curve Z (roll sensor lumbal)
// - scx: Sacrum Curve X (pitch sensor sacrum)

struct SpineFeatures {
    float ccx; // Cervical Curve X
    float tcx; // Thoracal Curve X
    float tcz; // Thoracal Curve Z
    float lcx; // Lumbal Curve X
    float lcz; // Lumbal Curve Z
    float scx; // Sacrum Curve X
    
    // Konversi ke array untuk input ke ANFIS
    void toArray(float arr[6]) const {
        arr[0] = ccx;
        arr[1] = tcx;
        arr[2] = tcz;
        arr[3] = lcx;
        arr[4] = lcz;
        arr[5] = scx;
    }
};

class FeatureExtractor {
public:
    // Ekstrak fitur dari data sensor (4 sensor MPU6050)
    // angleX = pitch, angleY = roll, angleZ = yaw
    // idx: 0=Cervical, 1=Thoracal, 2=Lumbal, 3=Sacrum
    static SpineFeatures fromSensorData(
        const float angleX[4],  // pitch
        const float angleY[4],  // roll
        const float angleZ[4]   // yaw (tidak digunakan dalam fitur)
    ) {
        SpineFeatures f{};
        
        // Mapping langsung dari sensor ke fitur
        f.ccx = angleX[0];  // Cervical pitch
        f.tcx = angleX[1];  // Thoracal pitch
        f.tcz = angleY[1];  // Thoracal roll
        f.lcx = angleX[2];  // Lumbal pitch
        f.lcz = angleY[2];  // Lumbal roll
        f.scx = angleX[3];  // Sacrum pitch
        
        return f;
    }
};
