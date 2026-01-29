static void run_kernel_update( void )
{
    binary_update_same_version_test();
    return;
}


int ota_manager_runnable( int argc, char *argv[] )
{
    wait_for_wifi();
    
    run_kernel_update();
    return 0;
}
