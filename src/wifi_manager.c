#include "http_client.h"
#include "wifi_runner.h"
#include <stdio.h>
#include <tinyara/config.h>

/* ******************************************************************************* */
/*                           Macro Defnitions                                      */
/* ******************************************************************************* */

#define XIAOMI_SSID "xiaomi_test"
#define XIAOMI_PASSWORD "1234567890"
#define XIAOMI_AUTH "wpa2_aes"

/* ******************************************************************************* */
/*                           Public Variable Declarations                          */
/* ******************************************************************************* */
uint8_t is_wifi_connected;

/* ******************************************************************************* */
/*                           Private Function Declarations                         */
/* ******************************************************************************* */

static void init_wifi( void );
static void connect_wifi( void );

/* ******************************************************************************* */
/*                           Private Function Defnitions                           */
/* ******************************************************************************* */

static void init_wifi( void )
{
    char *argv[] = { "wm_test", "start", NULL };
    int argc = 2;

    wm_test_main( argc, argv );

    return;
}

static void connect_wifi( void )
{
    char *argv[] = { "wm_test", "join", XIAOMI_SSID, XIAOMI_AUTH, XIAOMI_PASSWORD, NULL };
    int argc = 5;

    wm_test_main( argc, argv );

    return;
}

static void get_wifi_info( void )
{
    if ( !is_wifi_connected )
    {
        printf( "WiFi not connected yet, skip getting WiFi info\n" );
        return;
    }
    char *argv[] = { "wm_test", "info", NULL };
    int argc = 2;

    wm_test_main( argc, argv );

    return;
}

/* ******************************************************************************* */
/*                           Public Function Defnitions                            */
/* ******************************************************************************* */

int wifi_runnable( int argc, char *argv[] )
{
    init_wifi();
    sleep( 5 );
    connect_wifi();
    is_wifi_connected = 1;

    while ( 1 )
    {
        get_wifi_info();
        sleep( 5 );
    }
}
