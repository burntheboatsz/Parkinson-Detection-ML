"""
Example: How to use the trained model for predictions
"""

import sys
sys.path.append('../src')

from model_utils import ModelUtils
import pandas as pd


def example_predict_single():
    """Example: Predict untuk satu pasien"""
    print("\n" + "="*60)
    print("EXAMPLE 1: Single Patient Prediction")
    print("="*60 + "\n")
    
    # Load model dan parameters
    model = ModelUtils.load_model('../models/random_forest.pkl')  # Sesuaikan nama model
    scaler, features = ModelUtils.load_preprocessing_params('../models')
    
    # Contoh data pasien
    # CATATAN: Ini hanya contoh, sesuaikan dengan features dataset Anda
    patient_data = {
        'MDVP:Fo(Hz)': 197.076,
        'MDVP:Fhi(Hz)': 206.896,
        'MDVP:Flo(Hz)': 192.055,
        'MDVP:Jitter(%)': 0.00289,
        'MDVP:Jitter(Abs)': 0.00001,
        # ... tambahkan semua features lainnya
    }
    
    # Predict
    result = ModelUtils.predict_single_patient(model, patient_data, scaler, features)
    
    print("Prediction Result:")
    print(f"  Status: {result['prediction_label']}")
    if 'probability_parkinson' in result:
        print(f"  Probability (Healthy): {result['probability_healthy']:.2%}")
        print(f"  Probability (Parkinson): {result['probability_parkinson']:.2%}")


def example_predict_batch():
    """Example: Predict untuk multiple patients"""
    print("\n" + "="*60)
    print("EXAMPLE 2: Batch Prediction from CSV")
    print("="*60 + "\n")
    
    # Load model dan parameters
    model = ModelUtils.load_model('../models/random_forest.pkl')
    scaler, features = ModelUtils.load_preprocessing_params('../models')
    
    # Load data dari CSV
    df = pd.read_csv('../data/new_patients.csv')  # File dengan data pasien baru
    
    # Prepare features
    X = df[features]
    
    # Predict
    predictions, probabilities = ModelUtils.predict(model, X, scaler, features)
    
    # Add predictions to dataframe
    df['prediction'] = predictions
    df['prediction_label'] = ['Parkinson' if p == 1 else 'Healthy' for p in predictions]
    df['probability_parkinson'] = probabilities[:, 1]
    
    # Save results
    df.to_csv('../results/predictions.csv', index=False)
    print("✓ Predictions saved to results/predictions.csv")
    
    # Display results
    print("\nSample predictions:")
    print(df[['prediction_label', 'probability_parkinson']].head())


def example_model_comparison():
    """Example: Compare predictions dari multiple models"""
    print("\n" + "="*60)
    print("EXAMPLE 3: Model Comparison")
    print("="*60 + "\n")
    
    # Load multiple models
    models = {
        'Random Forest': ModelUtils.load_model('../models/random_forest.pkl'),
        'XGBoost': ModelUtils.load_model('../models/xgboost.pkl'),
        'SVM': ModelUtils.load_model('../models/svm.pkl')
    }
    
    scaler, features = ModelUtils.load_preprocessing_params('../models')
    
    # Sample patient data
    patient_data = {
        # ... isi dengan data pasien
    }
    
    # Compare predictions
    print("Model Comparison:")
    print("-" * 60)
    for model_name, model in models.items():
        if model is not None:
            result = ModelUtils.predict_single_patient(model, patient_data, scaler, features)
            print(f"{model_name:20} | {result['prediction_label']:10} | {result.get('probability_parkinson', 0):.2%}")


if __name__ == "__main__":
    print("\n🏥 PARKINSON DISEASE PREDICTION - EXAMPLES\n")
    
    # Jalankan examples
    # Uncomment untuk menjalankan
    
    # example_predict_single()
    # example_predict_batch()
    # example_model_comparison()
    
    print("\n✓ Examples ready!")
    print("  Uncomment function calls to run examples")
