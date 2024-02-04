#ifndef SIGNALPROCESSING_H
#define SIGNALPROCESSING_H
#define NB_MAX_VALUES 100 /* number of values in the signal vector  */
#define NS_PER_SECOND 1000000000
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
    void GetVectorInt(int *signalProc_vector, int size);
    void InitVector(double *values, int size);
    void MultiplyWithValue(double value, int size);
    void DivideWithValue(double value, int size);
    void SubstractWithValue(double value, int size);
    void AddWithValue(double value, int size);
    void GetVectorWithOffset(double *signalProc_vector,int size, int offset);
    void ClearVector();
    void NormalDistributionFree();
    void NormalDistributionPrint(prob_dist *pd);
    void NormalDistributionCalculate(double *data, int size, prob_dist *pd);
    prob_dist *NormalDistributionCreate();
    void NormalDistributionRun();
    void PrintVector();

private:
    int IndexOf(double value, prob_dist *pd);
    /**
     * @brief Signal vector
     */
    double SignalVector[NB_MAX_VALUES];
    timespec signal_timestamp[NB_MAX_VALUES];
    int item;
    /**
     * @brief total number of values added in SignalVector
     */
    int index;
    timespec timestamp;
    prob_dist *p_d;
};

#endif // SIGNALPROCESSING_H
