#include "SignalProcessing.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
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
}
/// @brief Clear signal processing vector
void SignalProcessing::ClearVector()
{
	for (int i = 0; i < NB_MAX_VALUES; i++)
	{
		// write the config items in file
		this->SignalVector[i] = 0.0;
  	}
	this->index = 0;
}
/// @brief Add a value to the signal processing vector
/// @param value Contain the values to be saved in the signal processing vector
/// @return this->index
int SignalProcessing::AddValue(double value)
{
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &(this->signal_timestamp[this->index]));
	this->SignalVector[this->index] = value;
	if ((this->index > NB_MAX_VALUES) && (this->index < 0))
	{
		this->index = 0/*  */;
	}	
	else
	{
		this->index++;
	}
	return (this->index);
}

/// @brief Add a value to the signal processing vector
/// @param td Contain the timestamp of the signal value at index
/// @param index the index of the signal value
/// @return void
void SignalProcessing::GetTimespec(struct timespec *td, int index)
{
	td->tv_nsec = signal_timestamp[index].tv_nsec;
	td->tv_sec = signal_timestamp[index].tv_sec;
}

/// @brief Get last value of the signal processing vector
/// @return void
double SignalProcessing::GetLastValue()
{
	return this->SignalVector[this->index-1];
}
/// @brief Set item of signal processing vector
/// @param Item is the item
/// @return void
void SignalProcessing::SetItem(int Item)
{
	this->item = Item;
}
/// @brief Get Max capacity of the signal processing vector
/// @return Max capacity
int SignalProcessing::GetMaxCapacity()
{
	return NB_MAX_VALUES;
}
/// @brief Get current index of the signal processing vector
/// @return Current index
int SignalProcessing::GetIndex()
{
	return (this->index);
}
/// @brief Get size of the signal processing vector
/// @return Current index
int SignalProcessing::GetSize()
{
	return (this->GetIndex());
}
/// @brief Get number for the signal processing vector
/// @return Item number
int SignalProcessing::GetItem()
{
	return (this->item);
}
/// @brief Get signal processing vector
/// @param signalProc_vector Contain signal processing vector 
/// @return void
void SignalProcessing::GetVector(double *signalProc_vector)
{
	for (int i = 0; i < this->index-1; i++)
	{
		signalProc_vector[i] = this->SignalVector[i];
	}
}
/// @brief Get signal processing vector
/// @param signalProc_vector Contain signal processing vector 
/// @return void
void SignalProcessing::GetVectorWithOffset(double *signalProc_vector,int size, int offset)
{
	for (int i = 0; i < size; i++)
	{
		signalProc_vector[i] = this->SignalVector[i+offset];
	}
}
/// @brief Init signal processing vector
/// @param values Contain signal processing vector which will be copied to SignalVector
/// @param size Contain size of the signal processing vector which will be copied to SignalVector
/// @return void
void SignalProcessing::InitVector(double *values,int size)
{
	for (int i = 0; i < size; ++i)
	{
		this->SignalVector[i] = values[i];
	}
	this->index = size+1;
}
/// @brief Multiply signal processing vector with a scalar value
/// @param value Contain value
/// @param size Number of element which will be multiply
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
/// @brief Substract signal processing vector with a scalar value
/// @param value Contain value
/// @param size Number of element which will be Substract
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
/// @brief Divide signal processing vector with a scalar value
/// @param value Contain value
/// @param size Number of element which will be Divided
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
/// @brief Add signal processing vector with a scalar value
/// @param value Contain value
/// @param size Number of element which will be add
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