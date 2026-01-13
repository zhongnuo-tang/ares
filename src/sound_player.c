#include "sound_player.h"

void task_play_sound(void)
{
    char *argv[] = {
        "soundplayer"               ,     // UNUSED
        "/res/kernel/audio/test.mp3",     // contents path
        "3"                         ,     // volume
        "48000"                     ,     // sample rate
        "1"                         ,     // stream policy
        "1"                               // looping
    };

    int argc = 6;
    
    soundplayer_main(argc, argv);
    while (1)
    {
        sleep(1);
    }
}
