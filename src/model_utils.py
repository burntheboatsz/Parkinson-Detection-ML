"""
Model Utilities Module
Modul untuk save, load, dan predict dengan model
"""

import joblib
import pandas as pd
import numpy as np
from pathlib import Path
import json


class ModelUtils:
    """Class untuk utilities model"""
    
    @staticmethod
    def save_model(model, model_name, save_dir='../models'):
        """
        Save trained model ke disk
        
        Args:
            model: Trained model
            model_name: Nama model
            save_dir: Directory untuk save model
            
        Returns:
            Path ke saved model
        """
        save_dir = Path(save_dir)
        save_dir.mkdir(parents=True, exist_ok=True)
        
        # Clean model name untuk filename
        filename = model_name.replace(' ', '_').lower() + '.pkl'
        filepath = save_dir / filename
        
        joblib.dump(model, filepath)
        print(f"✓ Model saved to {filepath}")
        
        return str(filepath)
    
    @staticmethod
    def load_model(model_path):
        """
        Load trained model dari disk
        
        Args:
            model_path: Path ke model file
            
        Returns:
            Loaded model
        """
        try:
            model = joblib.load(model_path)
            print(f"✓ Model loaded from {model_path}")
            return model
        except Exception as e:
            print(f"✗ Error loading model: {str(e)}")
            return None
    
    @staticmethod
    def save_preprocessing_params(scaler, feature_names, save_dir='../models'):
        """
        Save preprocessing parameters
        
        Args:
            scaler: Fitted scaler object
            feature_names: List of feature names
            save_dir: Directory untuk save
        """
        save_dir = Path(save_dir)
        save_dir.mkdir(parents=True, exist_ok=True)
        
        # Save scaler
        scaler_path = save_dir / 'scaler.pkl'
        joblib.dump(scaler, scaler_path)
        
        # Save feature names
        features_path = save_dir / 'feature_names.json'
        with open(features_path, 'w') as f:
            json.dump(feature_names, f)
        
        print(f"✓ Scaler saved to {scaler_path}")
        print(f"✓ Feature names saved to {features_path}")
    
    @staticmethod
    def load_preprocessing_params(model_dir='../models'):
        """
        Load preprocessing parameters
        
        Args:
            model_dir: Directory tempat model disimpan
            
        Returns:
            scaler, feature_names
        """
        model_dir = Path(model_dir)
        
        # Load scaler
        scaler_path = model_dir / 'scaler.pkl'
        scaler = joblib.load(scaler_path)
        
        # Load feature names
        features_path = model_dir / 'feature_names.json'
        with open(features_path, 'r') as f:
            feature_names = json.load(f)
        
        print(f"✓ Preprocessing parameters loaded")
        
        return scaler, feature_names
    
    @staticmethod
    def predict(model, data, scaler=None, feature_names=None):
        """
        Predict dengan model yang sudah di-train
        
        Args:
            model: Trained model
            data: Data untuk prediction (DataFrame, dict, atau array)
            scaler: Scaler object (jika data perlu di-scale)
            feature_names: Nama-nama features
            
        Returns:
            Predictions
        """
        # Convert data ke DataFrame jika perlu
        if isinstance(data, dict):
            data = pd.DataFrame([data])
        elif isinstance(data, np.ndarray):
            if feature_names:
                data = pd.DataFrame(data, columns=feature_names)
            else:
                data = pd.DataFrame(data)
        
        # Ensure feature order
        if feature_names:
            data = data[feature_names]
        
        # Scale data jika ada scaler
        if scaler:
            data = scaler.transform(data)
        
        # Predict
        predictions = model.predict(data)
        
        # Get probabilities jika tersedia
        try:
            probabilities = model.predict_proba(data)
            return predictions, probabilities
        except:
            return predictions, None
    
    @staticmethod
    def predict_single_patient(model, patient_data, scaler=None, feature_names=None):
        """
        Predict untuk satu pasien
        
        Args:
            model: Trained model
            patient_data: Dictionary berisi data pasien
            scaler: Scaler object
            feature_names: Nama-nama features
            
        Returns:
            Prediction result dengan probability
        """
        predictions, probabilities = ModelUtils.predict(
            model, patient_data, scaler, feature_names
        )
        
        result = {
            'prediction': int(predictions[0]),
            'prediction_label': 'Parkinson' if predictions[0] == 1 else 'Healthy'
        }
        
        if probabilities is not None:
            result['probability_healthy'] = float(probabilities[0][0])
            result['probability_parkinson'] = float(probabilities[0][1])
        
        return result
