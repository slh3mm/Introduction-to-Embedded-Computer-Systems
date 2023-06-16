#ifndef TASKSCHEDULER_H_
#define TASKSCHEDULER_H_

#include "msp.h"
#include "Defines.h"
#include "FSM.h"

typedef struct {
    void (*Task)(FSMType *FSM);     // Pointer to function task
    uint32_t TaskCycleCounter;      // Task is executed when (TaskCycleCounter == TaskExecutionPeriod)
    int32_t TaskExecutionPeriod;    // An integer multiple of the scheduler execution period
    FSMType *FSM;                   // State information
} TaskType;


// Function prototypes
void InitializeTaskSchedulerTimer(void);
void TaskSchedulerISR(void);

#endif /* TASKSCHEDULER_H_ */
