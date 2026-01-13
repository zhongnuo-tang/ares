#include "task_define.h"
#include "sound_player.h"
#include "lcd_runner.h"

static const task_t task_table[] = {
    { "sound",  SCHED_PRIORITY_DEFAULT, 65536 , task_play_sound    },
    { "lcd"  ,  SCHED_PRIORITY_DEFAULT, 131072, task_display_lcd   },
};

#define TASK_COUNT (sizeof(task_table) / sizeof(task_table[0]))
void run_tasks(void);
