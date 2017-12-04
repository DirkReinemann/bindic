#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

const int WINDOW_HEIGHT = 50;
const int WINDOW_WIDTH = 600;
const int WAIT_MILLIS = 500;

typedef struct Hints {
    unsigned long flags;
    unsigned long functions;
    unsigned long decorations;
    long          inputMode;
    unsigned long status;
} Hints;

int main(int argc, char **argv)
{
    if (argc != 3) {
        printf("Usage: %s [VALUE] [MAX]\n", argv[0]);
        return 1;
    }

    int val = atol(argv[1]);
    int max = atol(argv[2]);

    if (val > max) {
        printf("The actual value '%i' is bigger than the maximum value '%i'\n.", val, max);
        return 3;
    }

    Display *display = XOpenDisplay(NULL);
    if (display == NULL) {
        fprintf(stderr, "Cannot open display\n");
        exit(1);
    }

    int screen = DefaultScreen(display);
    Window window = XCreateSimpleWindow(display, RootWindow(display, screen), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 1, BlackPixel(display, screen), WhitePixel(display, screen));

    // set modal window
    Atom state = XInternAtom(display, "_NET_WM_STATE_MODAL", False);
    XChangeProperty(display, window, XInternAtom(display, "_NET_WM_STATE", False), XA_ATOM, 32, PropModeReplace, (unsigned char *)&state, 1);

    // remove window decorations
    Hints hints;
    Atom property;
    hints.flags = 2;
    hints.decorations = 0;
    property = XInternAtom(display, "_MOTIF_WM_HINTS", False);
    XChangeProperty(display, window, property, property, 32, PropModeReplace, (unsigned char *)&hints, 5);

    // remove input events
    XWMHints *wmhints = XAllocWMHints();
    wmhints->flags = InputHint;
    wmhints->input = False;
    XSetWMHints(display, window, wmhints);
    XFree(wmhints);

    XSelectInput(display, window, ExposureMask);
    XMapWindow(display, window);

    Colormap colormap = DefaultColormap(display, screen);

    XColor xcolor;
    xcolor.red = 64 * 256;
    xcolor.green = 132 * 256;
    xcolor.blue = 214 * 256;
    xcolor.flags = DoRed | DoGreen | DoBlue;
    XAllocColor(display, colormap, &xcolor);

    XEvent event;
    GC gc = DefaultGC(display, screen);
    for (;;) {
        XNextEvent(display, &event);
        if (event.type == Expose) {
            int fill = (int)(WINDOW_WIDTH * val) / max;

            XSetForeground(display, gc, xcolor.pixel);
            XFillRectangle(display, window, gc, 0, 0, fill, WINDOW_HEIGHT);
            XFlush(display);

            usleep(WAIT_MILLIS * 1000);
            break;
        }
    }

    XFreeColormap(display, colormap);
    XCloseDisplay(display);
    return 0;
}
