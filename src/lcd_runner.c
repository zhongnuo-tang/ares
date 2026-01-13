#include "lcd_runner.h"

void task_display_lcd(void)
{    
    char *argv[] = {
        "lcd_test"    ,
        "stress_test" ,
        "start"       ,
        "2"           ,
    };

    int argc = 4;

    lcd_test_main(argc, argv);
    while (1)
    {
        sleep(1);
    }
    
}