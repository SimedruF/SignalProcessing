#include "test.h"
#include "SignalProcessing.h"
#include <stdio.h>
#include <time.h>

#define BLADE_1 1
#define BLADE_2 2
#define BLADE_3 3
#define BLADE_4 4
#define BLADE_5 5
#define BLADE_6 6
#define BLADE_7 7
#define BLADE_8 8
#define BLADE_9 9
#define BLADE_10 10
#define BLADE_11 11
#define NB_MAX_VALUES_TEST 10000
timespec timestamp;

int main()
{
    double SigProcVector[NB_MAX_VALUES_TEST];
    double TestSigVector[10] = {
        2.3,
        5.6,
        78.5,
        22.3,
        45.6,
        78.5,
        72.3,
        85.6,
        378.5,
        42.3
    };
    int local_index;
    SignalProcessing Vector1[3];

    // set blade 1 and 2
    Vector1[BLADE_1].SetItem(BLADE_1);
    Vector1[BLADE_2].SetItem(BLADE_2);

    // add a value for blade 1
    printf(" Index:%d\n ", Vector1[BLADE_1].AddValue(2.4));
    //get the value for blade 1
    printf("  Value = %f\n", Vector1[BLADE_1].GetLastValue());

    // add a value for blade 2
    printf(" Index:%d \n", Vector1[BLADE_2].AddValue(3.5454));
    printf("  Value = %f\n", Vector1[BLADE_2].GetLastValue());

    // get timestamp for blade 1 
    Vector1[BLADE_1].GetTimespec(&timestamp, 0);
    printf(" Timstamp Blade 1: %d\n", timestamp.tv_sec);
    // get timestamp for blade 2
    Vector1[BLADE_2].GetTimespec(&timestamp, 0);
    printf(" Timstamp Blade 2: %d\n", timestamp.tv_sec);

    printf(" Index:%d, blade: %d\n", Vector1[BLADE_1].AddValue(6.5454), Vector1[BLADE_1].GetItem());
    printf(" Index:%d, blade: %d\n ", Vector1[BLADE_2].AddValue(7.5454), Vector1[BLADE_2].GetItem());
    printf(" Index:%d, blade: %d\n ", Vector1[BLADE_1].AddValue(26.3454), Vector1[BLADE_1].GetItem());
    printf(" Index:%d, blade: %d\n ", Vector1[BLADE_2].AddValue(27.4454), Vector1[BLADE_2].GetItem());
    printf(" Index:%d, blade: %d\n ", Vector1[BLADE_1].AddValue(56.5454), Vector1[BLADE_1].GetItem());
    printf(" Index:%d, blade: %d\n ", Vector1[BLADE_2].AddValue(77.5454), Vector1[BLADE_2].GetItem());
    printf(" Index:%d, blade: %d\n ", Vector1[BLADE_1].AddValue(76.3454), Vector1[BLADE_1].GetItem());
    printf(" Index:%d, blade: %d\n ", Vector1[BLADE_2].AddValue(87.4454), Vector1[BLADE_2].GetItem());

    printf("  Current index = %d, blade: %d\n", Vector1[BLADE_1].GetIndex(), Vector1[BLADE_1].GetItem());
    printf("  Current index = %d, blade: %d\n", Vector1[BLADE_2].GetIndex(), Vector1[BLADE_2].GetItem());

    // get the values from blade 1
    Vector1[BLADE_1].GetVector((double*) &SigProcVector[0]);
    local_index = Vector1[BLADE_1].GetIndex();
    printf("  Local index = %d, blade: %d\n", local_index, Vector1[BLADE_1].GetItem());
    for (int i=0; i < local_index;i++)
    {
        printf(" Vector[%d]: %f \n",i, SigProcVector[i]);
    }
    Vector1[BLADE_3].SetItem(BLADE_3);
    Vector1[BLADE_3].InitVector(TestSigVector, 10);
    Vector1[BLADE_3].GetVector((double *)&SigProcVector[0]);
    local_index = Vector1[BLADE_3].GetIndex();
    printf("  Local index = %d, blade: %d\n", local_index, Vector1[BLADE_3].GetItem());
    for (int i = 0; i < local_index; i++)
    {
        printf(" Vector[%d]: %f \n", i, SigProcVector[i]);
    }
    /// add value 3 to the vector
    Vector1[BLADE_3].AddWithValue(3.0, 10);
    Vector1[BLADE_3].GetVector((double *)&SigProcVector[0]);
    local_index = Vector1[BLADE_3].GetIndex();
    printf("  Local index = %d, blade: %d\n", local_index, Vector1[BLADE_3].GetItem());
    for (int i = 0; i < local_index; i++)
    {
        printf(" Vector[%d]: %f \n", i, SigProcVector[i]);
    }

    /// multiply with value 6
    Vector1[BLADE_3].MultiplyWithValue(6.0,10);
    Vector1[BLADE_3].GetVector((double *)&SigProcVector[0]);
    local_index = Vector1[BLADE_3].GetIndex();
    printf("  Local index = %d, blade: %d\n", local_index, Vector1[BLADE_3].GetItem());
    for (int i = 0; i < local_index; i++)
    {
        printf(" Vector[%d]: %f \n", i, SigProcVector[i]);
    }

    /// Divide with value 1.4
    Vector1[BLADE_3].DivideWithValue(1.4, 5);
    Vector1[BLADE_3].GetVector((double *)&SigProcVector[0]);
    local_index = Vector1[BLADE_3].GetIndex();
    printf("  Local index = %d, blade: %d\n", local_index, Vector1[BLADE_3].GetItem());
    for (int i = 0; i < local_index; i++)
    {
        printf(" Vector[%d]: %f \n", i, SigProcVector[i]);
    }

    /// Substract with value 200.0
    Vector1[BLADE_3].SubstractWithValue(200.0, 10);
    Vector1[BLADE_3].GetVector((double *)&SigProcVector[0]);
    local_index = Vector1[BLADE_3].GetIndex();
    printf("  Local index = %d, blade: %d\n", local_index, Vector1[BLADE_3].GetItem());
    for (int i = 0; i < local_index; i++)
    {
        printf(" Vector[%d]: %f \n", i, SigProcVector[i]);
    }

    /// Divide with value 200.0
    Vector1[BLADE_3].DivideWithValue(200.0, 20000);
    Vector1[BLADE_3].GetVector((double *)&SigProcVector[0]);
    local_index = Vector1[BLADE_3].GetIndex();
    printf(" Local index = %d, blade: %d\n", local_index, Vector1[BLADE_3].GetItem());
    for (int i = 0; i < 20/*local_index*/; i++)
    {
        printf(" Vector[%d]: %f \n", i, SigProcVector[i]);
    }

    return 0;
}