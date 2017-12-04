#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

const int WINDOW_HEIGHT = 50;
const int WINDOW_WIDTH = 600;

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

    int s = DefaultScreen(display);
    Window window = XCreateSimpleWindow(display, RootWindow(display, s), 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 1, BlackPixel(display, s), WhitePixel(display, s));

    Atom type = XInternAtom(display, "_NET_WM_WINDOW_TYPE_SPLASH", False);
    int ret = XChangeProperty(display, window, XInternAtom(display, "_NET_WM_WINDOW_TYPE", False), XA_ATOM, 32, PropModeReplace, (unsigned char *)&type, 1);

    printf("%i\n", ret);

    Atom state = XInternAtom(display, "_NET_WM_STATE_MODAL", False);
    XChangeProperty(display, window, XInternAtom(display, "_NET_WM_STATE", False), XA_ATOM, 32, PropModeReplace, (unsigned char *)&state, 1);

    XSelectInput(display, window, ExposureMask | KeyPressMask);
    XMapWindow(display, window);

    XFlush(display);
    printf("window %i\n", (int)window);

    Colormap colormap = DefaultColormap(display, s);

    XColor xcolor;
    xcolor.red = 64 * 256;
    xcolor.green = 132 * 256;
    xcolor.blue = 214 * 256;
    xcolor.flags = DoRed | DoGreen | DoBlue;
    XAllocColor(display, colormap, &xcolor);

    XEvent event;
    GC gc = DefaultGC(display, s);
    for (;;) {
        XNextEvent(display, &event);
        if (event.type == Expose) {
            XSetForeground(display, gc, xcolor.pixel);
            XFillRectangle(display, window, gc, 10, 10, 580, 30);
        } else if (event.type == KeyPress) {
            break;
        }
    }

    XFreeColormap(display, colormap);
    XCloseDisplay(display);
    return 0;
}
