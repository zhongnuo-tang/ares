#include <tinyara/config.h>
#include <stdio.h>

#include "lcd_runner.h"

void set_lcd_power(uint8_t onoff)
{
    char *argv[] = {
        "lcd_test"    ,
        "power"   ,
        onoff ? "100" : "0"
    };

    int argc = 3;

    lcd_test_main(argc, argv);

}

void start_lcd_stress_test(void)
{
    char *argv[] = {
        "lcd_test"    ,
        "stress_test" ,
        "start"       ,
        "1"           ,
    };

    int argc = 4;

    lcd_test_main(argc, argv);
}

void stop_lcd_stress_test(void)
{
    char *argv[] = {
        "lcd_test"    ,
        "stress_test" ,
        "stop"
    };

    int argc = 3;

    lcd_test_main(argc, argv);
}

int task_display_lcd(int argc, char *argv[])
{   
    while (1)
    {
        start_lcd_stress_test();
        sleep(10);
        stop_lcd_stress_test();
        set_lcd_power(0);
        sleep(10);
        set_lcd_power(1);

    }
    
}