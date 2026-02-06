#include "task_manager.h"
#include <stdio.h>
#include <tinyara/config.h>

#define MAX_SSID_LEN 32
#define MAX_PASSWORD_LEN 64
#define XIAOMI_SSID "xiaomi_test"
#define XIAOMI_PASSWORD "1234567890"
#define XIAOMI_AUTH "wpa2_aes"
#define SUT_DEFAULT_MAC "AA:BB:CC:DD:EE:FF"

/* ******************************************************************************* */
/*                           Public Variable Defnitions                            */
/* ******************************************************************************* */
char ssid[MAX_SSID_LEN] = {0};
char password[MAX_PASSWORD_LEN] = {0};
char sut_mac[18] = {0};

/* ******************************************************************************* */
/*                           Public Function Defnitions                            */
/* ******************************************************************************* */

#ifdef CONFIG_BUILD_KERNEL
int main( int argc, FAR char *argv[] )
#else
int ares_main( int argc, char *argv[] )
#endif
{
    if ( argc > 1)
    {
        strncpy(ssid, argv[1], MAX_SSID_LEN - 1);
        ssid[MAX_SSID_LEN - 1] = '\0';

        strncpy(password, argv[2], MAX_PASSWORD_LEN - 1);
        password[MAX_PASSWORD_LEN - 1] = '\0';

        strncpy( sut_mac, argv[3], 17 );
        sut_mac[17] = '\0';
    }
    else
    {
        strncpy(ssid, XIAOMI_SSID, MAX_SSID_LEN - 1);
        ssid[MAX_SSID_LEN - 1] = '\0';

        strncpy(password, XIAOMI_PASSWORD, MAX_PASSWORD_LEN - 1);
        password[MAX_PASSWORD_LEN - 1] = '\0';

        strncpy( sut_mac, SUT_DEFAULT_MAC, 17 );
        sut_mac[17] = '\0';
    }
    run_tasks( argc, argv );
    return 0;
}
