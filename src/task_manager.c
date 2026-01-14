#include "task_manager.h"

#define TASK_COUNT (sizeof(task_table) / sizeof(task_table[0]))

static char *fs_mnt_argv[] = {
    MNT_PATH,
    NULL
};

static char *fs_mnt0_argv[] = {
    MNT0_PATH,
    NULL
};

static const task_t task_table[] = {
    { "sound"       , SCHED_PRIORITY_DEFAULT, 65536, task_play_sound , NULL                             },
    { "lcd"         , SCHED_PRIORITY_DEFAULT, 65536, task_display_lcd, NULL                             },
    { "wifi"        , SCHED_PRIORITY_DEFAULT, 16384, wifi_runnable   , NULL                             },
    { "fs_mnt"      , SCHED_PRIORITY_DEFAULT, 8192 , fs_task_main    , fs_mnt_argv                      },
    { "fs_mnt0"     , SCHED_PRIORITY_DEFAULT, 8192 , fs_task_main    , fs_mnt0_argv                     },
    { "uart_rx"     , SCHED_PRIORITY_DEFAULT, 8192 , uart_runnable   , NULL                     },
};

void run_tasks(void)
{
    for (size_t i = 0; i < TASK_COUNT; i++)
    {
        int pid = task_create(task_table[i].name,
                              task_table[i].priority,
                              task_table[i].stack_size,
                              task_table[i].run,
                              task_table[i].arg
                              );
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
