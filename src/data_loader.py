"""
Data Loader Module
Modul untuk load dan validasi dataset Parkinson
"""

import pandas as pd
import numpy as np
from pathlib import Path
import os


class DataLoader:
    """Class untuk loading dan validasi dataset"""
    
    def __init__(self, data_path: str):
        """
        Initialize DataLoader
        
        Args:
            data_path: Path ke file dataset
        """
        self.data_path = data_path
        self.df = None
        
    def load_data(self, encoding='utf-8'):
        """
        Load dataset dari CSV
        
        Args:
            encoding: Encoding file (default: utf-8)
            
        Returns:
            pandas.DataFrame: Dataset yang sudah di-load
        """
        try:
            self.df = pd.read_csv(self.data_path, encoding=encoding)
            print(f"✓ Dataset berhasil di-load dari {self.data_path}")
            print(f"  Shape: {self.df.shape}")
            return self.df
        except FileNotFoundError:
            print(f"✗ File tidak ditemukan: {self.data_path}")
            print(f"  Silakan letakkan dataset Anda di folder 'data/'")
            return None
        except Exception as e:
            print(f"✗ Error saat loading data: {str(e)}")
            return None
    
    def get_info(self):
        """Tampilkan informasi dataset"""
        if self.df is None:
            print("Dataset belum di-load. Gunakan load_data() terlebih dahulu.")
            return
        
        print("\n" + "="*60)
        print("INFORMASI DATASET")
        print("="*60)
        print(f"\nJumlah baris: {self.df.shape[0]}")
        print(f"Jumlah kolom: {self.df.shape[1]}")
        print(f"\nKolom-kolom:")
        print(self.df.columns.tolist())
        print(f"\nTipe data:")
        print(self.df.dtypes)
        print(f"\nMissing values:")
        print(self.df.isnull().sum())
        print(f"\nStatistik deskriptif:")
        print(self.df.describe())
        
        # Cek kolom target
        possible_targets = ['status', 'target', 'label', 'class', 'diagnosis']
        target_col = None
        for col in possible_targets:
            if col in self.df.columns:
                target_col = col
                break
        
        if target_col:
            print(f"\n\nDistribusi Target ({target_col}):")
            print(self.df[target_col].value_counts())
            print(f"\nPersentase:")
            print(self.df[target_col].value_counts(normalize=True) * 100)
        
        return self.df.info()
