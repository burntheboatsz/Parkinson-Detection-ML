"""
Model Evaluation Module
Modul untuk evaluasi performa model
"""

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from sklearn.metrics import (
    accuracy_score, precision_score, recall_score, f1_score,
    confusion_matrix, classification_report, roc_auc_score, roc_curve
)
import warnings
warnings.filterwarnings('ignore')


class ModelEvaluator:
    """Class untuk evaluasi model"""
    
    def __init__(self):
        self.results = {}
        
    def evaluate_model(self, model, model_name, X_test, y_test):
        """
        Evaluasi satu model
        
        Args:
            model: Trained model
            model_name: Nama model
            X_test: Testing features
            y_test: Testing target
            
        Returns:
            Dictionary berisi metrics
        """
        # Predictions
        y_pred = model.predict(X_test)
        
        # Probabilities (jika ada)
        try:
            y_pred_proba = model.predict_proba(X_test)[:, 1]
        except:
            y_pred_proba = y_pred
        
        # Calculate metrics
        metrics = {
            'Model': model_name,
            'Accuracy': accuracy_score(y_test, y_pred),
            'Precision': precision_score(y_test, y_pred, average='weighted', zero_division=0),
            'Recall': recall_score(y_test, y_pred, average='weighted', zero_division=0),
            'F1-Score': f1_score(y_test, y_pred, average='weighted', zero_division=0)
        }
        
        # ROC-AUC (untuk binary classification)
        try:
            if len(np.unique(y_test)) == 2:
                metrics['ROC-AUC'] = roc_auc_score(y_test, y_pred_proba)
        except:
            metrics['ROC-AUC'] = None
        
        self.results[model_name] = {
            'metrics': metrics,
            'y_pred': y_pred,
            'y_pred_proba': y_pred_proba,
            'confusion_matrix': confusion_matrix(y_test, y_pred)
        }
        
        return metrics
    
    def evaluate_all_models(self, trained_models, X_test, y_test):
        """
        Evaluasi semua model
        
        Args:
            trained_models: Dictionary berisi trained models
            X_test: Testing features
            y_test: Testing target
            
        Returns:
            DataFrame berisi hasil evaluasi semua model
        """
        print("\n" + "="*60)
        print("EVALUASI MODELS")
        print("="*60 + "\n")
        
        all_metrics = []
        
        for model_name, model in trained_models.items():
            try:
                metrics = self.evaluate_model(model, model_name, X_test, y_test)
                all_metrics.append(metrics)
                print(f"✓ {model_name} evaluated")
            except Exception as e:
                print(f"✗ Error evaluating {model_name}: {str(e)}")
        
        results_df = pd.DataFrame(all_metrics)
        results_df = results_df.sort_values('Accuracy', ascending=False).reset_index(drop=True)
        
        print("\n" + "="*60)
        print("HASIL EVALUASI")
        print("="*60)
        print(results_df.to_string(index=False))
        print("="*60 + "\n")
        
        return results_df
    
    def plot_confusion_matrix(self, model_name, figsize=(8, 6), save_path=None):
        """
        Plot confusion matrix untuk satu model
        
        Args:
            model_name: Nama model
            figsize: Ukuran figure
            save_path: Path untuk save plot
        """
        if model_name not in self.results:
            print(f"Model {model_name} belum di-evaluasi")
            return
        
        cm = self.results[model_name]['confusion_matrix']
        
        plt.figure(figsize=figsize)
        sns.heatmap(cm, annot=True, fmt='d', cmap='Blues', cbar=False)
        plt.title(f'Confusion Matrix - {model_name}')
        plt.ylabel('Actual')
        plt.xlabel('Predicted')
        
        if save_path:
            plt.savefig(save_path, dpi=300, bbox_inches='tight')
            print(f"✓ Confusion matrix saved to {save_path}")
        
        plt.tight_layout()
        plt.show()
    
    def plot_model_comparison(self, results_df, figsize=(12, 6), save_path=None):
        """
        Plot perbandingan performa semua model
        
        Args:
            results_df: DataFrame hasil evaluasi
            figsize: Ukuran figure
            save_path: Path untuk save plot
        """
        metrics = ['Accuracy', 'Precision', 'Recall', 'F1-Score']
        
        fig, axes = plt.subplots(2, 2, figsize=figsize)
        axes = axes.ravel()
        
        for idx, metric in enumerate(metrics):
            if metric in results_df.columns:
                ax = axes[idx]
                results_df.plot(x='Model', y=metric, kind='bar', ax=ax, legend=False)
                ax.set_title(f'{metric} Comparison')
                ax.set_xlabel('')
                ax.set_ylabel(metric)
                ax.tick_params(axis='x', rotation=45)
                ax.grid(axis='y', alpha=0.3)
        
        plt.tight_layout()
        
        if save_path:
            plt.savefig(save_path, dpi=300, bbox_inches='tight')
            print(f"✓ Comparison plot saved to {save_path}")
        
        plt.show()
    
    def get_classification_report(self, model_name):
        """
        Generate classification report untuk satu model
        
        Args:
            model_name: Nama model
            
        Returns:
            Classification report string
        """
        if model_name not in self.results:
            print(f"Model {model_name} belum di-evaluasi")
            return None
        
        # Untuk generate report, kita perlu y_test
        # Ini akan di-handle di notebook
        print(f"Classification report untuk {model_name} tersedia di notebook")
        return self.results[model_name]
