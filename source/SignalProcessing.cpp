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

/// @brief Applies a simple 1D Kalman filter for signal denoising
/// @param process_noise Process noise covariance (Q)
/// @param measurement_noise Measurement noise covariance (R)
/// @param out_vector Output vector for filtered values
/// @param initial_estimate Initial state estimate
/// @param initial_error Initial error covariance
void SignalProcessing::KalmanFilter(double process_noise, double measurement_noise, 
                                   double *out_vector, double initial_estimate, double initial_error)
{
    if (out_vector == nullptr || this->index == 0)
        return;
    
    // Kalman filter state variables
    double estimate = initial_estimate;
    double error_covariance = initial_error;
    
    for (int i = 0; i < this->index; ++i)
    {
        // Prediction step
        double predicted_estimate = estimate;
        double predicted_error = error_covariance + process_noise;
        
        // Update step
        double kalman_gain = predicted_error / (predicted_error + measurement_noise);
        estimate = predicted_estimate + kalman_gain * (this->SignalVector[i] - predicted_estimate);
        error_covariance = (1.0 - kalman_gain) * predicted_error;
        
        out_vector[i] = estimate;
    }
}

/// @brief Soft thresholding function for wavelet denoising
/// @param value Input value
/// @param threshold Threshold value
/// @return Thresholded value
double SignalProcessing::SoftThreshold(double value, double threshold)
{
    if (value > threshold)
        return value - threshold;
    else if (value < -threshold)
        return value + threshold;
    else
        return 0.0;
}

/// @brief Partition function for quicksort
/// @param arr Array to partition
/// @param low Starting index
/// @param high Ending index
/// @return Partition index
int SignalProcessing::PartitionDouble(double *arr, int low, int high)
{
    double pivot = arr[high];
    int i = low - 1;
    
    for (int j = low; j < high; ++j)
    {
        if (arr[j] <= pivot)
        {
            i++;
            double temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    
    double temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    
    return i + 1;
}

/// @brief Quicksort implementation for doubles
/// @param arr Array to sort
/// @param low Starting index
/// @param high Ending index
void SignalProcessing::QuickSortDouble(double *arr, int low, int high)
{
    if (low < high)
    {
        int pi = PartitionDouble(arr, low, high);
        QuickSortDouble(arr, low, pi - 1);
        QuickSortDouble(arr, pi + 1, high);
    }
}

/// @brief Haar wavelet transform (1D)
/// @param data Data array
/// @param size Size of data (must be power of 2)
/// @param direction 1 for forward, -1 for inverse
void SignalProcessing::HaarWaveletTransform(double *data, int size, int direction)
{
    if (data == nullptr || size < 2)
        return;
    
    double temp[NB_MAX_VALUES];
    
    if (direction == 1) // Forward transform
    {
        int half = size / 2;
        for (int i = 0; i < half; ++i)
        {
            temp[i] = (data[2 * i] + data[2 * i + 1]) / 1.414213562373095; // sqrt(2)
            temp[half + i] = (data[2 * i] - data[2 * i + 1]) / 1.414213562373095;
        }
        for (int i = 0; i < size; ++i)
            data[i] = temp[i];
    }
    else // Inverse transform
    {
        int half = size / 2;
        for (int i = 0; i < half; ++i)
        {
            temp[2 * i] = (data[i] + data[half + i]) / 1.414213562373095;
            temp[2 * i + 1] = (data[i] - data[half + i]) / 1.414213562373095;
        }
        for (int i = 0; i < size; ++i)
            data[i] = temp[i];
    }
}

/// @brief Applies wavelet denoising using soft thresholding
/// @param threshold Threshold value for wavelet coefficients
/// @param out_vector Output vector for denoised values
/// @param level Decomposition level
void SignalProcessing::WaveletDenoise(double threshold, double *out_vector, int level)
{
    if (out_vector == nullptr || this->index == 0)
        return;
    
    // Copy signal to output
    for (int i = 0; i < this->index; ++i)
        out_vector[i] = this->SignalVector[i];
    
    // Find nearest power of 2
    int size = this->index;
    int transform_size = 1;
    while (transform_size < size)
        transform_size *= 2;
    
    // Pad with zeros if needed
    for (int i = size; i < transform_size; ++i)
        out_vector[i] = 0.0;
    
    // Apply forward wavelet transform multiple times
    int current_size = transform_size;
    for (int l = 0; l < level && current_size >= 2; ++l)
    {
        HaarWaveletTransform(out_vector, current_size, 1);
        current_size /= 2;
    }
    
    // Apply soft thresholding to detail coefficients
    // Keep approximation coefficients (first current_size elements) untouched
    for (int i = current_size; i < transform_size; ++i)
    {
        out_vector[i] = SoftThreshold(out_vector[i], threshold);
    }
    
    // Apply inverse wavelet transform
    current_size *= 2;
    for (int l = 0; l < level && current_size <= transform_size; ++l)
    {
        HaarWaveletTransform(out_vector, current_size, -1);
        current_size *= 2;
    }
    
    // Trim to original size
    for (int i = size; i < transform_size; ++i)
        out_vector[i] = 0.0;
}

/// @brief Applies median filter for noise removal
/// @param window_size Window size (must be odd)
/// @param out_vector Output vector for filtered values
void SignalProcessing::MedianFilter(int window_size, double *out_vector)
{
    if (out_vector == nullptr || this->index == 0 || window_size < 1)
        return;
    
    // Ensure odd window size
    if (window_size % 2 == 0)
        window_size++;
    
    int half_window = window_size / 2;
    double window[101]; // Support up to window size 101
    
    if (window_size > 101)
        window_size = 101;
    
    for (int i = 0; i < this->index; ++i)
    {
        int count = 0;
        
        // Collect values in window
        for (int j = -half_window; j <= half_window; ++j)
        {
            int idx = i + j;
            if (idx >= 0 && idx < this->index)
            {
                window[count++] = this->SignalVector[idx];
            }
        }
        
        // Sort window to find median
        if (count > 0)
        {
            QuickSortDouble(window, 0, count - 1);
            
            // Get median
            if (count % 2 == 1)
                out_vector[i] = window[count / 2];
            else
                out_vector[i] = (window[count / 2 - 1] + window[count / 2]) / 2.0;
        }
        else
        {
            out_vector[i] = this->SignalVector[i];
        }
    }
}

/// @brief Estimates noise level using Median Absolute Deviation (MAD)
/// @return Estimated noise standard deviation
double SignalProcessing::EstimateNoiseLevel()
{
    if (this->index < 2)
        return 0.0;
    
    // Calculate differences (high-pass filter approximation)
    double differences[NB_MAX_VALUES];
    for (int i = 0; i < this->index - 1; ++i)
    {
        differences[i] = this->SignalVector[i + 1] - this->SignalVector[i];
    }
    int diff_size = this->index - 1;
    
    // Calculate absolute values
    for (int i = 0; i < diff_size; ++i)
    {
        if (differences[i] < 0)
            differences[i] = -differences[i];
    }
    
    // Sort to find median
    QuickSortDouble(differences, 0, diff_size - 1);
    
    // Get median
    double median;
    if (diff_size % 2 == 1)
        median = differences[diff_size / 2];
    else
        median = (differences[diff_size / 2 - 1] + differences[diff_size / 2]) / 2.0;
    
    // Estimate noise sigma using MAD
    // sigma ≈ MAD / 0.6745
    return median / 0.6745;
}

// ========== ANOMALY DETECTION IMPLEMENTATION ==========

/// @brief Detects anomalies using Z-score method (statistical threshold)
/// @param threshold_sigma Number of standard deviations for anomaly threshold
/// @param anomaly_indices Output array for anomaly indices
/// @param max_anomalies Maximum number of anomalies to detect
/// @return Number of anomalies detected
int SignalProcessing::DetectAnomaliesZScore(double threshold_sigma, int *anomaly_indices, int max_anomalies)
{
    if (anomaly_indices == nullptr || this->index < 3 || max_anomalies <= 0)
        return 0;
    
    double mean = GetMean();
    double std_dev = GetStandardDeviation();
    
    if (std_dev == 0.0)
        return 0;
    
    int anomaly_count = 0;
    
    for (int i = 0; i < this->index && anomaly_count < max_anomalies; ++i)
    {
        double z_score = (this->SignalVector[i] - mean) / std_dev;
        
        // Check if absolute z-score exceeds threshold
        if (z_score > threshold_sigma || z_score < -threshold_sigma)
        {
            anomaly_indices[anomaly_count++] = i;
        }
    }
    
    return anomaly_count;
}

/// @brief Detects anomalies using Interquartile Range (IQR) method
/// @param iqr_multiplier Multiplier for IQR (1.5 for outliers, 3.0 for extreme)
/// @param anomaly_indices Output array for anomaly indices
/// @param max_anomalies Maximum number of anomalies to detect
/// @return Number of anomalies detected
int SignalProcessing::DetectAnomaliesIQR(double iqr_multiplier, int *anomaly_indices, int max_anomalies)
{
    if (anomaly_indices == nullptr || this->index < 4 || max_anomalies <= 0)
        return 0;
    
    // Copy and sort data to find quartiles
    double sorted[NB_MAX_VALUES];
    for (int i = 0; i < this->index; ++i)
        sorted[i] = this->SignalVector[i];
    
    QuickSortDouble(sorted, 0, this->index - 1);
    
    // Calculate Q1, Q3
    int q1_pos = this->index / 4;
    int q3_pos = (3 * this->index) / 4;
    
    double q1 = sorted[q1_pos];
    double q3 = sorted[q3_pos];
    double iqr = q3 - q1;
    
    // Calculate bounds
    double lower_bound = q1 - iqr_multiplier * iqr;
    double upper_bound = q3 + iqr_multiplier * iqr;
    
    int anomaly_count = 0;
    
    for (int i = 0; i < this->index && anomaly_count < max_anomalies; ++i)
    {
        if (this->SignalVector[i] < lower_bound || this->SignalVector[i] > upper_bound)
        {
            anomaly_indices[anomaly_count++] = i;
        }
    }
    
    return anomaly_count;
}

/// @brief Detects anomalies using Moving Average Deviation method
/// @param window_size Window size for moving average
/// @param threshold_factor Multiplication factor for deviation threshold
/// @param anomaly_indices Output array for anomaly indices
/// @param max_anomalies Maximum number of anomalies to detect
/// @return Number of anomalies detected
int SignalProcessing::DetectAnomaliesMAD(int window_size, double threshold_factor, int *anomaly_indices, int max_anomalies)
{
    if (anomaly_indices == nullptr || this->index < window_size || max_anomalies <= 0)
        return 0;
    
    int anomaly_count = 0;
    
    for (int i = window_size; i < this->index && anomaly_count < max_anomalies; ++i)
    {
        // Calculate moving average for window before current point
        double sum = 0.0;
        for (int j = i - window_size; j < i; ++j)
            sum += this->SignalVector[j];
        double moving_avg = sum / window_size;
        
        // Calculate standard deviation in window
        double variance_sum = 0.0;
        for (int j = i - window_size; j < i; ++j)
        {
            double diff = this->SignalVector[j] - moving_avg;
            variance_sum += diff * diff;
        }
        double std_dev = sqrt(variance_sum / window_size);
        
        // Check if current point deviates significantly
        double deviation = this->SignalVector[i] - moving_avg;
        if (deviation < 0) deviation = -deviation;
        
        if (std_dev > 0 && deviation > threshold_factor * std_dev)
        {
            anomaly_indices[anomaly_count++] = i;
        }
    }
    
    return anomaly_count;
}

/// @brief Detects sudden changes/spikes in signal (rate of change anomaly)
/// @param threshold_change Minimum rate of change to be considered anomalous
/// @param anomaly_indices Output array for anomaly indices
/// @param max_anomalies Maximum number of anomalies to detect
/// @return Number of anomalies detected
int SignalProcessing::DetectSuddenChanges(double threshold_change, int *anomaly_indices, int max_anomalies)
{
    if (anomaly_indices == nullptr || this->index < 2 || max_anomalies <= 0)
        return 0;
    
    int anomaly_count = 0;
    
    for (int i = 1; i < this->index && anomaly_count < max_anomalies; ++i)
    {
        double change = this->SignalVector[i] - this->SignalVector[i - 1];
        if (change < 0) change = -change;
        
        if (change >= threshold_change)
        {
            anomaly_indices[anomaly_count++] = i;
        }
    }
    
    return anomaly_count;
}

/// @brief Segments signal by markers and calculates statistics for each segment
/// @param marker_indices Array of indices where each segment starts
/// @param num_markers Number of markers
/// @param segment_stats Output array for segment statistics
/// @return Number of segments analyzed
int SignalProcessing::SegmentByMarkers(int *marker_indices, int num_markers, SegmentStats *segment_stats)
{
    if (marker_indices == nullptr || segment_stats == nullptr || num_markers < 1)
        return 0;
    
    int segment_count = 0;
    
    for (int seg = 0; seg < num_markers; ++seg)
    {
        int start = marker_indices[seg];
        int end = (seg < num_markers - 1) ? marker_indices[seg + 1] - 1 : this->index - 1;
        
        if (start >= this->index || start < 0 || end >= this->index || start > end)
            continue;
        
        // Calculate statistics for this segment
        segment_stats[segment_count].start_index = start;
        segment_stats[segment_count].end_index = end;
        segment_stats[segment_count].segment_id = seg;
        segment_stats[segment_count].num_points = end - start + 1;
        
        // Calculate mean
        double sum = 0.0;
        double max_val = this->SignalVector[start];
        double min_val = this->SignalVector[start];
        
        for (int i = start; i <= end; ++i)
        {
            sum += this->SignalVector[i];
            if (this->SignalVector[i] > max_val) max_val = this->SignalVector[i];
            if (this->SignalVector[i] < min_val) min_val = this->SignalVector[i];
        }
        
        segment_stats[segment_count].mean = sum / segment_stats[segment_count].num_points;
        segment_stats[segment_count].max_value = max_val;
        segment_stats[segment_count].min_value = min_val;
        segment_stats[segment_count].peak_to_peak = max_val - min_val;
        
        // Calculate standard deviation and RMS
        double variance_sum = 0.0;
        double rms_sum = 0.0;
        
        for (int i = start; i <= end; ++i)
        {
            double diff = this->SignalVector[i] - segment_stats[segment_count].mean;
            variance_sum += diff * diff;
            rms_sum += this->SignalVector[i] * this->SignalVector[i];
        }
        
        segment_stats[segment_count].std_dev = sqrt(variance_sum / segment_stats[segment_count].num_points);
        segment_stats[segment_count].rms = sqrt(rms_sum / segment_stats[segment_count].num_points);
        
        // Initial anomaly score based on deviation from global mean
        double global_mean = GetMean();
        segment_stats[segment_count].anomaly_score = segment_stats[segment_count].mean - global_mean;
        if (segment_stats[segment_count].anomaly_score < 0)
            segment_stats[segment_count].anomaly_score = -segment_stats[segment_count].anomaly_score;
        
        segment_count++;
    }
    
    return segment_count;
}

/// @brief Finds the segment with highest anomaly score
/// @param marker_indices Array of indices where each segment starts
/// @param num_markers Number of markers
/// @param anomaly_method Method: 0=ZScore, 1=IQR, 2=MAD, 3=MaxValue
/// @return Index of most anomalous segment
int SignalProcessing::FindMostAnomalousSegment(int *marker_indices, int num_markers, int anomaly_method)
{
    if (marker_indices == nullptr || num_markers < 1)
        return -1;
    
    SegmentStats segment_stats[100];
    int num_segments = SegmentByMarkers(marker_indices, num_markers, segment_stats);
    
    if (num_segments == 0)
        return -1;
    
    int most_anomalous = 0;
    double max_score = 0.0;
    
    // Calculate global statistics for comparison
    double global_mean = GetMean();
    double global_std = GetStandardDeviation();
    
    for (int i = 0; i < num_segments; ++i)
    {
        double score = 0.0;
        
        switch (anomaly_method)
        {
            case 0: // Z-Score based on mean deviation
                if (global_std > 0)
                {
                    score = (segment_stats[i].mean - global_mean) / global_std;
                    if (score < 0) score = -score;
                }
                break;
                
            case 1: // Based on standard deviation (variability)
                score = segment_stats[i].std_dev;
                break;
                
            case 2: // Based on RMS
                score = segment_stats[i].rms;
                break;
                
            case 3: // Based on max value
                score = segment_stats[i].max_value;
                if (score < 0) score = -score;
                break;
                
            default:
                score = segment_stats[i].anomaly_score;
                break;
        }
        
        segment_stats[i].anomaly_score = score;
        
        if (score > max_score)
        {
            max_score = score;
            most_anomalous = i;
        }
    }
    
    return most_anomalous;
}

/// @brief Detects anomalies in periodic signals
/// @param period Expected period length
/// @param tolerance Tolerance for period variations (0.0 to 1.0)
/// @param anomaly_indices Output array for anomaly indices
/// @param max_anomalies Maximum number of anomalies to detect
/// @return Number of anomalies detected
int SignalProcessing::DetectPeriodicAnomalies(int period, double tolerance, int *anomaly_indices, int max_anomalies)
{
    if (anomaly_indices == nullptr || this->index < period * 2 || max_anomalies <= 0)
        return 0;
    
    int anomaly_count = 0;
    int num_cycles = this->index / period;
    
    // Calculate average pattern for one period
    double avg_pattern[NB_MAX_VALUES];
    for (int i = 0; i < period; ++i)
        avg_pattern[i] = 0.0;
    
    for (int cycle = 0; cycle < num_cycles; ++cycle)
    {
        for (int i = 0; i < period; ++i)
        {
            int idx = cycle * period + i;
            if (idx < this->index)
                avg_pattern[i] += this->SignalVector[idx];
        }
    }
    
    for (int i = 0; i < period; ++i)
        avg_pattern[i] /= num_cycles;
    
    // Calculate standard deviation for each position in period
    double std_pattern[NB_MAX_VALUES];
    for (int i = 0; i < period; ++i)
    {
        double variance = 0.0;
        for (int cycle = 0; cycle < num_cycles; ++cycle)
        {
            int idx = cycle * period + i;
            if (idx < this->index)
            {
                double diff = this->SignalVector[idx] - avg_pattern[i];
                variance += diff * diff;
            }
        }
        std_pattern[i] = sqrt(variance / num_cycles);
    }
    
    // Detect deviations
    for (int i = 0; i < this->index && anomaly_count < max_anomalies; ++i)
    {
        int pattern_pos = i % period;
        double expected = avg_pattern[pattern_pos];
        double std_dev = std_pattern[pattern_pos];
        
        if (std_dev > 0)
        {
            double deviation = this->SignalVector[i] - expected;
            if (deviation < 0) deviation = -deviation;
            
            if (deviation > tolerance * std_dev)
            {
                anomaly_indices[anomaly_count++] = i;
            }
        }
    }
    
    return anomaly_count;
}

/// @brief Calculates overall anomaly score for the signal
/// @param method Method: 0=ZScore, 1=IQR, 2=MaxDeviation
/// @return Anomaly score
double SignalProcessing::CalculateAnomalyScore(int method)
{
    if (this->index < 2)
        return 0.0;
    
    double score = 0.0;
    
    switch (method)
    {
        case 0: // Based on max Z-score
        {
            double mean = GetMean();
            double std_dev = GetStandardDeviation();
            if (std_dev > 0)
            {
                double max_zscore = 0.0;
                for (int i = 0; i < this->index; ++i)
                {
                    double z = (this->SignalVector[i] - mean) / std_dev;
                    if (z < 0) z = -z;
                    if (z > max_zscore) max_zscore = z;
                }
                score = max_zscore;
            }
            break;
        }
        
        case 1: // Based on IQR ratio
        {
            double sorted[NB_MAX_VALUES];
            for (int i = 0; i < this->index; ++i)
                sorted[i] = this->SignalVector[i];
            
            QuickSortDouble(sorted, 0, this->index - 1);
            
            int q1_pos = this->index / 4;
            int q3_pos = (3 * this->index) / 4;
            double iqr = sorted[q3_pos] - sorted[q1_pos];
            double range = sorted[this->index - 1] - sorted[0];
            
            if (iqr > 0)
                score = range / iqr;
            break;
        }
        
        case 2: // Based on max deviation from mean
        {
            double mean = GetMean();
            double max_dev = 0.0;
            for (int i = 0; i < this->index; ++i)
            {
                double dev = this->SignalVector[i] - mean;
                if (dev < 0) dev = -dev;
                if (dev > max_dev) max_dev = dev;
            }
            score = max_dev;
            break;
        }
        
        default:
            score = GetStandardDeviation();
            break;
    }
    
    return score;
}

// ========== FREQUENCY ANALYSIS IMPLEMENTATION ==========

/// @brief Finds next power of 2 greater than or equal to n
/// @param n Input number
/// @return Next power of 2
int SignalProcessing::NextPowerOfTwo(int n)
{
    int power = 1;
    while (power < n)
        power *= 2;
    return power;
}

/// @brief Applies window function to data
/// @param data Data array
/// @param size Size of data
/// @param window_type 0=Rectangular, 1=Hann, 2=Hamming, 3=Blackman
void SignalProcessing::ApplyWindow(double *data, int size, int window_type)
{
    if (data == nullptr || size < 1)
        return;
    
    for (int i = 0; i < size; ++i)
    {
        double w = 1.0;
        
        switch (window_type)
        {
            case 1: // Hann window
                w = 0.5 * (1.0 - cos(2.0 * M_PI * i / (size - 1)));
                break;
                
            case 2: // Hamming window
                w = 0.54 - 0.46 * cos(2.0 * M_PI * i / (size - 1));
                break;
                
            case 3: // Blackman window
                w = 0.42 - 0.5 * cos(2.0 * M_PI * i / (size - 1)) + 
                    0.08 * cos(4.0 * M_PI * i / (size - 1));
                break;
                
            default: // Rectangular (no window)
                w = 1.0;
                break;
        }
        
        data[i] *= w;
    }
}

/// @brief Cooley-Tukey FFT algorithm
/// @param real Real part of signal
/// @param imag Imaginary part of signal
/// @param size Size (must be power of 2)
/// @param direction 1 for forward, -1 for inverse
void SignalProcessing::FFT(double *real, double *imag, int size, int direction)
{
    if (size < 2 || real == nullptr || imag == nullptr)
        return;
    
    // Bit-reversal permutation
    int j = 0;
    for (int i = 0; i < size - 1; ++i)
    {
        if (i < j)
        {
            double temp = real[i];
            real[i] = real[j];
            real[j] = temp;
            
            temp = imag[i];
            imag[i] = imag[j];
            imag[j] = temp;
        }
        
        int k = size / 2;
        while (k <= j)
        {
            j -= k;
            k /= 2;
        }
        j += k;
    }
    
    // FFT computation
    for (int step = 2; step <= size; step *= 2)
    {
        double theta = direction * 2.0 * M_PI / step;
        double w_real = cos(theta);
        double w_imag = sin(theta);
        
        for (int start = 0; start < size; start += step)
        {
            double wr = 1.0;
            double wi = 0.0;
            
            int half_step = step / 2;
            for (int k = 0; k < half_step; ++k)
            {
                int idx1 = start + k;
                int idx2 = idx1 + half_step;
                
                double tr = wr * real[idx2] - wi * imag[idx2];
                double ti = wr * imag[idx2] + wi * real[idx2];
                
                real[idx2] = real[idx1] - tr;
                imag[idx2] = imag[idx1] - ti;
                real[idx1] += tr;
                imag[idx1] += ti;
                
                double wr_temp = wr * w_real - wi * w_imag;
                wi = wr * w_imag + wi * w_real;
                wr = wr_temp;
            }
        }
    }
    
    // Normalize for inverse FFT
    if (direction == -1)
    {
        for (int i = 0; i < size; ++i)
        {
            real[i] /= size;
            imag[i] /= size;
        }
    }
}

/// @brief Performs FFT analysis on a window of the signal
/// @param start_index Starting index
/// @param window_size Window size
/// @param sampling_rate Sampling rate in Hz
/// @param spectrum Output spectrum
/// @return true if successful
bool SignalProcessing::FFTAnalysis(int start_index, int window_size, double sampling_rate, FrequencySpectrum *spectrum)
{
    if (spectrum == nullptr || start_index < 0 || window_size < 2 || sampling_rate <= 0)
        return false;
    
    int end_index = start_index + window_size;
    if (end_index > this->index)
        return false;
    
    // Round to nearest power of 2
    int fft_size = NextPowerOfTwo(window_size);
    
    // Allocate temporary arrays
    double *real = (double *)malloc(fft_size * sizeof(double));
    double *imag = (double *)malloc(fft_size * sizeof(double));
    
    if (real == nullptr || imag == nullptr)
    {
        if (real) free(real);
        if (imag) free(imag);
        return false;
    }
    
    // Copy data and zero-pad if necessary
    for (int i = 0; i < fft_size; ++i)
    {
        if (i < window_size)
        {
            real[i] = this->SignalVector[start_index + i];
        }
        else
        {
            real[i] = 0.0;
        }
        imag[i] = 0.0;
    }
    
    // Apply Hann window to reduce spectral leakage
    ApplyWindow(real, window_size, 1);
    
    // Perform FFT
    FFT(real, imag, fft_size, 1);
    
    // Calculate magnitudes and phases (only first half due to symmetry)
    int num_bins = fft_size / 2 + 1;
    spectrum->bins = (FrequencyBin *)malloc(num_bins * sizeof(FrequencyBin));
    
    if (spectrum->bins == nullptr)
    {
        free(real);
        free(imag);
        return false;
    }
    
    spectrum->num_bins = num_bins;
    spectrum->sampling_rate = sampling_rate;
    spectrum->frequency_resolution = sampling_rate / fft_size;
    spectrum->window_size = window_size;
    spectrum->total_power = 0.0;
    spectrum->dominant_frequency = 0.0;
    
    double max_magnitude = 0.0;
    
    for (int i = 0; i < num_bins; ++i)
    {
        spectrum->bins[i].frequency = i * spectrum->frequency_resolution;
        spectrum->bins[i].magnitude = sqrt(real[i] * real[i] + imag[i] * imag[i]);
        spectrum->bins[i].phase = atan2(imag[i], real[i]);
        spectrum->bins[i].power = spectrum->bins[i].magnitude * spectrum->bins[i].magnitude;
        
        spectrum->total_power += spectrum->bins[i].power;
        
        if (i > 0 && spectrum->bins[i].magnitude > max_magnitude)
        {
            max_magnitude = spectrum->bins[i].magnitude;
            spectrum->dominant_frequency = spectrum->bins[i].frequency;
        }
    }
    
    free(real);
    free(imag);
    
    return true;
}

/// @brief Performs FFT analysis on entire signal
/// @param sampling_rate Sampling rate in Hz
/// @param spectrum Output spectrum
/// @return true if successful
bool SignalProcessing::FFTAnalysis(double sampling_rate, FrequencySpectrum *spectrum)
{
    return FFTAnalysis(0, this->index, sampling_rate, spectrum);
}

/// @brief Finds peaks in frequency spectrum
/// @param spectrum Frequency spectrum
/// @param min_magnitude Minimum magnitude threshold
/// @param peak_frequencies Output array for frequencies
/// @param peak_magnitudes Output array for magnitudes
/// @param max_peaks Maximum number of peaks
/// @return Number of peaks found
int SignalProcessing::FindFrequencyPeaks(FrequencySpectrum *spectrum, double min_magnitude,
                                        double *peak_frequencies, double *peak_magnitudes, int max_peaks)
{
    if (spectrum == nullptr || peak_frequencies == nullptr || peak_magnitudes == nullptr || max_peaks < 1)
        return 0;
    
    int peak_count = 0;
    
    // Find local maxima
    for (int i = 1; i < spectrum->num_bins - 1 && peak_count < max_peaks; ++i)
    {
        if (spectrum->bins[i].magnitude > spectrum->bins[i - 1].magnitude &&
            spectrum->bins[i].magnitude > spectrum->bins[i + 1].magnitude &&
            spectrum->bins[i].magnitude >= min_magnitude)
        {
            peak_frequencies[peak_count] = spectrum->bins[i].frequency;
            peak_magnitudes[peak_count] = spectrum->bins[i].magnitude;
            peak_count++;
        }
    }
    
    // Sort peaks by magnitude (descending) using bubble sort
    for (int i = 0; i < peak_count - 1; ++i)
    {
        for (int j = 0; j < peak_count - i - 1; ++j)
        {
            if (peak_magnitudes[j] < peak_magnitudes[j + 1])
            {
                // Swap magnitudes
                double temp_mag = peak_magnitudes[j];
                peak_magnitudes[j] = peak_magnitudes[j + 1];
                peak_magnitudes[j + 1] = temp_mag;
                
                // Swap frequencies
                double temp_freq = peak_frequencies[j];
                peak_frequencies[j] = peak_frequencies[j + 1];
                peak_frequencies[j + 1] = temp_freq;
            }
        }
    }
    
    return peak_count;
}

/// @brief Calculates power in a frequency band
/// @param spectrum Frequency spectrum
/// @param freq_low Lower frequency bound
/// @param freq_high Upper frequency bound
/// @return Power in band
double SignalProcessing::GetPowerInBand(FrequencySpectrum *spectrum, double freq_low, double freq_high)
{
    if (spectrum == nullptr || freq_low < 0 || freq_high <= freq_low)
        return 0.0;
    
    double power = 0.0;
    
    for (int i = 0; i < spectrum->num_bins; ++i)
    {
        if (spectrum->bins[i].frequency >= freq_low && spectrum->bins[i].frequency <= freq_high)
        {
            power += spectrum->bins[i].power;
        }
    }
    
    return power;
}

/// @brief Analyzes harmonics of a fundamental frequency
/// @param spectrum Frequency spectrum
/// @param fundamental Fundamental frequency
/// @param num_harmonics Number of harmonics to analyze
/// @param harmonic_magnitudes Output array for harmonic magnitudes
/// @return Total harmonic distortion (THD)
double SignalProcessing::AnalyzeHarmonics(FrequencySpectrum *spectrum, double fundamental,
                                         int num_harmonics, double *harmonic_magnitudes)
{
    if (spectrum == nullptr || fundamental <= 0 || num_harmonics < 1 || harmonic_magnitudes == nullptr)
        return 0.0;
    
    double fundamental_magnitude = 0.0;
    double harmonics_sum = 0.0;
    
    // For each harmonic
    for (int h = 1; h <= num_harmonics; ++h)
    {
        double target_freq = h * fundamental;
        double tolerance = spectrum->frequency_resolution * 2.0;
        
        // Find closest bin to harmonic frequency
        double max_mag = 0.0;
        for (int i = 0; i < spectrum->num_bins; ++i)
        {
            double freq_diff = spectrum->bins[i].frequency - target_freq;
            if (freq_diff < 0) freq_diff = -freq_diff;
            
            if (freq_diff < tolerance && spectrum->bins[i].magnitude > max_mag)
            {
                max_mag = spectrum->bins[i].magnitude;
            }
        }
        
        harmonic_magnitudes[h - 1] = max_mag;
        
        if (h == 1)
        {
            fundamental_magnitude = max_mag;
        }
        else
        {
            harmonics_sum += max_mag * max_mag;
        }
    }
    
    // Calculate THD (Total Harmonic Distortion)
    if (fundamental_magnitude > 0)
    {
        return sqrt(harmonics_sum) / fundamental_magnitude;
    }
    
    return 0.0;
}

/// @brief Compares frequency spectra of different segments
/// @param marker_indices Segment start indices
/// @param num_markers Number of segments
/// @param sampling_rate Sampling rate
/// @param spectra Output array of spectra
/// @return Number of spectra computed
int SignalProcessing::CompareSegmentSpectra(int *marker_indices, int num_markers,
                                           double sampling_rate, FrequencySpectrum *spectra)
{
    if (marker_indices == nullptr || spectra == nullptr || num_markers < 1)
        return 0;
    
    int count = 0;
    
    for (int seg = 0; seg < num_markers; ++seg)
    {
        int start = marker_indices[seg];
        int end = (seg < num_markers - 1) ? marker_indices[seg + 1] : this->index;
        int size = end - start;
        
        if (start >= 0 && end <= this->index && size > 4)
        {
            if (FFTAnalysis(start, size, sampling_rate, &spectra[count]))
            {
                count++;
            }
        }
    }
    
    return count;
}

/// @brief Frees memory allocated for spectrum
/// @param spectrum Spectrum to free
void SignalProcessing::FreeSpectrum(FrequencySpectrum *spectrum)
{
    if (spectrum != nullptr && spectrum->bins != nullptr)
    {
        free(spectrum->bins);
        spectrum->bins = nullptr;
        spectrum->num_bins = 0;
    }
}

/// @brief Detects frequency anomalies by comparing with baseline
/// @param current_spectrum Current spectrum
/// @param baseline_spectrum Baseline spectrum
/// @param threshold Threshold ratio
/// @return Anomaly score
double SignalProcessing::DetectFrequencyAnomalies(FrequencySpectrum *current_spectrum,
                                                 FrequencySpectrum *baseline_spectrum,
                                                 double threshold)
{
    if (current_spectrum == nullptr || baseline_spectrum == nullptr)
        return 0.0;
    
    int min_bins = (current_spectrum->num_bins < baseline_spectrum->num_bins) ? 
                    current_spectrum->num_bins : baseline_spectrum->num_bins;
    
    double anomaly_score = 0.0;
    int anomaly_count = 0;
    
    for (int i = 1; i < min_bins; ++i)  // Skip DC component
    {
        double baseline_mag = baseline_spectrum->bins[i].magnitude;
        double current_mag = current_spectrum->bins[i].magnitude;
        
        if (baseline_mag > 0)
        {
            double ratio = current_mag / baseline_mag;
            
            if (ratio > threshold || ratio < (1.0 / threshold))
            {
                double deviation = (ratio > 1.0) ? (ratio - 1.0) : (1.0 - ratio);
                anomaly_score += deviation;
                anomaly_count++;
            }
        }
    }
    
    // Normalize by number of bins
    if (anomaly_count > 0)
    {
        anomaly_score /= anomaly_count;
    }
    
    return anomaly_score;
}

// ========== ML/AI FEATURE EXTRACTION IMPLEMENTATION ==========

/// @brief Extracts comprehensive ML feature vector from current signal
/// @param sampling_rate Sampling rate in Hz
/// @param features Output structure for features
/// @return true if successful
bool SignalProcessing::ExtractMLFeatures(double sampling_rate, MLFeatureVector *features)
{
    if (features == nullptr || this->index < 10)
    {
        return false;
    }
    
    int n = this->index;
    
    // === STATISTICAL FEATURES ===
    features->mean = this->GetMean();
    features->variance = this->GetVariance();
    features->std_dev = this->GetStandardDeviation();
    
    // RMS (Root Mean Square)
    double sum_squares = 0.0;
    for (int i = 0; i < n; i++)
    {
        sum_squares += this->SignalVector[i] * this->SignalVector[i];
    }
    features->rms = sqrt(sum_squares / n);
    
    // Peak-to-peak and crest factor
    double min_val = this->SignalVector[0];
    double max_val = this->SignalVector[0];
    for (int i = 1; i < n; i++)
    {
        if (this->SignalVector[i] < min_val) min_val = this->SignalVector[i];
        if (this->SignalVector[i] > max_val) max_val = this->SignalVector[i];
    }
    features->peak_to_peak = max_val - min_val;
    features->crest_factor = (features->rms > 0) ? (max_val / features->rms) : 0.0;
    
    // Skewness (third moment)
    double sum_cubed = 0.0;
    for (int i = 0; i < n; i++)
    {
        double deviation = this->SignalVector[i] - features->mean;
        sum_cubed += deviation * deviation * deviation;
    }
    features->skewness = (features->std_dev > 0) ? 
                         (sum_cubed / n) / (features->std_dev * features->std_dev * features->std_dev) : 0.0;
    
    // Kurtosis (fourth moment)
    double sum_fourth = 0.0;
    for (int i = 0; i < n; i++)
    {
        double deviation = this->SignalVector[i] - features->mean;
        double dev_squared = deviation * deviation;
        sum_fourth += dev_squared * dev_squared;
    }
    features->kurtosis = (features->variance > 0) ? 
                         (sum_fourth / n) / (features->variance * features->variance) : 0.0;
    
    // === TIME DOMAIN FEATURES ===
    
    // Zero crossing rate
    int zero_crossings = 0;
    for (int i = 1; i < n; i++)
    {
        if ((this->SignalVector[i-1] >= 0 && this->SignalVector[i] < 0) ||
            (this->SignalVector[i-1] < 0 && this->SignalVector[i] >= 0))
        {
            zero_crossings++;
        }
    }
    features->zero_crossing_rate = (double)zero_crossings / n;
    
    // Mean crossing rate
    int mean_crossings = 0;
    for (int i = 1; i < n; i++)
    {
        if ((this->SignalVector[i-1] >= features->mean && this->SignalVector[i] < features->mean) ||
            (this->SignalVector[i-1] < features->mean && this->SignalVector[i] >= features->mean))
        {
            mean_crossings++;
        }
    }
    features->mean_crossing_rate = (double)mean_crossings / n;
    
    // Energy
    features->energy = sum_squares;
    
    // Autocorrelation peak (first non-zero lag peak)
    features->autocorr_peak = 0.0;
    int max_lag = (n > 100) ? 100 : n / 2;
    double max_autocorr = 0.0;
    for (int lag = 1; lag < max_lag; lag++)
    {
        double autocorr = 0.0;
        for (int i = 0; i < n - lag; i++)
        {
            autocorr += this->SignalVector[i] * this->SignalVector[i + lag];
        }
        autocorr /= (n - lag);
        if (autocorr > max_autocorr)
        {
            max_autocorr = autocorr;
            features->autocorr_peak = (double)lag / sampling_rate;  // in seconds
        }
    }
    
    // === FREQUENCY DOMAIN FEATURES ===
    
    FrequencySpectrum spectrum;
    bool fft_success = this->FFTAnalysis(sampling_rate, &spectrum);
    
    if (fft_success)
    {
        features->dominant_frequency = spectrum.dominant_frequency;
        features->total_power = spectrum.total_power;
        
        // Power in frequency bands
        features->power_low_freq = this->GetPowerInBand(&spectrum, 0.0, 10.0);
        features->power_mid_freq = this->GetPowerInBand(&spectrum, 10.0, 100.0);
        features->power_high_freq = this->GetPowerInBand(&spectrum, 100.0, sampling_rate / 2.0);
        
        // Spectral centroid (center of mass of spectrum)
        double weighted_sum = 0.0;
        double total_magnitude = 0.0;
        for (int i = 0; i < spectrum.num_bins; i++)
        {
            weighted_sum += spectrum.bins[i].frequency * spectrum.bins[i].magnitude;
            total_magnitude += spectrum.bins[i].magnitude;
        }
        features->spectral_centroid = (total_magnitude > 0) ? (weighted_sum / total_magnitude) : 0.0;
        
        // Spectral spread (standard deviation around centroid)
        double spread_sum = 0.0;
        for (int i = 0; i < spectrum.num_bins; i++)
        {
            double diff = spectrum.bins[i].frequency - features->spectral_centroid;
            spread_sum += diff * diff * spectrum.bins[i].magnitude;
        }
        features->spectral_spread = (total_magnitude > 0) ? 
                                    sqrt(spread_sum / total_magnitude) : 0.0;
        
        // Spectral entropy (measure of spectral complexity)
        features->spectral_entropy = 0.0;
        for (int i = 0; i < spectrum.num_bins; i++)
        {
            if (spectrum.bins[i].magnitude > 0)
            {
                double normalized = spectrum.bins[i].magnitude / total_magnitude;
                features->spectral_entropy -= normalized * log(normalized + 1e-10);
            }
        }
        
        this->FreeSpectrum(&spectrum);
    }
    else
    {
        // FFT failed, set frequency features to 0
        features->dominant_frequency = 0.0;
        features->total_power = 0.0;
        features->power_low_freq = 0.0;
        features->power_mid_freq = 0.0;
        features->power_high_freq = 0.0;
        features->spectral_centroid = 0.0;
        features->spectral_spread = 0.0;
        features->spectral_entropy = 0.0;
    }
    
    features->num_features = 21;  // Total count
    return true;
}

/// @brief Extracts features from a specific segment
/// @param start_index Starting index
/// @param window_size Size of segment
/// @param sampling_rate Sampling rate in Hz
/// @param features Output structure
/// @return true if successful
bool SignalProcessing::ExtractMLFeaturesFromSegment(int start_index, int window_size,
                                                    double sampling_rate, MLFeatureVector *features)
{
    if (features == nullptr || start_index < 0 || window_size <= 0)
    {
        return false;
    }
    
    if (start_index + window_size > this->index)
    {
        return false;  // Segment out of bounds
    }
    
    // Create temporary SignalProcessing object with segment data
    SignalProcessing temp;
    for (int i = 0; i < window_size; i++)
    {
        temp.AddValue(this->SignalVector[start_index + i]);
    }
    
    return temp.ExtractMLFeatures(sampling_rate, features);
}

/// @brief Exports features to flat array for ML libraries
/// @param features Feature structure
/// @param output_array Output array (size >= 21)
/// @return Number of features exported
int SignalProcessing::ExportFeaturesToArray(MLFeatureVector *features, double *output_array)
{
    if (features == nullptr || output_array == nullptr)
    {
        return 0;
    }
    
    int idx = 0;
    
    // Statistical features (8)
    output_array[idx++] = features->mean;
    output_array[idx++] = features->std_dev;
    output_array[idx++] = features->variance;
    output_array[idx++] = features->skewness;
    output_array[idx++] = features->kurtosis;
    output_array[idx++] = features->rms;
    output_array[idx++] = features->peak_to_peak;
    output_array[idx++] = features->crest_factor;
    
    // Frequency features (9)
    output_array[idx++] = features->dominant_frequency;
    output_array[idx++] = features->spectral_centroid;
    output_array[idx++] = features->spectral_spread;
    output_array[idx++] = features->spectral_entropy;
    output_array[idx++] = features->total_power;
    output_array[idx++] = features->power_low_freq;
    output_array[idx++] = features->power_mid_freq;
    output_array[idx++] = features->power_high_freq;
    
    // Time domain features (4)
    output_array[idx++] = features->zero_crossing_rate;
    output_array[idx++] = features->mean_crossing_rate;
    output_array[idx++] = features->energy;
    output_array[idx++] = features->autocorr_peak;
    
    return idx;  // Should be 21
}

/// @brief Normalizes features using z-score normalization
/// @param features Feature vector to normalize
/// @param mean_values Mean values from training set
/// @param std_values Std dev values from training set
void SignalProcessing::NormalizeMLFeatures(MLFeatureVector *features, 
                                          double *mean_values, double *std_values)
{
    if (features == nullptr || mean_values == nullptr || std_values == nullptr)
    {
        return;
    }
    
    double temp_array[21];
    this->ExportFeaturesToArray(features, temp_array);
    
    // Apply z-score normalization: (x - mean) / std
    for (int i = 0; i < 21; i++)
    {
        if (std_values[i] > 0)
        {
            temp_array[i] = (temp_array[i] - mean_values[i]) / std_values[i];
        }
    }
    
    // Put normalized values back into structure
    int idx = 0;
    features->mean = temp_array[idx++];
    features->std_dev = temp_array[idx++];
    features->variance = temp_array[idx++];
    features->skewness = temp_array[idx++];
    features->kurtosis = temp_array[idx++];
    features->rms = temp_array[idx++];
    features->peak_to_peak = temp_array[idx++];
    features->crest_factor = temp_array[idx++];
    
    features->dominant_frequency = temp_array[idx++];
    features->spectral_centroid = temp_array[idx++];
    features->spectral_spread = temp_array[idx++];
    features->spectral_entropy = temp_array[idx++];
    features->total_power = temp_array[idx++];
    features->power_low_freq = temp_array[idx++];
    features->power_mid_freq = temp_array[idx++];
    features->power_high_freq = temp_array[idx++];
    
    features->zero_crossing_rate = temp_array[idx++];
    features->mean_crossing_rate = temp_array[idx++];
    features->energy = temp_array[idx++];
    features->autocorr_peak = temp_array[idx++];
}

// ========== DECIMATION AND INTERPOLATION IMPLEMENTATION ==========

/// @brief Decimates signal by factor (downsampling)
/// @param factor Decimation factor
/// @param out_vector Output array
/// @param apply_antialiasing Apply low-pass filter before decimation
/// @return Number of output samples
int SignalProcessing::Decimate(int factor, double *out_vector, bool apply_antialiasing)
{
    if (out_vector == nullptr || factor < 1 || this->index < factor)
    {
        return 0;
    }
    
    double *temp_signal = this->SignalVector;
    double filtered[NB_MAX_VALUES];
    
    // Apply anti-aliasing filter if requested
    if (apply_antialiasing && factor > 1)
    {
        // Use moving average as low-pass filter
        // Window size = factor to prevent aliasing
        int window = factor;
        for (int i = 0; i < this->index; i++)
        {
            double sum = 0.0;
            int count = 0;
            int start = (i - window/2 < 0) ? 0 : i - window/2;
            int end = (i + window/2 >= this->index) ? this->index - 1 : i + window/2;
            
            for (int j = start; j <= end; j++)
            {
                sum += this->SignalVector[j];
                count++;
            }
            filtered[i] = sum / count;
        }
        temp_signal = filtered;
    }
    
    // Decimate by keeping every Nth sample
    int out_index = 0;
    for (int i = 0; i < this->index; i += factor)
    {
        out_vector[out_index++] = temp_signal[i];
    }
    
    return out_index;
}

/// @brief Interpolates signal using linear interpolation
/// @param factor Interpolation factor
/// @param out_vector Output array
/// @return Number of output samples
int SignalProcessing::InterpolateLinear(int factor, double *out_vector)
{
    if (out_vector == nullptr || factor < 1 || this->index < 2)
    {
        return 0;
    }
    
    if (factor == 1)
    {
        // No interpolation needed
        for (int i = 0; i < this->index; i++)
        {
            out_vector[i] = this->SignalVector[i];
        }
        return this->index;
    }
    
    int out_index = 0;
    
    // Interpolate between each pair of samples
    for (int i = 0; i < this->index - 1; i++)
    {
        double y0 = this->SignalVector[i];
        double y1 = this->SignalVector[i + 1];
        
        // Insert original sample
        out_vector[out_index++] = y0;
        
        // Insert interpolated samples
        for (int j = 1; j < factor; j++)
        {
            double t = (double)j / factor;  // 0 < t < 1
            out_vector[out_index++] = y0 + t * (y1 - y0);  // Linear interpolation
        }
    }
    
    // Add last sample
    out_vector[out_index++] = this->SignalVector[this->index - 1];
    
    return out_index;
}

/// @brief Resamples signal to new sampling rate
/// @param current_rate Current sampling rate in Hz
/// @param target_rate Target sampling rate in Hz
/// @param out_vector Output array
/// @return Number of output samples
int SignalProcessing::Resample(double current_rate, double target_rate, double *out_vector)
{
    if (out_vector == nullptr || current_rate <= 0 || target_rate <= 0)
    {
        return 0;
    }
    
    double ratio = target_rate / current_rate;
    
    if (ratio == 1.0)
    {
        // No resampling needed
        for (int i = 0; i < this->index; i++)
        {
            out_vector[i] = this->SignalVector[i];
        }
        return this->index;
    }
    else if (ratio < 1.0)
    {
        // Downsampling - use decimation
        int factor = (int)(1.0 / ratio + 0.5);  // Round to nearest integer
        return this->Decimate(factor, out_vector, true);
    }
    else
    {
        // Upsampling - use interpolation
        int factor = (int)(ratio + 0.5);  // Round to nearest integer
        return this->InterpolateLinear(factor, out_vector);
    }
}

// ========== CORRELATION ANALYSIS IMPLEMENTATION ==========

/// @brief Computes autocorrelation of signal
/// @param max_lag Maximum lag to compute
/// @param out_correlation Output correlation array
/// @param normalize Normalize to [-1, 1]
/// @return Number of correlation values
int SignalProcessing::Autocorrelation(int max_lag, double *out_correlation, bool normalize)
{
    if (out_correlation == nullptr || max_lag < 0 || this->index < 2)
    {
        return 0;
    }
    
    // Limit max_lag to signal length
    if (max_lag >= this->index)
    {
        max_lag = this->index - 1;
    }
    
    double mean = this->GetMean();
    int n = this->index;
    
    // Compute autocorrelation for each lag
    for (int lag = 0; lag <= max_lag; lag++)
    {
        double sum = 0.0;
        int count = n - lag;
        
        for (int i = 0; i < count; i++)
        {
            sum += (this->SignalVector[i] - mean) * 
                   (this->SignalVector[i + lag] - mean);
        }
        
        out_correlation[lag] = sum / count;
    }
    
    // Normalize if requested
    if (normalize && out_correlation[0] != 0.0)
    {
        double r0 = out_correlation[0];  // Variance
        for (int lag = 0; lag <= max_lag; lag++)
        {
            out_correlation[lag] /= r0;
        }
    }
    
    return max_lag + 1;
}

/// @brief Computes cross-correlation between two signals
/// @param signal2 Second signal
/// @param signal2_size Size of second signal
/// @param max_lag Maximum lag (positive and negative)
/// @param out_correlation Output correlation array
/// @param normalize Normalize to [-1, 1]
/// @return Number of correlation values
int SignalProcessing::CrossCorrelation(double *signal2, int signal2_size, int max_lag,
                                       double *out_correlation, bool normalize)
{
    if (out_correlation == nullptr || signal2 == nullptr || 
        max_lag < 0 || this->index < 1 || signal2_size < 1)
    {
        return 0;
    }
    
    double mean1 = this->GetMean();
    
    // Calculate mean of signal2
    double sum2 = 0.0;
    for (int i = 0; i < signal2_size; i++)
    {
        sum2 += signal2[i];
    }
    double mean2 = sum2 / signal2_size;
    
    int out_index = 0;
    
    // Compute cross-correlation for negative lags
    for (int lag = -max_lag; lag <= max_lag; lag++)
    {
        double sum = 0.0;
        int count = 0;
        
        for (int i = 0; i < this->index; i++)
        {
            int j = i + lag;
            if (j >= 0 && j < signal2_size)
            {
                sum += (this->SignalVector[i] - mean1) * (signal2[j] - mean2);
                count++;
            }
        }
        
        if (count > 0)
        {
            out_correlation[out_index++] = sum / count;
        }
        else
        {
            out_correlation[out_index++] = 0.0;
        }
    }
    
    // Normalize if requested
    if (normalize)
    {
        // Compute variances
        double var1 = 0.0;
        for (int i = 0; i < this->index; i++)
        {
            double diff = this->SignalVector[i] - mean1;
            var1 += diff * diff;
        }
        var1 /= this->index;
        
        double var2 = 0.0;
        for (int i = 0; i < signal2_size; i++)
        {
            double diff = signal2[i] - mean2;
            var2 += diff * diff;
        }
        var2 /= signal2_size;
        
        double normalization = sqrt(var1 * var2);
        if (normalization > 0.0)
        {
            for (int i = 0; i < out_index; i++)
            {
                out_correlation[i] /= normalization;
            }
        }
    }
    
    return out_index;
}

/// @brief Finds lag with maximum correlation
/// @param correlation Correlation array
/// @param size Array size
/// @param peak_value Output peak value
/// @return Index of peak
int SignalProcessing::FindCorrelationPeak(double *correlation, int size, double *peak_value)
{
    if (correlation == nullptr || size < 1)
    {
        if (peak_value != nullptr) *peak_value = 0.0;
        return -1;
    }
    
    int peak_index = 0;
    double max_value = correlation[0];
    
    // Skip lag=0 for autocorrelation (always 1.0 when normalized)
    int start = (size > 1) ? 1 : 0;
    
    for (int i = start; i < size; i++)
    {
        if (correlation[i] > max_value)
        {
            max_value = correlation[i];
            peak_index = i;
        }
    }
    
    if (peak_value != nullptr)
    {
        *peak_value = max_value;
    }
    
    return peak_index;
}

