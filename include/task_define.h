#include <tinyara/config.h>

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <errno.h>

typedef int (*task_fn_t)(void);

typedef struct {
    const char *name;   /* task name */
    int         priority;
    int         stack_size;
    task_fn_t   run;    /* function to execute */
} task_t;
