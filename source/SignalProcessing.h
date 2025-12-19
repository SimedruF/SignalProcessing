#ifndef SIGNALPROCESSING_H
#define SIGNALPROCESSING_H
#define NB_MAX_VALUES 1000 /* number of values in the signal vector  */
#define NS_PER_SECOND 1000000000
#define DEBUG_INFO 1
#define MAX_INDX 12
#include <time.h>
#include <math.h>
// --------------------------------------------------------
// STRUCT prob_dist_item
// --------------------------------------------------------
typedef struct prob_dist_item
{
    double value;
    int frequency;
    double probability;
    double normal_probability;
    double normal_frequency;
} prob_dist_item;

// --------------------------------------------------------
// STRUCT prob_dist
// --------------------------------------------------------
typedef struct prob_dist
{
    prob_dist_item *items;
    int count;

    double total_probability;
    double total_normal_probability;
    double total_frequency;
    double total_normal_frequency;
} prob_dist;

typedef struct index_lookup_table
{
    int received;
    int normal;
} index_lookup_table;

// --------------------------------------------------------
// STRUCT SegmentStats - Statistics for signal segments
// --------------------------------------------------------
typedef struct SegmentStats
{
    int start_index;        // Starting index of segment
    int end_index;          // Ending index of segment
    int segment_id;         // Segment identifier (e.g., blade number)
    double mean;            // Mean value in segment
    double std_dev;         // Standard deviation in segment
    double max_value;       // Maximum value in segment
    double min_value;       // Minimum value in segment
    double rms;             // Root mean square
    double peak_to_peak;    // Peak-to-peak amplitude
    double anomaly_score;   // Computed anomaly score
    int num_points;         // Number of data points in segment
} SegmentStats;

// --------------------------------------------------------
// STRUCT FrequencyBin - Frequency analysis result
// --------------------------------------------------------
typedef struct FrequencyBin
{
    double frequency;       // Frequency in Hz
    double magnitude;       // Magnitude of this frequency component
    double phase;           // Phase in radians
    double power;          // Power (magnitude squared)
} FrequencyBin;

// --------------------------------------------------------
// STRUCT FrequencySpectrum - Complete frequency analysis
// --------------------------------------------------------
typedef struct FrequencySpectrum
{
    FrequencyBin *bins;     // Array of frequency bins
    int num_bins;           // Number of frequency bins
    double sampling_rate;   // Sampling rate in Hz
    double frequency_resolution; // Frequency resolution (bin width)
    double dominant_frequency;   // Frequency with highest magnitude
    double total_power;     // Total signal power
    int window_size;        // Size of analyzed window
} FrequencySpectrum;

// --------------------------------------------------------
// STRUCT MLFeatureVector - Feature vector for ML/AI
// --------------------------------------------------------
typedef struct MLFeatureVector
{
    // Statistical features
    double mean;
    double std_dev;
    double variance;
    double skewness;
    double kurtosis;
    double rms;
    double peak_to_peak;
    double crest_factor;
    
    // Frequency domain features
    double dominant_frequency;
    double spectral_centroid;
    double spectral_spread;
    double spectral_entropy;
    double total_power;
    double power_low_freq;    // 0-10 Hz
    double power_mid_freq;    // 10-100 Hz
    double power_high_freq;   // 100+ Hz
    
    // Time domain features
    double zero_crossing_rate;
    double mean_crossing_rate;  // Rate of crossing the mean value
    double energy;
    double autocorr_peak;     // First autocorrelation peak (periodicity)
    
    // Total number of features
    int num_features;
} MLFeatureVector;

class SignalProcessing{
public:
    /**
     * @brief Applies exponential smoothing to the signal vector
     * @param alpha Smoothing factor (0 < alpha <= 1)
     * @param out_vector Destination vector for smoothed values (size >= GetIndex())
     */
    void ExponentialSmoothing(double alpha, double *out_vector);

    /**
     * @brief Normalizes the signal vector to [0, 1] range
     */
    void NormalizeVector();
    /**
     * @brief Scales the signal vector to a given range [new_min, new_max]
     * @param new_min Minimum value of the new range
     * @param new_max Maximum value of the new range
     */
    void ScaleVector(double new_min, double new_max);
    /**
     * @brief Calculates the mean (average) of the signal vector
     * @return Mean value
     */
    double GetMean();
    /**
     * @brief Calculates the variance of the signal vector
     * @return Variance value
     */
    double GetVariance();
    /**
     * @brief Calculates the standard deviation of the signal vector
     * @return Standard deviation value
     */
    double GetStandardDeviation();

    /**
     * @brief Calculates the moving average of the last window_size values
     * @param window_size Number of values to average
     * @return Moving average value
     */
    double GetMovingAverage(int window_size);
    /**
     * @brief Calculates the moving average for each position and stores it in out_vector
     * @param out_vector Destination vector
     * @param window_size Number of values to average
     */
    void GetMovingAverageVector(double *out_vector, int window_size);

    /**
     * @brief Constructor for SignalProcessing class
     */
    SignalProcessing();
    /**
     * @brief Adds a value to the signal vector
     * @param value Value to add
     * @return Current index
     */
    int AddValue(double value);
    /**
     * @brief Adds a value with timestamp to the signal vector
     * @param value Value to add
     * @param ts Timestamp to associate with the value
     * @return Current index
     */
    int AddValueWithTimestamp(double value, struct timespec ts);
    /**
     * @brief Sets the item identifier
     * @param Item Item value
     */
    void SetItem(int Item);
    /**
     * @brief Gets the timestamp for a value in the vector
     * @param td timespec struct to store the timestamp
     * @param index Value index
     */
    void GetTimespec(struct timespec *td, int index);
    /**
     * @brief Returns the last value added
     * @return Value
     */
    double GetLastValue();
    /**
     * @brief Returns the maximum capacity of the vector
     * @return Maximum capacity
     */
    int GetMaxCapacity();
    /**
     * @brief Returns the current size of the vector
     * @return Size
     */
    int GetSize();
    /**
     * @brief Returns the current index
     * @return Index
     */
    int GetIndex();
    /**
     * @brief Returns the item identifier
     * @return Item
     */
    int GetItem();
    /**
     * @brief Copies the signal vector to another vector
     * @param signalProc_vector Destination vector
     */
    void GetVector(double *signalProc_vector);
    /**
     * @brief Copies the signal vector to an int vector
     * @param signalProc_vector Destination vector
     * @param size Size
     */
    void GetVectorInt(int *signalProc_vector, int size);
    /**
     * @brief Initializes the signal vector with given values
     * @param values Source vector
     * @param size Size
     */
    void InitVector(double *values, int size);
    /**
     * @brief Multiplies the signal vector by a value
     * @param value Value
     * @param size Size
     */
    void MultiplyWithValue(double value, int size);
    /**
     * @brief Divides the signal vector by a value
     * @param value Value
     * @param size Size
     */
    void DivideWithValue(double value, int size);
    /**
     * @brief Subtracts a value from the signal vector
     * @param value Value
     * @param size Size
     */
    void SubstractWithValue(double value, int size);
    /**
     * @brief Adds a value to the signal vector
     * @param value Value
     * @param size Size
     */
    void AddWithValue(double value, int size);
    /**
     * @brief Copies the signal vector with offset
     * @param signalProc_vector Destination vector
     * @param size Size
     * @param offset Offset
     */
    void GetVectorWithOffset(double *signalProc_vector,int size, int offset);
    /**
     * @brief Resets the signal vector
     */
    void ClearVector();
    /**
     * @brief Frees memory for probability distribution
     */
    void NormalDistributionFree();
    /**
     * @brief Prints the normal distribution
     * @param pd Distribution struct
     */
    void NormalDistributionPrint(prob_dist *pd);
    /**
     * @brief Calculates the normal distribution
     * @param data Data vector
     * @param size Size
     * @param pd Distribution struct
     */
    void NormalDistributionCalculate(double *data, int size, prob_dist *pd);
    /**
     * @brief Creates the normal distribution struct
     * @return Pointer to created struct
     */
    prob_dist *NormalDistributionCreate();
    /**
     * @brief Runs the normal distribution calculation
     */
    void NormalDistributionRun();
    /**
     * @brief Prints the signal vector
     */
    void PrintVector();
    /**
     * @brief Builds the index lookup table
     * @param first_received First received index
     */
    void BuildIndexLookupTable(int first_received);
    /**
     * @brief Returns the normalized index from the lookup table
     * @param ReceivedIndex Received index
     * @return Normalized index
     */
    int  GetIndexLookupTable(int ReceivedIndex);
    /**
     * @brief Detects threshold crossing events in the signal vector
     * @param threshold The threshold value to detect
     * @param direction 1 for rising edge (below->above), -1 for falling edge (above->below), 0 for both
     * @param events Output array to store indices where crossings occur (size >= GetIndex())
     * @return Number of threshold crossings detected
     */
    int DetectThresholdCrossing(double threshold, int direction, int *events);
    /**
     * @brief Detects zero-crossing events in the signal vector
     * @param direction 1 for positive crossing (negative->positive), -1 for negative crossing (positive->negative), 0 for both
     * @param events Output array to store indices where crossings occur (size >= GetIndex())
     * @return Number of zero crossings detected
     */
    int DetectZeroCrossing(int direction, int *events);
    /**
     * @brief Gets the threshold crossing flag status
     * @return True if threshold crossing was detected
     */
    bool GetThresholdCrossingFlag();
    /**
     * @brief Gets the zero crossing flag status
     * @return True if zero crossing was detected
     */
    bool GetZeroCrossingFlag();
    /**
     * @brief Clears event detection flags
     */
    void ClearEventFlags();

    /**
     * @brief Detects peaks (local maxima) in the signal vector
     * @param peaks Output array to store peak indices
     * @param max_peaks Maximum number of peaks to detect
     * @return Number of peaks detected
     */
    int DetectPeaks(int *peaks, int max_peaks);
    
    /**
     * @brief Detects peaks above a threshold value
     * @param threshold Minimum value for peak detection
     * @param peaks Output array to store peak indices
     * @param max_peaks Maximum number of peaks to detect
     * @return Number of peaks detected
     */
    int DetectPeaksWithThreshold(double threshold, int *peaks, int max_peaks);
    
    /**
     * @brief Detects peaks with minimum prominence
     * @param min_prominence Minimum prominence (height above surrounding valleys)
     * @param peaks Output array to store peak indices
     * @param max_peaks Maximum number of peaks to detect
     * @return Number of peaks detected
     */
    int DetectPeaksWithProminence(double min_prominence, int *peaks, int max_peaks);
    
    /**
     * @brief Detects peaks with minimum distance between them
     * @param min_distance Minimum distance between consecutive peaks
     * @param peaks Output array to store peak indices
     * @param max_peaks Maximum number of peaks to detect
     * @return Number of peaks detected
     */
    int DetectPeaksWithDistance(int min_distance, int *peaks, int max_peaks);
    
    /**
     * @brief Gets the value at a specific peak index
     * @param peak_index Index of the peak in the signal vector
     * @return Value at the peak
     */
    double GetPeakValue(int peak_index);

    /**
     * @brief Gets the signal value at a specific index
     * @param index Index in the signal vector
     * @return Value at the specified index
     */
    double GetValue(int index);
    
    /**
     * @brief Gets the timestamp at a specific index
     * @param index Index in the signal vector
     * @return Timestamp at the specified index
     */
    struct timespec GetTimestamp(int index);

    /**
     * @brief Applies a simple 1D Kalman filter for signal denoising
     * @param process_noise Process noise covariance (Q)
     * @param measurement_noise Measurement noise covariance (R)
     * @param out_vector Output vector for filtered values (size >= GetIndex())
     * @param initial_estimate Initial state estimate (use 0 if unknown)
     * @param initial_error Initial error covariance (use 1.0 if unknown)
     */
    void KalmanFilter(double process_noise, double measurement_noise, double *out_vector, 
                      double initial_estimate = 0.0, double initial_error = 1.0);
    
    /**
     * @brief Applies wavelet denoising using soft thresholding
     * @param threshold Threshold value for wavelet coefficients
     * @param out_vector Output vector for denoised values (size >= GetIndex())
     * @param level Decomposition level (1-3, default 1)
     */
    void WaveletDenoise(double threshold, double *out_vector, int level = 1);
    
    /**
     * @brief Applies median filter for noise removal
     * @param window_size Window size (must be odd number, e.g., 3, 5, 7)
     * @param out_vector Output vector for filtered values (size >= GetIndex())
     */
    void MedianFilter(int window_size, double *out_vector);
    
    /**
     * @brief Estimates noise level using Median Absolute Deviation (MAD)
     * @return Estimated noise standard deviation
     */
    double EstimateNoiseLevel();

    // ========== ANOMALY DETECTION ==========
    
    /**
     * @brief Detects anomalies using statistical threshold (Z-score method)
     * @param threshold_sigma Number of standard deviations for anomaly threshold (e.g., 3.0)
     * @param anomaly_indices Output array to store indices of detected anomalies
     * @param max_anomalies Maximum number of anomalies to detect
     * @return Number of anomalies detected
     */
    int DetectAnomaliesZScore(double threshold_sigma, int *anomaly_indices, int max_anomalies);
    
    /**
     * @brief Detects anomalies using Interquartile Range (IQR) method
     * @param iqr_multiplier Multiplier for IQR (typically 1.5 for outliers, 3.0 for extreme outliers)
     * @param anomaly_indices Output array to store indices of detected anomalies
     * @param max_anomalies Maximum number of anomalies to detect
     * @return Number of anomalies detected
     */
    int DetectAnomaliesIQR(double iqr_multiplier, int *anomaly_indices, int max_anomalies);
    
    /**
     * @brief Detects anomalies using Moving Average Deviation method
     * @param window_size Window size for moving average
     * @param threshold_factor Multiplication factor for deviation threshold
     * @param anomaly_indices Output array to store indices of detected anomalies
     * @param max_anomalies Maximum number of anomalies to detect
     * @return Number of anomalies detected
     */
    int DetectAnomaliesMAD(int window_size, double threshold_factor, int *anomaly_indices, int max_anomalies);
    
    /**
     * @brief Detects sudden spikes/changes in signal (rate of change anomaly)
     * @param threshold_change Minimum rate of change to be considered anomalous
     * @param anomaly_indices Output array to store indices of detected anomalies
     * @param max_anomalies Maximum number of anomalies to detect
     * @return Number of anomalies detected
     */
    int DetectSuddenChanges(double threshold_change, int *anomaly_indices, int max_anomalies);
    
    /**
     * @brief Segments signal by markers/tags (e.g., turbine blades)
     * @param marker_indices Array of indices where each segment starts (e.g., blade positions)
     * @param num_markers Number of markers
     * @param segment_stats Output array to store statistics for each segment
     * @return Number of segments analyzed
     */
    int SegmentByMarkers(int *marker_indices, int num_markers, SegmentStats *segment_stats);
    
    /**
     * @brief Identifies the segment (e.g., blade) with highest anomaly score
     * @param marker_indices Array of indices where each segment starts
     * @param num_markers Number of markers
     * @param anomaly_method Method to use: 0=ZScore, 1=IQR, 2=MAD, 3=MaxValue
     * @return Index of the most anomalous segment (0-based segment number)
     */
    int FindMostAnomalousSegment(int *marker_indices, int num_markers, int anomaly_method);
    
    /**
     * @brief Detects anomalies in periodic signals (turbine rotations)
     * @param period Expected period length
     * @param tolerance Tolerance for period variations (percentage, e.g., 0.1 for 10%)
     * @param anomaly_indices Output array to store indices of detected anomalies
     * @param max_anomalies Maximum number of anomalies to detect
     * @return Number of anomalies detected
     */
    int DetectPeriodicAnomalies(int period, double tolerance, int *anomaly_indices, int max_anomalies);
    
    /**
     * @brief Calculates anomaly score for entire signal
     * @param method Method to use: 0=ZScore, 1=IQR, 2=MaxDeviation
     * @return Anomaly score (higher = more anomalous)
     */
    double CalculateAnomalyScore(int method);

    // ========== FREQUENCY ANALYSIS ==========
    
    /**
     * @brief Performs FFT on a window of the signal
     * @param start_index Starting index of the window
     * @param window_size Size of the window (will be rounded to nearest power of 2)
     * @param sampling_rate Sampling rate in Hz
     * @param spectrum Output structure containing frequency analysis
     * @return true if successful, false otherwise
     */
    bool FFTAnalysis(int start_index, int window_size, double sampling_rate, FrequencySpectrum *spectrum);
    
    /**
     * @brief Performs FFT on entire signal
     * @param sampling_rate Sampling rate in Hz
     * @param spectrum Output structure containing frequency analysis
     * @return true if successful, false otherwise
     */
    bool FFTAnalysis(double sampling_rate, FrequencySpectrum *spectrum);
    
    /**
     * @brief Finds peaks in frequency spectrum
     * @param spectrum Frequency spectrum to analyze
     * @param min_magnitude Minimum magnitude to be considered a peak
     * @param peak_frequencies Output array for peak frequencies
     * @param peak_magnitudes Output array for peak magnitudes
     * @param max_peaks Maximum number of peaks to find
     * @return Number of peaks found
     */
    int FindFrequencyPeaks(FrequencySpectrum *spectrum, double min_magnitude, 
                          double *peak_frequencies, double *peak_magnitudes, int max_peaks);
    
    /**
     * @brief Calculates power in a frequency band
     * @param spectrum Frequency spectrum to analyze
     * @param freq_low Lower frequency bound (Hz)
     * @param freq_high Upper frequency bound (Hz)
     * @return Total power in the frequency band
     */
    double GetPowerInBand(FrequencySpectrum *spectrum, double freq_low, double freq_high);
    
    /**
     * @brief Analyzes harmonics of a fundamental frequency
     * @param spectrum Frequency spectrum to analyze
     * @param fundamental Fundamental frequency (Hz)
     * @param num_harmonics Number of harmonics to analyze
     * @param harmonic_magnitudes Output array for harmonic magnitudes
     * @return Total harmonic distortion (THD)
     */
    double AnalyzeHarmonics(FrequencySpectrum *spectrum, double fundamental, 
                           int num_harmonics, double *harmonic_magnitudes);
    
    /**
     * @brief Compares frequency spectra of different segments
     * @param marker_indices Array of segment start indices
     * @param num_markers Number of segments
     * @param sampling_rate Sampling rate in Hz
     * @param spectra Output array of spectra for each segment
     * @return Number of spectra computed
     */
    int CompareSegmentSpectra(int *marker_indices, int num_markers, 
                             double sampling_rate, FrequencySpectrum *spectra);
    
    /**
     * @brief Frees memory allocated for frequency spectrum
     * @param spectrum Spectrum to free
     */
    void FreeSpectrum(FrequencySpectrum *spectrum);
    
    /**
     * @brief Detects frequency anomalies by comparing with baseline spectrum
     * @param current_spectrum Current frequency spectrum
     * @param baseline_spectrum Baseline (normal) frequency spectrum
     * @param threshold Threshold for anomaly detection (ratio)
     * @return Anomaly score (0 = normal, higher = more anomalous)
     */
    double DetectFrequencyAnomalies(FrequencySpectrum *current_spectrum, 
                                   FrequencySpectrum *baseline_spectrum, 
                                   double threshold);

    // ========== ML/AI FEATURE EXTRACTION ==========
    
    /**
     * @brief Extracts comprehensive feature vector for ML/AI applications
     * @param sampling_rate Sampling rate in Hz (needed for frequency features)
     * @param features Output structure containing all extracted features
     * @return true if successful, false otherwise
     * 
     * Extracts 21 features from the current signal:
     * - Statistical: mean, std_dev, variance, skewness, kurtosis, RMS, peak-to-peak, crest factor
     * - Frequency: dominant freq, spectral centroid, spectral spread, spectral entropy, power bands
     * - Time domain: zero-crossing rate, energy, autocorrelation peak
     * 
     * Perfect for feeding into neural networks, SVMs, or other ML models
     */
    bool ExtractMLFeatures(double sampling_rate, MLFeatureVector *features);
    
    /**
     * @brief Extracts feature vector from a specific segment/window
     * @param start_index Starting index of the segment
     * @param window_size Size of the segment to analyze
     * @param sampling_rate Sampling rate in Hz
     * @param features Output structure containing all extracted features
     * @return true if successful, false otherwise
     */
    bool ExtractMLFeaturesFromSegment(int start_index, int window_size, 
                                      double sampling_rate, MLFeatureVector *features);
    
    /**
     * @brief Exports features to a flat array for direct ML library input
     * @param features Feature vector structure
     * @param output_array Output array (must be allocated, size >= 21)
     * @return Number of features exported
     * 
     * Exports in order: statistical (8) → frequency (9) → time domain (4)
     * Compatible with TensorFlow, PyTorch, scikit-learn format requirements
     */
    int ExportFeaturesToArray(MLFeatureVector *features, double *output_array);
    
    /**
     * @brief Normalizes feature vector for ML input (z-score normalization)
     * @param features Feature vector to normalize
     * @param mean_values Array of mean values for each feature (size 21)
     * @param std_values Array of std dev values for each feature (size 21)
     * 
     * Apply this before feeding to neural networks for better convergence
     * Compute mean_values and std_values from your training dataset
     */
    void NormalizeMLFeatures(MLFeatureVector *features, double *mean_values, double *std_values);

    // ========== DECIMATION AND INTERPOLATION ==========
    
    /**
     * @brief Decimates signal by keeping every Nth sample (downsampling)
     * @param factor Decimation factor (e.g., 2 = keep every 2nd sample, 4 = every 4th)
     * @param out_vector Output array for decimated signal (size >= GetIndex()/factor)
     * @param apply_antialiasing If true, applies low-pass filter before decimation
     * @return Number of samples in decimated signal
     * 
     * Reduces sampling rate by factor. If apply_antialiasing=true, applies
     * moving average filter to prevent aliasing artifacts.
     */
    int Decimate(int factor, double *out_vector, bool apply_antialiasing = true);
    
    /**
     * @brief Interpolates signal using linear interpolation (upsampling)
     * @param factor Interpolation factor (e.g., 2 = double samples, 4 = quadruple)
     * @param out_vector Output array for interpolated signal (size >= GetIndex()*factor)
     * @return Number of samples in interpolated signal
     * 
     * Increases sampling rate by inserting linearly interpolated values
     * between existing samples.
     */
    int InterpolateLinear(int factor, double *out_vector);
    
    /**
     * @brief Resamples signal to a new sampling rate
     * @param current_rate Current sampling rate in Hz
     * @param target_rate Target sampling rate in Hz
     * @param out_vector Output array for resampled signal (size >= calculated new size)
     * @return Number of samples in resampled signal
     * 
     * Combines decimation and interpolation to achieve arbitrary resampling.
     * Example: 100 Hz → 48 Hz for audio processing
     */
    int Resample(double current_rate, double target_rate, double *out_vector);

    // ========== CORRELATION ANALYSIS ==========
    
    /**
     * @brief Computes autocorrelation of the signal
     * @param max_lag Maximum lag to compute (number of shifts)
     * @param out_correlation Output array for autocorrelation values (size >= max_lag+1)
     * @param normalize If true, normalizes output to [-1, 1] range
     * @return Number of correlation values computed
     * 
     * Autocorrelation measures similarity of signal with delayed version of itself.
     * Useful for:
     * - Detecting periodicity
     * - Finding fundamental frequency
     * - Measuring signal predictability
     */
    int Autocorrelation(int max_lag, double *out_correlation, bool normalize = true);
    
    /**
     * @brief Computes cross-correlation between current signal and another signal
     * @param signal2 Second signal array
     * @param signal2_size Size of second signal
     * @param max_lag Maximum lag to compute (positive and negative)
     * @param out_correlation Output array for correlation values (size >= 2*max_lag+1)
     * @param normalize If true, normalizes output to [-1, 1] range
     * @return Number of correlation values computed
     * 
     * Cross-correlation measures similarity between two signals at different time shifts.
     * Useful for:
     * - Time delay estimation
     * - Pattern matching
     * - Signal alignment
     * - Detecting common patterns
     */
    int CrossCorrelation(double *signal2, int signal2_size, int max_lag, 
                        double *out_correlation, bool normalize = true);
    
    /**
     * @brief Finds the lag with maximum correlation value
     * @param correlation Correlation array (from Autocorrelation or CrossCorrelation)
     * @param size Size of correlation array
     * @param peak_value Output parameter for the peak correlation value
     * @return Lag index with maximum correlation
     * 
     * Useful for finding:
     * - Fundamental period (autocorrelation)
     * - Time delay between signals (cross-correlation)
     */
    int FindCorrelationPeak(double *correlation, int size, double *peak_value);

private:
        int IndexOf(double value, prob_dist *pd);
        void HaarWaveletTransform(double *data, int size, int direction);
        double SoftThreshold(double value, double threshold);
        void QuickSortDouble(double *arr, int low, int high);
        int PartitionDouble(double *arr, int low, int high);
        void FFT(double *real, double *imag, int size, int direction);
        int NextPowerOfTwo(int n);
        void ApplyWindow(double *data, int size, int window_type);
        /**
         * @brief Signal vector
         */
        double SignalVector[NB_MAX_VALUES];
        index_lookup_table index_lookup[MAX_INDX];
        timespec signal_timestamp[NB_MAX_VALUES];
        int item;
        /**
         * @brief total number of values added in SignalVector
         */
        int index;
        timespec timestamp;
        prob_dist *p_d;
        bool threshold_crossing_flag;
        bool zero_crossing_flag;
    };

#endif // SIGNALPROCESSING_H
