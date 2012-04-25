#include <stdio.h>
#include <SDL.h>

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define SCREEN_DEPTH 8

int main(int argc, char *argv[]) {
    SDL_Surface *screen;
    SDL_Event event;
    Uint8       *p;
    int         x = 10; //x coordinate of our pixel
    int         y = 20; //y coordinate of our pixel

    SDL_Joystick *joy;
    SDL_Init(SDL_INIT_JOYSTICK);
    // Check for joystick
    if(SDL_NumJoysticks()>0){
        // Open joystick
        joy=SDL_JoystickOpen(0);

        if(joy)
        {
            printf("Opened Joystick 0\n");
            printf("Name: %s\n", SDL_JoystickName(0));
            printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joy));
            printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy));
            printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy));
        }
        else
            printf("Couldn't open Joystick 0\n");

    }



    /* Initialize SDL */
    SDL_Init(SDL_INIT_VIDEO);

    /* Initialize the screen / window */
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH, SDL_SWSURFACE);

    int running = 1;
    while(running)
    {
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    printf("quitting!\n");
                    running = 0;
                    break;
                case SDL_JOYAXISMOTION:
                    if(event.jaxis.axis == 0) //X
                    {
                        x = event.jaxis.value / 500 + SCREEN_WIDTH / 2;
                    }
                    else if(event.jaxis.axis == 1) //y
                    {
                        y = event.jaxis.value / 500 + SCREEN_HEIGHT / 2;
                    }
                    break;
            }
        }
        /* Make p point to the place we want to draw the pixel */
        p = (Uint8 *)screen->pixels + y * screen->pitch + x * screen->format->BytesPerPixel;

        /* Draw the pixel! */
        *p=0xff;

        /* update the screen (aka double buffering) */
        SDL_Flip(screen);
    }
    // Close if opened
    if(SDL_JoystickOpened(0))
        SDL_JoystickClose(joy);
}
