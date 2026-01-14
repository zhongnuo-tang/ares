#ifndef FS_RUNNER_H
#define FS_RUNNER_H

#ifdef __cplusplus
extern "C" {
#endif

#define MNT0_PATH "/mnt0/test.txt"
#define MNT_PATH "/mnt/test.txt"

/*============================================================
 *  Public Function Prototypes
 *============================================================*/

/**
 * @brief Filesystem read/write task handler
 * This function is intended to be called as an RTOS task
 * to perform repeated filesystem read/write operations.
 */
int fs_task_main(int argc, char *argv[]);

#ifdef __cplusplus
}
#endif

#endif /* FS_RUNNER_H */
