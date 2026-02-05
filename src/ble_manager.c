#include <stdio.h>
#include "ble_rmc.h"

/* ******************************************************************************* */
/*                           Public Variable Defnitions                            */
/* ******************************************************************************* */

extern char sut_mac[];

/* ******************************************************************************* */
/*                           Private Function Defnitions                           */
/* ******************************************************************************* */

static void ble_rmc_init( void )
{
    char *argv[] = { "ble_rmc", "init", NULL };
    int argc = 2;

    ble_rmc_main( argc, argv );

    return;
}

static void ble_rmc_dut( void )
{
    char *argv[] = { "ble_rmc", "test", "DUT", sut_mac, NULL };
    int argc = 4;

    ble_rmc_main( argc, argv );

    return;
}

int ble_runnable( int argc, char *argv[] )
{
    ble_rmc_init();
    sleep( 5 );
    ble_rmc_dut();

    return 0;
}
