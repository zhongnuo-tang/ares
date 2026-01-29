#ifndef WIFI_RUNNER_H
#define WIFI_RUNNER_H

#ifdef __cplusplus
extern "C" {
#endif


/*============================================================
 *  Public Function Prototypes
 *============================================================*/

/**
 * @brief WiFi task handler
 * This function is intended to be called as an RTOS task.
 * Connects to a WiFi network and repeatedly performs GET requests.
 */
int wifi_runnable(int argc, char *argv[]);

void wait_for_wifi(void);

#ifdef __cplusplus
}
#endif

#endif /* WIFI_RUNNER_H */
