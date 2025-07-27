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
class SignalProcessing
{
public:
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
    };

#endif // SIGNALPROCESSING_H
