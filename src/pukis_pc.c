#include <stdio.h>
#include <SDL.h>
#include <stdint.h>   /* Standard types */
#include <string.h>   /* String function definitions */
#include <unistd.h>   /* UNIX standard function definitions */
#include <fcntl.h>    /* File control definitions */
#include <errno.h>    /* Error number definitions */
#include <termios.h>  /* POSIX terminal control definitions */
#include <sys/ioctl.h>
#include <getopt.h>


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_DEPTH 32

void putpixel( SDL_Surface *surface, int x, int y, Uint32 pixel )
{
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;
    
    //Set the pixel
    pixels[ ( y * surface->w ) + x ] = pixel;
}

int main(int argc, char *argv[]) {
    SDL_Surface *screen;
    SDL_Event event;
    Uint32  *p;
    int         x = SCREEN_HEIGHT / 2; //x coordinate of our pixel
    int         y = SCREEN_WIDTH / 2; //y coordinate of our pixel

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

    //SDL_ShowCursor(0);
    //SDL_WM_GrabInput(SDL_GRAB_ON);

    /* Initialize the screen / window */
    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_DEPTH, SDL_HWSURFACE);

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
                        x = event.jaxis.value / 20 + SCREEN_WIDTH / 2;
                        if(x < 1)
                            x = 1;
                        if(x > SCREEN_WIDTH - 2)
                            x = SCREEN_WIDTH - 2;

                    }
                    else if(event.jaxis.axis == 1) //y
                    {
                        y = event.jaxis.value / 20 + SCREEN_HEIGHT / 2;
                        if(y < 1)
                            y = 1;
                        if(y > SCREEN_HEIGHT - 2)
                            y = SCREEN_HEIGHT - 2;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    x = event.motion.x / 500 + SCREEN_WIDTH / 2;
                    y = event.motion.y / 500 + SCREEN_WIDTH / 2;
            }
        }
        // Clear screen
        SDL_FillRect(SDL_GetVideoSurface(), NULL, 0);
        /* Make p point to the place we want to draw the pixel */
        //int val;
        int xring[] = {-1, 0, 0, 1};
        int yring[] = {0, -1, 1, 0};
        for(int i=0;i<4;i++)
        {
            //p = (Uint32 *)screen->pixels + (y + yring[i]) * screen->pitch + (x + xring[i]) * screen->format->BytesPerPixel;
            Uint32 col =  0x88 + (0x88 << 8) + (0x88 << 16);
            putpixel(screen,x+xring[i], y+yring[i], col);
        }
        //p = (Uint32 *)screen->pixels + y * screen->pitch + x * screen->format->BytesPerPixel;
        
        Uint32 col =  0xff + (0xff << 8) + (0xff << 16);
        putpixel(screen,x, y, col);
        /* Draw the pixel! */
        //*p=0xff + (0xff << 8) + (0xff << 16);

        /* update the screen (aka double buffering) */
        SDL_Flip(screen);
    }
    // Close if opened
    if(SDL_JoystickOpened(0))
        SDL_JoystickClose(joy);
}
