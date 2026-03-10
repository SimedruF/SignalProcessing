#!/usr/bin/env python3
"""
Read and visualize signal data from HDF5 files created by SignalRecorder
"""

import h5py
import numpy as np
import matplotlib.pyplot as plt

def read_and_plot_signal(filename):
    """Read HDF5 file and plot signals"""
    
    with h5py.File(filename, 'r') as f:
        print("=" * 60)
        print(f"Reading: {filename}")
        print("=" * 60)
        
        # Print file structure
        print("\n📁 File Structure:")
        def print_structure(name, obj):
            if isinstance(obj, h5py.Dataset):
                print(f"  Dataset: {name} - Shape: {obj.shape}, Type: {obj.dtype}")
            elif isinstance(obj, h5py.Group):
                print(f"  Group: {name}/")
        f.visititems(print_structure)
        
        # Read metadata
        print("\n📋 Session Metadata:")
        if 'Session' in f:
            for key in f['Session'].keys():
                value = f['Session'][key][()].decode('utf-8')
                print(f"  {key}: {value}")
        
        print("\n⚙️ Parameters:")
        if 'Parameters' in f:
            for key in f['Parameters'].keys():
                value = f['Parameters'][key][()].decode('utf-8')
                print(f"  {key}: {value}")
        
        # Read signals
        original_signal = None
        filtered_signal = None
        
        if 'Signals/Raw/original_signal' in f:
            original_signal = f['Signals/Raw/original_signal'][:]
            print(f"\n📊 Original signal: {len(original_signal)} samples")
            print(f"   Mean: {np.mean(original_signal):.4f}")
            print(f"   Std: {np.std(original_signal):.4f}")
            print(f"   Min: {np.min(original_signal):.4f}")
            print(f"   Max: {np.max(original_signal):.4f}")
        
        if 'Signals/Processed/filtered_signal' in f:
            filtered_signal = f['Signals/Processed/filtered_signal'][:]
            print(f"\n🔧 Filtered signal: {len(filtered_signal)} samples")
        
        # Read anomalies if present
        anomalies = []
        if 'Anomalies/indices' in f:
            anomalies = f['Anomalies/indices'][:].astype(int)
            print(f"\n⚠️  Anomalies detected: {len(anomalies)} points")
            if 'Anomalies' in f:
                for key in f['Anomalies'].attrs.keys():
                    print(f"   {key}: {f['Anomalies'].attrs[key]}")
        
        # Read FFT data if present
        fft_frequencies = None
        fft_magnitudes = None
        if 'Analysis/FFT/frequencies' in f:
            fft_frequencies = f['Analysis/FFT/frequencies'][:]
            fft_magnitudes = f['Analysis/FFT/magnitudes'][:]
            print(f"\n📈 FFT data: {len(fft_frequencies)} frequency bins")
        
        # Read ML features if present
        if 'Analysis/ML_Features/feature_vector' in f:
            features = f['Analysis/ML_Features/feature_vector'][:]
            feature_names = f['Analysis/ML_Features'].attrs['feature_names'].decode('utf-8').split(',')
            print(f"\n🤖 ML Features ({len(features)} features):")
            for name, value in zip(feature_names, features):
                print(f"   {name}: {value:.4f}")
        
        # Plotting
        fig, axes = plt.subplots(2, 1, figsize=(12, 8))
        fig.suptitle(f'Signal Analysis: {filename}', fontsize=14, fontweight='bold')
        
        # Plot time domain
        ax1 = axes[0]
        if original_signal is not None:
            ax1.plot(original_signal, label='Original Signal', alpha=0.7, linewidth=1)
        if filtered_signal is not None:
            ax1.plot(filtered_signal, label='Filtered Signal', linewidth=1.5)
        if len(anomalies) > 0 and original_signal is not None:
            ax1.scatter(anomalies, original_signal[anomalies], 
                       color='red', s=50, marker='o', label='Anomalies', zorder=5)
        
        ax1.set_xlabel('Sample')
        ax1.set_ylabel('Amplitude')
        ax1.set_title('Time Domain Signal')
        ax1.legend()
        ax1.grid(True, alpha=0.3)
        
        # Plot FFT
        ax2 = axes[1]
        if fft_frequencies is not None and fft_magnitudes is not None:
            ax2.plot(fft_frequencies, fft_magnitudes, linewidth=1.5)
            ax2.set_xlabel('Frequency (Hz)')
            ax2.set_ylabel('Magnitude')
            ax2.set_title('Frequency Spectrum')
            ax2.grid(True, alpha=0.3)
            
            # Mark dominant frequency
            max_idx = np.argmax(fft_magnitudes)
            ax2.axvline(fft_frequencies[max_idx], color='red', 
                       linestyle='--', alpha=0.5, label=f'Dominant: {fft_frequencies[max_idx]:.1f} Hz')
            ax2.legend()
        else:
            ax2.text(0.5, 0.5, 'No FFT data available', 
                    ha='center', va='center', transform=ax2.transAxes, fontsize=12)
            ax2.set_title('Frequency Spectrum (No data)')
        
        plt.tight_layout()
        plt.show()
        
        print("\n" + "=" * 60)
        print("✅ Done!")
        print("=" * 60)

if __name__ == '__main__':
    import sys
    
    if len(sys.argv) > 1:
        filename = sys.argv[1]
    else:
        # Default to most recent file in build directory
        filename = 'signal_recording.h5'
    
    try:
        read_and_plot_signal(filename)
    except FileNotFoundError:
        print(f"❌ Error: File '{filename}' not found!")
        print("\nUsage:")
        print(f"  python3 read_hdf5.py [filename.h5]")
        print(f"\nExample:")
        print(f"  python3 read_hdf5.py build/signal_recording.h5")
    except Exception as e:
        print(f"❌ Error reading file: {e}")
