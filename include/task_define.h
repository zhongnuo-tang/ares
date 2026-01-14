#ifndef TASK_DEFINE_H
#define TASK_DEFINE_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================
 *  Type Definitions
 *============================================================*/

typedef int (*task_fn_t)(int argc, char *argv[]);

typedef struct {
    const char *name;   /* task name */
    int         priority;
    int         stack_size;
    task_fn_t   run;    /* function to execute */
    char * const      arg;    /* argument to the function */
} task_t;

#ifdef __cplusplus
}
#endif

#endif /* TASK_DEFINE_H */
