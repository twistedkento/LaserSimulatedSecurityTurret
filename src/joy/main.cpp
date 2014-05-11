#include <iostream>
#include <stdio.h>
#include <SDL.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
    int i = 0;
    int found_stick = 0;
    SDL_Joystick *joystick;
    SDL_Event event;
    if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
        std::cout << SDL_GetError() << std::endl;
        exit(1); 
    }
    printf("%i joysticks were found.\n\n", SDL_NumJoysticks() );
    printf("The names of the joysticks are:\n");
                
    for(i=0; i < SDL_NumJoysticks(); i++ ) 
    {
        printf("    %s\n", SDL_JoystickNameForIndex(i));
        joystick = SDL_JoystickOpen(i);
        if (joystick == NULL) {
            fprintf(stderr, "SDL_JoystickOpen(%d) failed: %s\n", i,
                    SDL_GetError());
        } else {
            char guid[64];
            bool axis[SDL_JoystickNumAxes(joystick)];
            float procent;
            SDL_JoystickGetGUIDString(SDL_JoystickGetGUID(joystick),
                                      guid, sizeof (guid));
            printf(" axes: %d\n", SDL_JoystickNumAxes(joystick));
            printf(" balls: %d\n", SDL_JoystickNumBalls(joystick));
            printf(" hats: %d\n", SDL_JoystickNumHats(joystick));
            printf(" buttons: %d\n", SDL_JoystickNumButtons(joystick));
            printf("instance id: %d\n", SDL_JoystickInstanceID(joystick));
            printf(" guid: %s\n", guid);
            for(;;) {
                while (SDL_PollEvent(&event)) {
                    switch (event.type) {
                        case SDL_JOYAXISMOTION:
                            procent = (event.jaxis.value/32767.0);
                            printf("Joystick %d axis %d value: %f\n",
                                   event.jaxis.which,
                                   event.jaxis.axis, procent);
                            break;
                        case SDL_JOYHATMOTION:
                            printf("Joystick %d hat %d value:",
                                   event.jhat.which, event.jhat.hat);
                            if (event.jhat.value == SDL_HAT_CENTERED)
                                printf(" centered");
                            if (event.jhat.value & SDL_HAT_UP)
                                printf(" up");
                            if (event.jhat.value & SDL_HAT_RIGHT)
                                printf(" right");
                            if (event.jhat.value & SDL_HAT_DOWN)
                                printf(" down");
                            if (event.jhat.value & SDL_HAT_LEFT)
                                printf(" left");
                            printf("\n");
                            break;
                        case SDL_JOYBALLMOTION:
                            printf("Joystick %d ball %d delta: (%d,%d)\n",
                                   event.jball.which,
                                   event.jball.ball, event.jball.xrel, event.jball.yrel);
                            break;
                        case SDL_JOYBUTTONDOWN:
                            exit(1);
                            printf("Joystick %d button %d down\n",
                                   event.jbutton.which, event.jbutton.button);
                            break;
                        case SDL_JOYBUTTONUP:
                            printf("Joystick %d button %d up\n",
                                   event.jbutton.which, event.jbutton.button);
                            break;
                    }
                }
            struct timespec ts, ts2;
            ts.tv_sec = 0;
            ts.tv_nsec = 40000000L;
            nanosleep(&ts, &ts2);
            }
            SDL_JoystickClose(joystick);
        }
    }
    return 0;
}
