"""
Data Preprocessing Module
Modul untuk preprocessing data sebelum training
"""

import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler, MinMaxScaler
from imblearn.over_sampling import SMOTE
from imblearn.under_sampling import RandomUnderSampler
from imblearn.combine import SMOTETomek


class DataPreprocessor:
    """Class untuk preprocessing data"""
    
    def __init__(self):
        self.scaler = None
        self.feature_names = None
        
    def split_features_target(self, df, target_col='status', drop_cols=None):
        """
        Pisahkan features dan target
        
        Args:
            df: DataFrame
            target_col: Nama kolom target
            drop_cols: List kolom yang akan di-drop (misal: 'name', 'id')
            
        Returns:
            X, y: Features dan target
        """
        if drop_cols is None:
            drop_cols = []
        
        # Drop kolom yang tidak diperlukan
        cols_to_drop = drop_cols + [target_col]
        X = df.drop(columns=cols_to_drop, errors='ignore')
        y = df[target_col]
        
        self.feature_names = X.columns.tolist()
        
        print(f"✓ Features: {X.shape[1]} kolom")
        print(f"✓ Target: {target_col}")
        print(f"✓ Distribusi target: {y.value_counts().to_dict()}")
        
        return X, y
    
    def scale_data(self, X_train, X_test, method='standard'):
        """
        Normalisasi/Standardisasi data
        
        Args:
            X_train: Training features
            X_test: Testing features
            method: 'standard' atau 'minmax'
            
        Returns:
            X_train_scaled, X_test_scaled
        """
        if method == 'standard':
            self.scaler = StandardScaler()
        elif method == 'minmax':
            self.scaler = MinMaxScaler()
        else:
            raise ValueError("Method harus 'standard' atau 'minmax'")
        
        X_train_scaled = self.scaler.fit_transform(X_train)
        X_test_scaled = self.scaler.transform(X_test)
        
        print(f"✓ Data di-scale menggunakan {method} scaler")
        
        return X_train_scaled, X_test_scaled
    
    def handle_imbalanced_data(self, X_train, y_train, method='smote'):
        """
        Handle imbalanced dataset
        
        Args:
            X_train: Training features
            y_train: Training target
            method: 'smote', 'undersample', atau 'smotetomek'
            
        Returns:
            X_resampled, y_resampled
        """
        print(f"\nDistribusi sebelum balancing: {pd.Series(y_train).value_counts().to_dict()}")
        
        if method == 'smote':
            sampler = SMOTE(random_state=42)
        elif method == 'undersample':
            sampler = RandomUnderSampler(random_state=42)
        elif method == 'smotetomek':
            sampler = SMOTETomek(random_state=42)
        else:
            raise ValueError("Method harus 'smote', 'undersample', atau 'smotetomek'")
        
        X_resampled, y_resampled = sampler.fit_resample(X_train, y_train)
        
        print(f"Distribusi setelah balancing: {pd.Series(y_resampled).value_counts().to_dict()}")
        print(f"✓ Data di-balance menggunakan {method}")
        
        return X_resampled, y_resampled
    
    def prepare_data(self, df, target_col='status', drop_cols=None, 
                    test_size=0.2, random_state=42, scale=True, 
                    balance=False, balance_method='smote'):
        """
        Pipeline lengkap preprocessing
        
        Args:
            df: DataFrame
            target_col: Nama kolom target
            drop_cols: List kolom yang akan di-drop
            test_size: Ukuran test set (default: 0.2)
            random_state: Random state untuk reproducibility
            scale: Apakah data perlu di-scale
            balance: Apakah perlu handle imbalanced data
            balance_method: Method untuk balancing
            
        Returns:
            X_train, X_test, y_train, y_test
        """
        print("\n" + "="*60)
        print("DATA PREPROCESSING")
        print("="*60 + "\n")
        
        # 1. Split features dan target
        X, y = self.split_features_target(df, target_col, drop_cols)
        
        # 2. Train-test split
        X_train, X_test, y_train, y_test = train_test_split(
            X, y, test_size=test_size, random_state=random_state, stratify=y
        )
        print(f"\n✓ Data split: {len(X_train)} training, {len(X_test)} testing")
        
        # 3. Scaling
        if scale:
            X_train, X_test = self.scale_data(X_train, X_test)
        
        # 4. Handle imbalanced data
        if balance:
            X_train, y_train = self.handle_imbalanced_data(
                X_train, y_train, method=balance_method
            )
        
        print("\n" + "="*60)
        return X_train, X_test, y_train, y_test
