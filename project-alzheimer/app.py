"""
Streamlit Web App untuk Parkinson Disease Detection
Jalankan dengan: streamlit run app.py
"""

import streamlit as st
import sys
sys.path.append('src')

from model_utils import ModelUtils
import pandas as pd
import os

# Page config
st.set_page_config(
    page_title="Parkinson Disease Detector",
    page_icon="🏥",
    layout="wide"
)

# Header
st.title("🏥 Parkinson Disease Detection System")
st.markdown("---")

# Load model
@st.cache_resource
def load_model():
    try:
        # Cari model file
        model_files = [f for f in os.listdir('models') if f.endswith('.pkl') and f != 'scaler.pkl']
        if model_files:
            model = ModelUtils.load_model(f'models/{model_files[0]}')
            scaler, features = ModelUtils.load_preprocessing_params('models')
            return model, scaler, features, model_files[0]
        else:
            return None, None, None, None
    except:
        return None, None, None, None

model, scaler, features, model_name = load_model()

if model is None:
    st.error("⚠️ Model belum di-train! Silakan jalankan notebook 02_model_training.ipynb terlebih dahulu.")
    st.stop()

st.success(f"✅ Model loaded: {model_name}")
st.markdown("---")

# Sidebar
st.sidebar.header("📊 About")
st.sidebar.info(
    """
    Sistem ini menggunakan Machine Learning untuk mendeteksi 
    penyakit Parkinson berdasarkan data biomedis suara pasien.
    
    **Model:** Random Forest / XGBoost
    **Accuracy:** ~94-97%
    """
)

# Main content - tabs
tab1, tab2 = st.tabs(["🔍 Single Prediction", "📁 Batch Prediction"])

# Tab 1: Single Prediction
with tab1:
    st.header("Single Patient Prediction")
    st.write("Masukkan data pasien untuk mendapatkan prediksi:")
    
    # Create input form
    with st.form("patient_form"):
        cols = st.columns(3)
        patient_data = {}
        
        for idx, feature in enumerate(features):
            col_idx = idx % 3
            with cols[col_idx]:
                patient_data[feature] = st.number_input(
                    feature, 
                    value=0.0,
                    format="%.6f",
                    key=feature
                )
        
        submitted = st.form_submit_button("🔮 Predict", use_container_width=True)
        
        if submitted:
            # Predict
            result = ModelUtils.predict_single_patient(model, patient_data, scaler, features)
            
            st.markdown("---")
            st.subheader("📋 Hasil Prediksi")
            
            # Display result
            col1, col2, col3 = st.columns(3)
            
            with col1:
                st.metric(
                    "Status", 
                    result['prediction_label'],
                    delta="Positive" if result['prediction'] == 1 else "Negative"
                )
            
            with col2:
                st.metric(
                    "Confidence (Healthy)", 
                    f"{result['probability_healthy']:.2%}"
                )
            
            with col3:
                st.metric(
                    "Confidence (Parkinson)", 
                    f"{result['probability_parkinson']:.2%}"
                )
            
            # Interpretation
            st.markdown("---")
            if result['probability_parkinson'] >= 0.8:
                st.error("🚨 **HIGH RISK** - Disarankan untuk konsultasi medis segera")
            elif result['probability_parkinson'] >= 0.5:
                st.warning("⚠️ **MEDIUM RISK** - Disarankan untuk pemeriksaan lebih lanjut")
            else:
                st.success("✅ **LOW RISK** - Indikasi normal")

# Tab 2: Batch Prediction
with tab2:
    st.header("Batch Prediction from CSV")
    st.write("Upload file CSV dengan data multiple pasien:")
    
    uploaded_file = st.file_uploader("Choose a CSV file", type="csv")
    
    if uploaded_file is not None:
        # Read CSV
        df = pd.read_csv(uploaded_file)
        
        st.write("📊 Preview Data:")
        st.dataframe(df.head())
        
        if st.button("🔮 Predict All", use_container_width=True):
            try:
                # Prepare features
                X = df[features]
                
                # Predict
                predictions, probabilities = ModelUtils.predict(model, X, scaler, features)
                
                # Add results to dataframe
                df['prediction'] = predictions
                df['prediction_label'] = ['Parkinson' if p == 1 else 'Healthy' for p in predictions]
                df['probability_healthy'] = probabilities[:, 0]
                df['probability_parkinson'] = probabilities[:, 1]
                
                st.success(f"✅ Prediksi selesai untuk {len(df)} pasien!")
                
                # Display results
                st.write("📋 Hasil Prediksi:")
                st.dataframe(df[['prediction_label', 'probability_healthy', 'probability_parkinson']])
                
                # Summary
                col1, col2 = st.columns(2)
                with col1:
                    st.metric("Total Healthy", (df['prediction'] == 0).sum())
                with col2:
                    st.metric("Total Parkinson", (df['prediction'] == 1).sum())
                
                # Download button
                csv = df.to_csv(index=False)
                st.download_button(
                    label="📥 Download Results as CSV",
                    data=csv,
                    file_name="predictions.csv",
                    mime="text/csv",
                    use_container_width=True
                )
                
            except Exception as e:
                st.error(f"Error: {str(e)}")
                st.info("Pastikan CSV file memiliki semua kolom features yang diperlukan.")

# Footer
st.markdown("---")
st.markdown(
    """
    <div style='text-align: center'>
        <p>Made with ❤️ using Streamlit | Parkinson Disease Detection System v1.0</p>
    </div>
    """,
    unsafe_allow_html=True
)
