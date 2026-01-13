#include "task_manager.h"

void run_tasks(void)
{
    for (size_t i = 0; i < TASK_COUNT; i++) {
        int pid = task_create(task_table[i].name,
                              task_table[i].priority,
                              task_table[i].stack_size,
                              task_table[i].run,
                              NULL);
        if (pid < 0) 
        {
            return -1;
        }
        printf("Running (PID:%d) %s (prio=%d)\n",
               pid,
               task_table[i].name,
               task_table[i].priority);
    }
}
