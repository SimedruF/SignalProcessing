#include "SignalProcessing.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <chrono>
#ifndef M_PI
#define M_PI 3.14159
#endif

#ifndef M_E
#define M_E 2.71828
#endif
int CompareProbDistItem(const void *a, const void *b);

/// @brief SignalProcessing constructor
SignalProcessing::SignalProcessing()
{
	for (int i = 0; i < NB_MAX_VALUES; i++)
	{
		// write the config items in file
		this->SignalVector[i] = 0.0;
	}
	this->item = 0;
	this->index = 0;
	this->p_d = this->NormalDistributionCreate();
	this->threshold_crossing_flag = false;
	this->zero_crossing_flag = false;
}
/// @brief Clears the signal processing vector
void SignalProcessing::ClearVector()
{
	for (int i = 0; i < NB_MAX_VALUES; i++)
	{
		// write the config items in file
		this->SignalVector[i] = 0.0;
	}
	this->index = 0;
}
/// @brief Adds a value to the signal processing vector
/// @param value Value to be saved in the signal processing vector
/// @return this->index
int SignalProcessing::AddValue(double value)
{

	//clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &(this->signal_timestamp[this->index]));
	#ifdef WINDOWS
	static std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	#elif defined(__linux__)
	struct timespec ts; 
	clock_gettime(CLOCK_REALTIME, &ts);
	#endif
	
	this->SignalVector[this->index] = value;
	if ((this->index >= NB_MAX_VALUES) || (this->index < 0))
	{
		this->index = 0/*  */;
	}	
	else
	{
		this->index++;
	}

	return (this->index);
}

/// @brief Adds a value with timestamp to the signal vector
/// @param value Value to be saved in the signal processing vector
/// @param ts Timestamp to associate with the value
/// @return this->index
int SignalProcessing::AddValueWithTimestamp(double value, struct timespec ts)
{
	this->SignalVector[this->index] = value;
	this->signal_timestamp[this->index].tv_sec = ts.tv_sec;
	this->signal_timestamp[this->index].tv_nsec = ts.tv_nsec;
	
	if ((this->index >= NB_MAX_VALUES) || (this->index < 0))
	{
		this->index = 0;
	}	
	else
	{
		this->index++;
	}

	return (this->index);
}

/// @brief Gets the timestamp for a value in the signal processing vector
/// @param td Contains the timestamp of the signal value at index
/// @param index The index of the signal value
/// @return void
void SignalProcessing::GetTimespec(struct timespec *td, int index)
{
	if (td != nullptr)
	{
		td->tv_nsec = signal_timestamp[index].tv_nsec;
		td->tv_sec = signal_timestamp[index].tv_sec;
	}
}

/// @brief Gets the last value of the signal processing vector
/// @return void
double SignalProcessing::GetLastValue()
{
	return this->SignalVector[this->index-1];
}
/// @brief Sets the item of the signal processing vector
/// @param Item The item
/// @return void
void SignalProcessing::SetItem(int Item)
{
	this->item = Item;
}
/// @brief Gets the max capacity of the signal processing vector
/// @return Max capacity
int SignalProcessing::GetMaxCapacity()
{
	return NB_MAX_VALUES;
}
/// @brief Gets the current index of the signal processing vector
/// @return Current index
int SignalProcessing::GetIndex()
{
	return (this->index);
}
/// @brief Gets the size of the signal processing vector
/// @return Current index
int SignalProcessing::GetSize()
{
	return (this->GetIndex());
}
/// @brief Gets the item number for the signal processing vector
/// @return Item number
int SignalProcessing::GetItem()
{
	return (this->item);
}
/// @brief Gets the signal processing vector
/// @param signalProc_vector Contains the signal processing vector
/// @return void
void SignalProcessing::GetVector(double *signalProc_vector)
{
	if(signalProc_vector != nullptr)
	{
		for (int i = 0; i < this->index-1; i++)
		{
			signalProc_vector[i] = this->SignalVector[i];
		}
	}
}
/// @brief Gets the signal processing vector as int
/// @param signalProc_vector Contains int signal processing vector
/// @return void
void SignalProcessing::GetVectorInt(int *signalProc_vector, int size)
{
	if (signalProc_vector != nullptr)
	{
		for (int i = 0; i < size; i++)
		{
			signalProc_vector[i] = (int)(this->SignalVector[i]);
		}
	}
}

/// @brief Prints the signal processing vector
/// @return void
void SignalProcessing::PrintVector()
{
	for (int i = 0; i < this->GetIndex(); i++)
	{
		printf("\nSignalVector[%d] = %f", i, SignalVector[i]);
	}
	printf("\n------------------------------------------------");
}	


/// @brief Gets the signal processing vector with offset
/// @param signalProc_vector Contains the signal processing vector
/// @return void
void SignalProcessing::GetVectorWithOffset(double *signalProc_vector,int size, int offset)
{
	if (signalProc_vector != nullptr)
	{
		for (int i = 0; i < size; i++)
		{
			signalProc_vector[i] = this->SignalVector[i + offset];
		}
	}
}
/// @brief Initializes the signal processing vector
/// @param values Contains signal processing vector which will be copied to SignalVector
/// @param size Contains size of the signal processing vector which will be copied to SignalVector
/// @return void
void SignalProcessing::InitVector(double *values,int size)
{
	if (values != nullptr)
	{
		for (int i = 0; i < size; ++i)
		{
			this->SignalVector[i] = values[i];
		}
		this->index = size + 1;
	}
}
/// @brief Multiplies the signal processing vector with a scalar value
/// @param value Value
/// @param size Number of elements to multiply
/// @return void
void SignalProcessing::MultiplyWithValue(double value, int size)
{
	for (int i = 0; i < size; ++i)
	{
		if (i<=this->index)
		{
			this->SignalVector[i] *= value;
		}
	}
}
/// @brief Subtracts a scalar value from the signal processing vector
/// @param value Value
/// @param size Number of elements to subtract
/// @return void
void SignalProcessing::SubstractWithValue(double value, int size)
{
	for (int i = 0; i < size; ++i)
	{
		if (i <= this->index)
		{
			this->SignalVector[i] -= value;
		}
	}
}
/// @brief Divides the signal processing vector by a scalar value
/// @param value Value
/// @param size Number of elements to divide
/// @return void
void SignalProcessing::DivideWithValue(double value, int size)
{
	if (value != 0)
	{
		for (int i = 0; i < size; ++i)
		{
			if (i <= this->index)
			{
				this->SignalVector[i] /= value;
			}
		}
	}
}
/// @brief Adds a scalar value to the signal processing vector
/// @param value Value
/// @param size Number of elements to add
/// @return void
void SignalProcessing::AddWithValue(double value, int size)
{
	for (int i = 0; i < size; ++i)
	{
		if (i <= this->index)
		{
			this->SignalVector[i] += value;
		}
	}
}

/// @brief Creates a normal distribution
/// @return prob_dist
prob_dist *SignalProcessing::NormalDistributionCreate()
{
	/* allocate memory for prob _distrib*/
	prob_dist *pd = (prob_dist *)malloc(sizeof(prob_dist));

	pd->count = 0;
	pd->items = NULL;

	pd->total_probability = 0;
	pd->total_normal_probability = 0;
	pd->total_normal_frequency = 0;

	return pd;
}

/// @brief Calculates the normal distribution
/// @param data Contains values
/// @param size Number of elements to add
/// @param pd Probability distribution
/// @return void
void SignalProcessing::NormalDistributionCalculate(double *data, int size, prob_dist *pd)
{
	int index;
	double total = 0;
	double sumofsquares = 0;
	double mean;
	double variance;
	double stddev;
	if ((pd == NULL) || (data == NULL))
	{
		return;
	}
	pd->total_frequency = size;

	// CALCULATE FREQUENCIES
	// iterate data
	// add new values to pd
	// or
	// increment frequency of existing values
	for (int i = 0; i < size; i++)
	{
		index = this->IndexOf(data[i], pd);

		// already exists
		if (index >= 0)
		{
			pd->items[index].frequency++;
		}
		// does not exist
		else
		{
			pd->count++;

			// first item
			if (pd->items == NULL)
			{
				pd->items = (prob_dist_item *)malloc(sizeof(prob_dist_item));
			}
			else
			{
				pd->items = (prob_dist_item *)realloc(pd->items, sizeof(prob_dist_item) * pd->count);
			}

			pd->items[pd->count - 1].value = data[i];
			pd->items[pd->count - 1].frequency = 1;
		}

		total += data[i];
		sumofsquares += pow(data[i], 2);
	}

	// SORT ITEMS
	qsort(pd->items, pd->count, sizeof(prob_dist_item), CompareProbDistItem);

	// CALCULATE MEAN, VARIANCE AND STANDARD DEVIATION
	mean = total / size;
	variance = (sumofsquares - ((pow(total, 2)) / size)) / size;
	stddev = sqrt(variance);

	// CALCULATE PROBABILITIES OF EACH UNIQUE VALUE
	for (int c = 0; c < pd->count; c++)
	{
		pd->items[c].probability = (double)pd->items[c].frequency / (double)size;
		pd->items[c].normal_probability = ((1.0 / (stddev * sqrt(2.0 * M_PI))) * (pow(M_E, -1.0 * ((pow((pd->items[c].value - mean), 2.0)) / (variance * 2.0)))));
		pd->items[c].normal_frequency = pd->items[c].normal_probability * size;
		pd->total_probability += pd->items[c].probability;
		pd->total_normal_probability += pd->items[c].normal_probability;
		pd->total_normal_frequency += pd->items[c].normal_frequency;
	}
}

/// @brief Prints the normal distribution
/// @param pd Probability distribution
/// @return void
void SignalProcessing::NormalDistributionPrint(prob_dist *pd)
{
	printf("Value | Probability | Normal Prob | Freq | Normal Freq\n------------------------------------------------------\n");

	for (int i = 0; i < pd->count; i++)
	{
		printf("%f |%12.6lf |%12.4lf |%5d |%12.4lf\n", pd->items[i].value, pd->items[i].probability, pd->items[i].normal_probability, pd->items[i].frequency, pd->items[i].normal_frequency);
	}

	printf("------------------------------------------------------\n");

	printf("      |%12.4lf |%12.6lf |%5.0lf |%12.4lf\n", pd->total_probability, pd->total_normal_probability, pd->total_frequency, pd->total_normal_frequency);

	printf("------------------------------------------------------\n");
}

/// @brief Frees memory allocated for prob_dist
/// @param pd Probability distribution
/// @return void
void SignalProcessing::NormalDistributionFree()
{
	free(this->p_d->items);
	free(this->p_d);
}

/// @fn IndexOf
/// @brief Returns the index of a value
/// @param value Value
/// @param pd Probability distribution
/// @return int
int SignalProcessing::IndexOf(double value, prob_dist *pd)
{
	if (pd == NULL)
	{
		return -2;
	}
	/* Search the value */
	for (int i = 0; i < pd->count; i++)
	{
		if (pd->items[i].value == value)
		{
			return i;
		}
	}
	return -1;
}
/// @brief Computes the normal distribution and prints the result
/// @return void
void SignalProcessing::NormalDistributionRun()
{
	this->p_d = this->NormalDistributionCreate();
	this->NormalDistributionCalculate(this->SignalVector, this->GetIndex(), this->p_d);
	//this->NormalDistributionPrint(this->p_d);
}
void SignalProcessing::BuildIndexLookupTable(int first_received)
{
	int received_ref = first_received;
	for (int i = 0; i < MAX_INDX; i++)
	{
		if ((first_received + i) >= MAX_INDX)
		{
			index_lookup[i].received = first_received - received_ref;
			received_ref --;
		}
		else
		{
			index_lookup[i].received = first_received + i;
		}
		index_lookup[i].normal = i;
		#if DEBUG_INFO == 1
		printf("Received = %d", index_lookup[i].received);
		printf(" Normal = %d \n", index_lookup[i].normal);
		#endif
	}
	
}
/// @brief Gets the index from the lookup table
/// @param ReceivedIndex Received index
/// @return Returns the normalized index
int SignalProcessing::GetIndexLookupTable(int ReceivedIndex)
{
	for (int i = 0; i < MAX_INDX; i++)
	{
		if (index_lookup[i].received == ReceivedIndex)
		{
			return (index_lookup[i].normal);
		}
		
	}
	return 0;
}

/// @brief Calculates the moving average of the last window_size values
/// @param window_size Number of values to average
/// @return Moving average value
double SignalProcessing::GetMovingAverage(int window_size)
{
    if (window_size <= 0 || this->index == 0)
        return 0.0;
    int start = (this->index > window_size) ? (this->index - window_size) : 0;
    double sum = 0.0;
    int count = 0;
    for (int i = start; i < this->index; ++i)
    {
        sum += this->SignalVector[i];
        count++;
    }
    return (count > 0) ? (sum / count) : 0.0;
}

/// @brief Calculates the moving average for each position and stores it in out_vector
/// @param out_vector Destination vector
/// @param window_size Number of values to average
void SignalProcessing::GetMovingAverageVector(double *out_vector, int window_size)
{
    if (out_vector == nullptr || window_size <= 0)
        return;
    for (int i = 0; i < this->index; ++i)
    {
        int start = (i >= window_size - 1) ? (i - window_size + 1) : 0;
        double sum = 0.0;
        int count = 0;
        for (int j = start; j <= i; ++j)
        {
            sum += this->SignalVector[j];
            count++;
        }
        out_vector[i] = (count > 0) ? (sum / count) : 0.0;
    }
}

/// @brief Calculates the mean (average) of the signal vector
/// @return Mean value
double SignalProcessing::GetMean()
{
    if (this->index == 0)
        return 0.0;
    double sum = 0.0;
    for (int i = 0; i < this->index; ++i)
        sum += this->SignalVector[i];
    return sum / this->index;
}

/// @brief Calculates the variance of the signal vector
/// @return Variance value
double SignalProcessing::GetVariance()
{
    if (this->index == 0)
        return 0.0;
    double mean = GetMean();
    double sum = 0.0;
    for (int i = 0; i < this->index; ++i)
        sum += (this->SignalVector[i] - mean) * (this->SignalVector[i] - mean);
    return sum / this->index;
}

/// @brief Calculates the standard deviation of the signal vector
/// @return Standard deviation value
double SignalProcessing::GetStandardDeviation()
{
    return sqrt(GetVariance());
}

/// @brief Normalizes the signal vector to [0, 1] range
void SignalProcessing::NormalizeVector()
{
    if (this->index == 0)
        return;
    double min_val = this->SignalVector[0];
    double max_val = this->SignalVector[0];
    for (int i = 1; i < this->index; ++i)
    {
        if (this->SignalVector[i] < min_val) min_val = this->SignalVector[i];
        if (this->SignalVector[i] > max_val) max_val = this->SignalVector[i];
    }
    double range = max_val - min_val;
    if (range == 0.0) return;
    for (int i = 0; i < this->index; ++i)
        this->SignalVector[i] = (this->SignalVector[i] - min_val) / range;
}

/// @brief Scales the signal vector to a given range [new_min, new_max]
/// @param new_min Minimum value of the new range
/// @param new_max Maximum value of the new range
void SignalProcessing::ScaleVector(double new_min, double new_max)
{
    if (this->index == 0)
        return;
    double min_val = this->SignalVector[0];
    double max_val = this->SignalVector[0];
    for (int i = 1; i < this->index; ++i)
    {
        if (this->SignalVector[i] < min_val) min_val = this->SignalVector[i];
        if (this->SignalVector[i] > max_val) max_val = this->SignalVector[i];
    }
    double range = max_val - min_val;
    if (range == 0.0) return;
    for (int i = 0; i < this->index; ++i)
    {
        this->SignalVector[i] = new_min + ((this->SignalVector[i] - min_val) / range) * (new_max - new_min);
    }
}

/// @brief Applies exponential smoothing to the signal vector
/// @param alpha Smoothing factor (0 < alpha <= 1)
/// @param out_vector Destination vector for smoothed values (size >= GetIndex())
void SignalProcessing::ExponentialSmoothing(double alpha, double *out_vector)
{
    if (out_vector == nullptr || this->index == 0 || alpha <= 0.0 || alpha > 1.0)
        return;
    out_vector[0] = this->SignalVector[0];
    for (int i = 1; i < this->index; ++i)
    {
        out_vector[i] = alpha * this->SignalVector[i] + (1.0 - alpha) * out_vector[i - 1];
    }
}

/// @brief Detects threshold crossing events in the signal vector
/// @param threshold The threshold value to detect
/// @param direction 1 for rising edge (below->above), -1 for falling edge (above->below), 0 for both
/// @param events Output array to store indices where crossings occur (size >= GetIndex())
/// @return Number of threshold crossings detected
int SignalProcessing::DetectThresholdCrossing(double threshold, int direction, int *events)
{
    if (events == nullptr || this->index < 2)
        return 0;
    
    int event_count = 0;
    this->threshold_crossing_flag = false;
    
    for (int i = 1; i < this->index; ++i)
    {
        bool rising = (this->SignalVector[i - 1] < threshold) && (this->SignalVector[i] >= threshold);
        bool falling = (this->SignalVector[i - 1] > threshold) && (this->SignalVector[i] <= threshold);
        
        if ((direction == 1 && rising) || (direction == -1 && falling) || (direction == 0 && (rising || falling)))
        {
            events[event_count++] = i;
            this->threshold_crossing_flag = true;
        }
    }
    
    return event_count;
}

/// @brief Detects zero-crossing events in the signal vector
/// @param direction 1 for positive crossing (negative->positive), -1 for negative crossing (positive->negative), 0 for both
/// @param events Output array to store indices where crossings occur (size >= GetIndex())
/// @return Number of zero crossings detected
int SignalProcessing::DetectZeroCrossing(int direction, int *events)
{
    if (events == nullptr || this->index < 2)
        return 0;
    
    int event_count = 0;
    this->zero_crossing_flag = false;
    
    for (int i = 1; i < this->index; ++i)
    {
        bool positive_crossing = (this->SignalVector[i - 1] < 0.0) && (this->SignalVector[i] >= 0.0);
        bool negative_crossing = (this->SignalVector[i - 1] > 0.0) && (this->SignalVector[i] <= 0.0);
        
        if ((direction == 1 && positive_crossing) || (direction == -1 && negative_crossing) || (direction == 0 && (positive_crossing || negative_crossing)))
        {
            events[event_count++] = i;
            this->zero_crossing_flag = true;
        }
    }
    
    return event_count;
}

/// @brief Gets the threshold crossing flag status
/// @return True if threshold crossing was detected
bool SignalProcessing::GetThresholdCrossingFlag()
{
    return this->threshold_crossing_flag;
}

/// @brief Gets the zero crossing flag status
/// @return True if zero crossing was detected
bool SignalProcessing::GetZeroCrossingFlag()
{
    return this->zero_crossing_flag;
}

/// @brief Clears event detection flags
void SignalProcessing::ClearEventFlags()
{
    this->threshold_crossing_flag = false;
    this->zero_crossing_flag = false;
}

/// @fn CompareProbDistItem
/// @brief Returns -1 if a<b ; 1 if a >b else returns 0
/// @param const void *a
/// @param const void *b
/// @return int
int CompareProbDistItem(const void *a, const void *b)
{
	if (((prob_dist_item *)a)->value < ((prob_dist_item *)b)->value)
		return -1;
	else if (((prob_dist_item *)a)->value > ((prob_dist_item *)b)->value)
		return 1;
	else
		return 0;
}

/// @brief Detects peaks (local maxima) in the signal vector
/// @param peaks Output array to store peak indices
/// @param max_peaks Maximum number of peaks to detect
/// @return Number of peaks detected
int SignalProcessing::DetectPeaks(int *peaks, int max_peaks)
{
    if (peaks == nullptr || max_peaks <= 0 || this->index < 3)
        return 0;
    
    int peak_count = 0;
    
    // Check each point (excluding first and last)
    for (int i = 1; i < this->index - 1 && peak_count < max_peaks; ++i)
    {
        // A peak is a point higher than both neighbors
        if (this->SignalVector[i] > this->SignalVector[i - 1] && 
            this->SignalVector[i] > this->SignalVector[i + 1])
        {
            peaks[peak_count++] = i;
        }
    }
    
    return peak_count;
}

/// @brief Detects peaks above a threshold value
/// @param threshold Minimum value for peak detection
/// @param peaks Output array to store peak indices
/// @param max_peaks Maximum number of peaks to detect
/// @return Number of peaks detected
int SignalProcessing::DetectPeaksWithThreshold(double threshold, int *peaks, int max_peaks)
{
    if (peaks == nullptr || max_peaks <= 0 || this->index < 3)
        return 0;
    
    int peak_count = 0;
    
    // Check each point (excluding first and last)
    for (int i = 1; i < this->index - 1 && peak_count < max_peaks; ++i)
    {
        // A peak must be higher than both neighbors AND above threshold
        if (this->SignalVector[i] > this->SignalVector[i - 1] && 
            this->SignalVector[i] > this->SignalVector[i + 1] &&
            this->SignalVector[i] >= threshold)
        {
            peaks[peak_count++] = i;
        }
    }
    
    return peak_count;
}

/// @brief Detects peaks with minimum prominence
/// @param min_prominence Minimum prominence (height above surrounding valleys)
/// @param peaks Output array to store peak indices
/// @param max_peaks Maximum number of peaks to detect
/// @return Number of peaks detected
int SignalProcessing::DetectPeaksWithProminence(double min_prominence, int *peaks, int max_peaks)
{
    if (peaks == nullptr || max_peaks <= 0 || this->index < 3 || min_prominence < 0)
        return 0;
    
    int peak_count = 0;
    
    // First detect all local maxima
    for (int i = 1; i < this->index - 1 && peak_count < max_peaks; ++i)
    {
        if (this->SignalVector[i] > this->SignalVector[i - 1] && 
            this->SignalVector[i] > this->SignalVector[i + 1])
        {
            // Calculate prominence: find minimum on both sides
            double left_min = this->SignalVector[i];
            double right_min = this->SignalVector[i];
            
            // Search left for minimum
            for (int j = i - 1; j >= 0; --j)
            {
                if (this->SignalVector[j] < left_min)
                    left_min = this->SignalVector[j];
                // Stop at higher peak
                if (this->SignalVector[j] > this->SignalVector[i])
                    break;
            }
            
            // Search right for minimum
            for (int j = i + 1; j < this->index; ++j)
            {
                if (this->SignalVector[j] < right_min)
                    right_min = this->SignalVector[j];
                // Stop at higher peak
                if (this->SignalVector[j] > this->SignalVector[i])
                    break;
            }
            
            // Prominence is the minimum of the two side differences
            double left_prominence = this->SignalVector[i] - left_min;
            double right_prominence = this->SignalVector[i] - right_min;
            double prominence = (left_prominence < right_prominence) ? left_prominence : right_prominence;
            
            if (prominence >= min_prominence)
            {
                peaks[peak_count++] = i;
            }
        }
    }
    
    return peak_count;
}

/// @brief Detects peaks with minimum distance between them
/// @param min_distance Minimum distance between consecutive peaks
/// @param peaks Output array to store peak indices
/// @param max_peaks Maximum number of peaks to detect
/// @return Number of peaks detected
int SignalProcessing::DetectPeaksWithDistance(int min_distance, int *peaks, int max_peaks)
{
    if (peaks == nullptr || max_peaks <= 0 || this->index < 3 || min_distance < 1)
        return 0;
    
    // First detect all local maxima with their values
    int temp_peaks[NB_MAX_VALUES];
    double peak_values[NB_MAX_VALUES];
    int temp_count = 0;
    
    for (int i = 1; i < this->index - 1; ++i)
    {
        if (this->SignalVector[i] > this->SignalVector[i - 1] && 
            this->SignalVector[i] > this->SignalVector[i + 1])
        {
            temp_peaks[temp_count] = i;
            peak_values[temp_count] = this->SignalVector[i];
            temp_count++;
        }
    }
    
    if (temp_count == 0)
        return 0;
    
    // Sort peaks by value (descending) to prioritize higher peaks
    // Simple bubble sort for this implementation
    for (int i = 0; i < temp_count - 1; ++i)
    {
        for (int j = 0; j < temp_count - i - 1; ++j)
        {
            if (peak_values[j] < peak_values[j + 1])
            {
                // Swap values
                double temp_val = peak_values[j];
                peak_values[j] = peak_values[j + 1];
                peak_values[j + 1] = temp_val;
                // Swap indices
                int temp_idx = temp_peaks[j];
                temp_peaks[j] = temp_peaks[j + 1];
                temp_peaks[j + 1] = temp_idx;
            }
        }
    }
    
    // Select peaks respecting minimum distance
    bool selected[NB_MAX_VALUES] = {false};
    int peak_count = 0;
    
    for (int i = 0; i < temp_count && peak_count < max_peaks; ++i)
    {
        bool too_close = false;
        
        // Check distance to all already selected peaks
        for (int j = 0; j < temp_count; ++j)
        {
            if (selected[j])
            {
                int distance = temp_peaks[i] - temp_peaks[j];
                if (distance < 0) distance = -distance; // abs()
                
                if (distance < min_distance)
                {
                    too_close = true;
                    break;
                }
            }
        }
        
        if (!too_close)
        {
            selected[i] = true;
            peaks[peak_count++] = temp_peaks[i];
        }
    }
    
    // Sort final peaks by index for easier interpretation
    for (int i = 0; i < peak_count - 1; ++i)
    {
        for (int j = 0; j < peak_count - i - 1; ++j)
        {
            if (peaks[j] > peaks[j + 1])
            {
                int temp = peaks[j];
                peaks[j] = peaks[j + 1];
                peaks[j + 1] = temp;
            }
        }
    }
    
    return peak_count;
}

/// @brief Gets the value at a specific peak index
/// @param peak_index Index of the peak in the signal vector
/// @return Value at the peak
double SignalProcessing::GetPeakValue(int peak_index)
{
    if (peak_index < 0 || peak_index >= this->index)
        return 0.0;
    
    return this->SignalVector[peak_index];
}

/// @brief Gets the signal value at a specific index
/// @param index Index in the signal vector
/// @return Value at the specified index
double SignalProcessing::GetValue(int index)
{
    if (index < 0 || index >= this->index)
        return 0.0;
    
    return this->SignalVector[index];
}

/// @brief Gets the timestamp at a specific index
/// @param index Index in the signal vector
/// @return Timestamp at the specified index
struct timespec SignalProcessing::GetTimestamp(int index)
{
    struct timespec empty_ts = {0, 0};
    
    if (index < 0 || index >= this->index)
        return empty_ts;
    
    return this->signal_timestamp[index];
}

