#ifndef SIGNALPROCESSING_H
#define SIGNALPROCESSING_H
#define NB_MAX_VALUES 10000 /* number of values in the signal vector  */
#define NS_PER_SECOND 1000000000
#include <time.h>

class SignalProcessing
{
public:
    // SignalProcessing constructor
    SignalProcessing();

    int AddValue(double value);
    void SetItem(int Item);
    void GetTimespec(struct timespec *td, int index);
    double GetLastValue();
    int GetMaxCapacity();
    int GetSize();
    int GetIndex();
    int GetItem();
    void GetVector(double *signalProc_vector);
    void InitVector(double *values, int size);
    void MultiplyWithValue(double value, int size);
    void DivideWithValue(double value, int size);
    void SubstractWithValue(double value, int size);
    void AddWithValue(double value, int size);
    void GetVectorWithOffset(double *signalProc_vector,int size, int offset);
    void ClearVector();
private:
    double SignalVector[NB_MAX_VALUES];
    timespec signal_timestamp[NB_MAX_VALUES];
    int item;
    int index;
    timespec timestamp;

};

#endif // SIGNALPROCESSING_H
