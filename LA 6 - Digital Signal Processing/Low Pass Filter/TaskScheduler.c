#include "TaskScheduler.h"

extern TaskType Tasks[NUMBER_OF_TASKS];

void InitializeTaskSchedulerTimer(void)
{
    SysTick->CTRL = 0;                              // 1) disable SysTick during setup
    SysTick->LOAD = TASK_SCHEDULER_PERIOD - 1;      // 2) reload value sets period
    SysTick->VAL = 0;                               // 3) any write to current clears it
    SCB->SHP[11] = (TASK_SCHEDULER_PRIORITY << 5);  // set priority into top 3 bits of 8-bit register
    SysTick->CTRL = (BIT2 | BIT1 | BIT0);           // 4) enable SysTick with core clock and interrupts
}

void TaskSchedulerISR(void)
{
    uint32_t i;

    for (i = 0; i < NUMBER_OF_TASKS; i++) {
        Tasks[i].TaskCycleCounter++;
        if (Tasks[i].TaskCycleCounter >= Tasks[i].TaskExecutionPeriod) {
            Tasks[i].TaskCycleCounter = 0;
            (*Tasks[i].Task)((FSMType *) Tasks[i].FSM);
        }
    }
}

void SysTick_Handler(void)
{
    TaskSchedulerISR();
}
