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

private:
        int IndexOf(double value, prob_dist *pd);
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
