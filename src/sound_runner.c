#include <tinyara/config.h>
#include <stdio.h>

#include "sound_runner.h"

static void play_music(void)
{
    char *argv[] = {
        "soundplayer"               ,     // UNUSED
        "/res/kernel/audio/test.mp3",     // contents path
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
        play_music();
        sleep(1);
    }
}
