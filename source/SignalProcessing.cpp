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

