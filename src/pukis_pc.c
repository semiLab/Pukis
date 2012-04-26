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


#define BAUDRATE 9600
#define COMPORT "/dev/ttyUSB0"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_DEPTH 32


int serialport_init(const char* serialport, int baud);
int serialport_writebyte(int fd, uint8_t b);
int serialport_write(int fd, const char* str);
int serialport_read_until(int fd, char* buf, char until);



void putpixel( SDL_Surface *surface, int x, int y, Uint32 pixel )
{
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *)surface->pixels;

    //Set the pixel
    pixels[ ( y * surface->w ) + x ] = pixel;
}

int main(int argc, char *argv[]) {

    int sp = serialport_init(COMPORT, BAUDRATE);


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
                        x = event.jaxis.value / 100 + SCREEN_WIDTH / 2;
                        if(x < 1)
                            x = 1;
                        if(x > SCREEN_WIDTH - 2)
                            x = SCREEN_WIDTH - 2;
                        printf("x: %d\n", event.jaxis.value >> 8);
                        serialport_writebyte(sp,'x');
                        serialport_writebyte(sp,event.jaxis.value >> 8);
                    }
                    else if(event.jaxis.axis == 1) //y
                    {
                        y = event.jaxis.value / 100 + SCREEN_HEIGHT / 2;
                        if(y < 1)
                            y = 1;
                        if(y > SCREEN_HEIGHT - 2)
                            y = SCREEN_HEIGHT - 2;
                        printf("y: %d\n", event.jaxis.value >> 8);
                        serialport_writebyte(sp,'y');
                        serialport_writebyte(sp,event.jaxis.value >> 8);
                    }
                    break;
                case SDL_MOUSEMOTION:
                    x = event.motion.x / 500 + SCREEN_WIDTH / 2;
                    y = event.motion.y / 500 + SCREEN_WIDTH / 2;
            }
        }
        // Clear screen
        //        SDL_FillRect(SDL_GetVideoSurface(), NULL, 0);
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


int serialport_writebyte( int fd, uint8_t b)
{
    int n = write(fd,&b,1);
    if( n!=1)
        return -1;
    return 0;
}
int serialport_write(int fd, const char* str)
{
    int len = strlen(str);
    int n = write(fd, str, len);
    if( n!=len )
        return -1;
    return n;
}

int serialport_read_until(int fd, char* buf, char until)
{
    char b[1];
    int i=0;
    do {
        int n = read(fd, b, 1);  // read a char at a time
        if( n==-1) return -1;    // couldn't read
        if( n==0 ) {
            usleep( 10 * 1000 ); // wait 10 msec try again
            continue;
        }
        buf[i] = b[0]; i++;
    } while( b[0] != until );

    buf[i] = 0;  // null terminate the string
    return i;
}

// takes the string name of the serial port (e.g. "/dev/tty.usbserial","COM1")
// and a baud rate (bps) and connects to that port at that speed and 8N1.
// opens the port in fully raw mode so you can send binary data.
// returns valid fd, or -1 on error
int serialport_init(const char* serialport, int baud)
{
    struct termios toptions;
    int fd;

    //fprintf(stderr,"init_serialport: opening port %s @ %d bps\n",
    //	  serialport,baud);

    //fd = open(serialport, O_RDWR | O_NOCTTY | O_NDELAY);
    fd = open(serialport, O_RDWR | O_NOCTTY);
    if (fd == -1)  {
        perror("init_serialport: Unable to open port ");
        return -1;
    }
    if (tcgetattr(fd, &toptions) < 0) {
        perror("init_serialport: Couldn't get term attributes");
        return -1;
    }
    speed_t brate = baud; // let you override switch below if needed
    switch(baud) {
        case 4800:   brate=B4800;   break;
        case 9600:   brate=B9600;   break;
                     // if you want these speeds, uncomment these and set #defines if Linux

                     //#ifndef OSNAME_LINUX

                     //    case 14400:  brate=B14400;  break;

                     //#endif
        case 19200:  brate=B19200;  break;
                     //#ifndef OSNAME_LINUX

                     //    case 28800:  brate=B28800;  break;

                     //#endif
                     //case 28800:  brate=B28800;  break;
        case 38400:  brate=B38400;  break;
        case 57600:  brate=B57600;  break;
        case 115200: brate=B115200; break;
    }
    cfsetispeed(&toptions, brate);
    cfsetospeed(&toptions, brate);



    // 8N1
    toptions.c_cflag &= ~PARENB;
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag &= ~CSIZE;
    toptions.c_cflag |= CS8;
    // no flow control
    toptions.c_cflag &= ~CRTSCTS;

    toptions.c_cflag |= CREAD | CLOCAL;  // turn on READ & ignore ctrl lines
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY); // turn off s/w flow ctrl

    toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // make raw
    toptions.c_oflag &= ~OPOST; // make raw
    // see: http://unixwiz.net/techtips/termios-vmin-vtime.html
    toptions.c_cc[VMIN]  = 0;
    toptions.c_cc[VTIME] = 20;
    if( tcsetattr(fd, TCSANOW, &toptions) < 0) {
        perror("init_serialport: Couldn't set term attributes");
        return -1;
    }
    return fd;
}
