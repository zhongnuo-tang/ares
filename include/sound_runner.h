#ifndef SOUND_RUNNER_H
#define SOUND_RUNNER_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================
 *  Public Function Prototypes
 *============================================================*/

/**
 * @brief Sound task handler
 * This function is inteneded to be called as an RTOS task
 * to play sound repeatedly.
 */
int task_play_sound(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif /* SOUND_RUNNER_H */
