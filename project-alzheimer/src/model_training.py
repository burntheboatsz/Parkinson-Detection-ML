"""
Model Training Module
Modul untuk training berbagai model ML
"""

import pandas as pd
import numpy as np
from sklearn.ensemble import RandomForestClassifier, GradientBoostingClassifier
from sklearn.svm import SVC
from sklearn.linear_model import LogisticRegression
from sklearn.neighbors import KNeighborsClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.naive_bayes import GaussianNB
import xgboost as xgb
import lightgbm as lgb
from catboost import CatBoostClassifier
import warnings
warnings.filterwarnings('ignore')


class ModelTrainer:
    """Class untuk training multiple models"""
    
    def __init__(self):
        self.models = {}
        self.trained_models = {}
        
    def initialize_models(self):
        """Initialize berbagai model untuk dicoba"""
        self.models = {
            'Logistic Regression': LogisticRegression(random_state=42, max_iter=1000),
            'Decision Tree': DecisionTreeClassifier(random_state=42),
            'Random Forest': RandomForestClassifier(n_estimators=100, random_state=42),
            'SVM': SVC(kernel='rbf', random_state=42, probability=True),
            'KNN': KNeighborsClassifier(n_neighbors=5),
            'Naive Bayes': GaussianNB(),
            'Gradient Boosting': GradientBoostingClassifier(random_state=42),
            'XGBoost': xgb.XGBClassifier(random_state=42, eval_metric='logloss'),
            'LightGBM': lgb.LGBMClassifier(random_state=42, verbose=-1),
            'CatBoost': CatBoostClassifier(random_state=42, verbose=0)
        }
        
        print(f"✓ {len(self.models)} model telah di-initialize")
        return self.models
    
    def train_single_model(self, model_name, X_train, y_train):
        """
        Train satu model
        
        Args:
            model_name: Nama model
            X_train: Training features
            y_train: Training target
            
        Returns:
            Trained model
        """
        if model_name not in self.models:
            raise ValueError(f"Model {model_name} tidak ditemukan")
        
        print(f"Training {model_name}...", end=" ")
        model = self.models[model_name]
        model.fit(X_train, y_train)
        self.trained_models[model_name] = model
        print("✓")
        
        return model
    
    def train_all_models(self, X_train, y_train):
        """
        Train semua model
        
        Args:
            X_train: Training features
            y_train: Training target
            
        Returns:
            Dictionary berisi semua trained models
        """
        print("\n" + "="*60)
        print("TRAINING MODELS")
        print("="*60 + "\n")
        
        if not self.models:
            self.initialize_models()
        
        for model_name in self.models.keys():
            try:
                self.train_single_model(model_name, X_train, y_train)
            except Exception as e:
                print(f"✗ Error training {model_name}: {str(e)}")
        
        print(f"\n✓ {len(self.trained_models)} model berhasil di-train")
        print("="*60)
        
        return self.trained_models
    
    def get_model(self, model_name):
        """Ambil model yang sudah di-train"""
        return self.trained_models.get(model_name)
