"""
Prediction Script
Script untuk melakukan prediksi menggunakan model yang sudah di-train
"""

import sys
import os
sys.path.append(os.path.join(os.path.dirname(__file__), '..', 'src'))

import pandas as pd
import numpy as np
from model_utils import ModelUtils


def load_model_and_params(model_dir='models'):
    """
    Load model dan preprocessing parameters
    
    Returns:
        model, scaler, feature_names
    """
    # Get absolute path
    if not os.path.isabs(model_dir):
        script_dir = os.path.dirname(os.path.abspath(__file__))
        model_dir = os.path.join(os.path.dirname(script_dir), model_dir)
    
    # Load model (sesuaikan dengan nama model terbaik Anda)
    # Contoh: random_forest.pkl, xgboost.pkl, dll
    model_files = [f for f in os.listdir(model_dir) if f.endswith('.pkl') and f != 'scaler.pkl']
    
    if not model_files:
        print("Error: No model file found!")
        return None, None, None
    
    # Ambil model pertama (atau bisa disesuaikan)
    model_path = os.path.join(model_dir, model_files[0])
    model = ModelUtils.load_model(model_path)
    
    # Load preprocessing params
    scaler, feature_names = ModelUtils.load_preprocessing_params(model_dir)
    
    return model, scaler, feature_names


def predict_from_csv(csv_path, model, scaler, feature_names):
    """
    Predict dari CSV file
    
    Args:
        csv_path: Path ke CSV file
        model: Trained model
        scaler: Scaler object
        feature_names: List of feature names
        
    Returns:
        DataFrame dengan predictions
    """
    # Load data
    df = pd.read_csv(csv_path)
    
    # Prepare features
    X = df[feature_names]
    
    # Predict
    predictions, probabilities = ModelUtils.predict(model, X, scaler, feature_names)
    
    # Add predictions to dataframe
    df['prediction'] = predictions
    df['prediction_label'] = ['Parkinson' if p == 1 else 'Healthy' for p in predictions]
    
    if probabilities is not None:
        df['probability_healthy'] = probabilities[:, 0]
        df['probability_parkinson'] = probabilities[:, 1]
    
    return df


def predict_single_patient(patient_data, model, scaler, feature_names):
    """
    Predict untuk satu pasien
    
    Args:
        patient_data: Dictionary berisi data pasien
        model: Trained model
        scaler: Scaler object
        feature_names: List of feature names
        
    Returns:
        Prediction result
    """
    result = ModelUtils.predict_single_patient(model, patient_data, scaler, feature_names)
    return result


def main():
    """Main function"""
    print("\n" + "="*60)
    print("PARKINSON DISEASE PREDICTION")
    print("="*60 + "\n")
    
    # Load model and parameters
    print("Loading model...")
    model, scaler, feature_names = load_model_and_params('models')
    
    if model is None:
        print("Failed to load model. Please train a model first.")
        return
    
    print(f"✓ Model loaded successfully")
    print(f"  Features: {len(feature_names)}")
    
    # Menu
    print("\nOptions:")
    print("1. Predict from CSV file")
    print("2. Predict single patient (manual input)")
    print("3. Exit")
    
    choice = input("\nEnter your choice (1-3): ")
    
    if choice == '1':
        csv_path = input("Enter CSV file path: ")
        if os.path.exists(csv_path):
            results = predict_from_csv(csv_path, model, scaler, feature_names)
            
            print("\n" + "="*60)
            print("PREDICTION RESULTS")
            print("="*60)
            print(results[['prediction_label', 'probability_healthy', 'probability_parkinson']])
            
            # Save results
            output_path = csv_path.replace('.csv', '_predictions.csv')
            results.to_csv(output_path, index=False)
            print(f"\n✓ Results saved to {output_path}")
        else:
            print("File not found!")
    
    elif choice == '2':
        print("\nEnter patient data:")
        print("(Note: You need to provide all features)")
        print(f"Required features: {feature_names}")
        
        patient_data = {}
        for feature in feature_names:
            while True:
                try:
                    value = float(input(f"{feature}: "))
                    patient_data[feature] = value
                    break
                except ValueError:
                    print("Invalid input. Please enter a number.")
        
        result = predict_single_patient(patient_data, model, scaler, feature_names)
        
        print("\n" + "="*60)
        print("PREDICTION RESULT")
        print("="*60)
        print(f"Prediction: {result['prediction_label']}")
        if 'probability_parkinson' in result:
            print(f"Probability (Healthy): {result['probability_healthy']:.2%}")
            print(f"Probability (Parkinson): {result['probability_parkinson']:.2%}")
        print("="*60)
    
    elif choice == '3':
        print("Goodbye!")
    
    else:
        print("Invalid choice!")


if __name__ == "__main__":
    main()
