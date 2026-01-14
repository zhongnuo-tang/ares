#include <tinyara/config.h>
#include <stdio.h>

#include "sound_runner.h"

uint8_t counter = 0;

static void play_music(char* path)
{
    char *argv[] = {
        "soundplayer"               ,     // UNUSED
        path                        ,     // contents path
        "3"                         ,     // volume
        "48000"                     ,     // sample rate
        "1"                         ,     // stream policy
        "0"                               // looping
    };

    int argc = 6;
    
    soundplayer_main(argc, argv);
}

int task_play_sound(int argc, char *argv[])
{

    while (1)
    {
        if ( counter % 2 == 0 )
        {
            play_music("/res/kernel/audio/sleep.mp3");
        }
        else
        {
            play_music("/res/kernel/audio/test.mp3");
        }
        counter++;
        sleep(1);
    }
}
