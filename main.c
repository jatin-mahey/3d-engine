#include <X11/X.h>
#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>
#include "draw.h"

void draw(Display* display, Window win, GC gc, float screen_w, float screen_h);


int main() {

    Display* display;
    display = XOpenDisplay(":0");
    if (display == NULL)
    {
        fprintf(stderr, "Cannot connect to X server.\n");
        exit(-1);
    }

    XEvent xev;
    // store size of the screen
    int screen_w;
    int screen_h;
    int screen_n;
    // id of root window (covers the whole screen) 
    Window root_win;
    // id of black and white colors
    unsigned long white_pixel;
    unsigned long black_pixel;

    // get all the values
    screen_n    = DefaultScreen(display);
    screen_w    = DisplayWidth(display, screen_n);
    screen_h    = DisplayHeight(display, screen_n);
    root_win    = RootWindow(display, screen_n);

    // create our own window
    Window win;
    int win_w;
    int win_h;
    int win_x;
    int win_y;

    win_w = screen_w / 3;
    win_h = screen_h / 3;

    win_x = win_y = 0;

    unsigned int win_border;
    unsigned long win_border_w = 1;
    unsigned long background;

    win = XCreateSimpleWindow(display, root_win,
                              win_x, win_y,
                              win_w, win_h,
                              win_border_w,
                              BlackPixel(display, screen_n),
                              WhitePixel(display, screen_n));
    // register events
    XSelectInput(display, win, ExposureMask | ButtonPressMask);

    // win will be create but not display
    // in order to display the newly created window, we need to map
    XMapWindow(display, win);
    XFlush(display);

    // change window background
    XClearWindow(display, win);
    XSetWindowBackground(display, win, BlackPixel(display, screen_n));

    // Graphical context for drawing
    GC gc;
    XGCValues values;
    values.cap_style = CapButt;
    values.join_style = JoinBevel;

    unsigned long valuemask = GCCapStyle | GCJoinStyle;
    gc = XCreateGC(display, win, valuemask, &values);
    if (gc < 0)
    {
         fprintf(stderr, "XCreateGC: \n");
    }

    // setup canvas and style
    // set window foreground color to white
    XSetForeground(display, gc, WhitePixel(display, screen_n));
    // set window background color to black
    XSetBackground(display, gc, BlackPixel(display, screen_n));
    // change fill style to solid
    XSetFillStyle(display, gc, FillSolid);
    // change line drawing attributes
    XSetLineAttributes(display, gc, 2, LineSolid, CapRound, JoinRound);
    // sync
    XFlush(display);
    XSync(display, False);

    while (1)
    {
        XNextEvent(display, &xev);
        switch (xev.type)
        {
            // drawing stuff
            case Expose: draw(display, win, gc, screen_w, screen_h);
            // keep the window on till there is button pressed
            case ButtonPress: break;
        }
    }

    // free all the resources
    XUnmapWindow(display, win);
    XDestroyWindow(display, win);
    XCloseDisplay(display);
    return 0;
}


void draw(Display* display, Window win, GC gc, float screen_w, float screen_h)
{
    tri mesh[] = {
        (tri){ (vec3){ 0.0f, 0.0f, 0.0f },    (vec3){ 0.0f, 1.0f, 0.0f },    (vec3){ 1.0f, 1.0f, 0.0f } },
        (tri){ (vec3){ 0.0f, 0.0f, 0.0f },    (vec3){ 1.0f, 1.0f, 0.0f },    (vec3){ 1.0f, 0.0f, 0.0f } },
        (tri){ (vec3){ 1.0f, 0.0f, 0.0f },    (vec3){ 1.0f, 1.0f, 0.0f },    (vec3){ 1.0f, 1.0f, 1.0f } },
        (tri){ (vec3){ 1.0f, 0.0f, 0.0f },    (vec3){ 1.0f, 1.0f, 1.0f },    (vec3){ 1.0f, 0.0f, 1.0f } },
        (tri){ (vec3){ 1.0f, 0.0f, 1.0f },    (vec3){ 1.0f, 1.0f, 1.0f },    (vec3){ 0.0f, 1.0f, 1.0f } },
        (tri){ (vec3){ 1.0f, 0.0f, 1.0f },    (vec3){ 0.0f, 1.0f, 1.0f },    (vec3){ 0.0f, 0.0f, 1.0f } },
        (tri){ (vec3){ 0.0f, 0.0f, 1.0f },    (vec3){ 0.0f, 1.0f, 1.0f },    (vec3){ 0.0f, 1.0f, 0.0f } },
        (tri){ (vec3){ 0.0f, 0.0f, 1.0f },    (vec3){ 0.0f, 1.0f, 0.0f },    (vec3){ 0.0f, 0.0f, 0.0f } },
        (tri){ (vec3){ 0.0f, 1.0f, 0.0f },    (vec3){ 0.0f, 1.0f, 1.0f },    (vec3){ 1.0f, 1.0f, 1.0f } },
        (tri){ (vec3){ 0.0f, 1.0f, 0.0f },    (vec3){ 1.0f, 1.0f, 1.0f },    (vec3){ 1.0f, 1.0f, 0.0f } },
        (tri){ (vec3){ 1.0f, 0.0f, 1.0f },    (vec3){ 0.0f, 0.0f, 1.0f },    (vec3){ 0.0f, 0.0f, 0.0f } },
        (tri){ (vec3){ 1.0f, 0.0f, 1.0f },    (vec3){ 0.0f, 0.0f, 0.0f },    (vec3){ 1.0f, 0.0f, 0.0f } }
    };

    tri projected;
    mat4x4 projCoff = {0};
    XPoint points[4] = {0};
    int npoints = 0;
    // generate projection cofficients
    projection(&projCoff, screen_w, screen_h);

    for (int i=0; i<12; i++)
    {
        projected = (tri){ 0 };

        // offset into screen
        /*projected.t[0].z = projected.t[0].z + 3.0f;*/
        /*projected.t[1].z = projected.t[1].z + 3.0f;*/
        /*projected.t[2].z = projected.t[2].z + 3.0f;*/

        // 3d -> 2d
        for (int j=0; j<3; j++)
        {
            mulmatrix(&(mesh[i].t[j]), &projCoff, &(projected.t[j]));
        }
        // scale to screen
        projected.t[0].x *= (0.5f*screen_w); projected.t[0].y *= (0.5f*screen_h);
        projected.t[1].x *= (0.5f*screen_w); projected.t[1].y *= (0.5f*screen_h);
        projected.t[2].x *= (0.5f*screen_w); projected.t[2].y *= (0.5f*screen_h);

        // draw triangle using points
        points[0] = (XPoint){ projected.t[0].x,  projected.t[0].y };
        points[1] = (XPoint){ projected.t[1].x,  projected.t[1].y };
        points[2] = (XPoint){ projected.t[2].x,  projected.t[2].y };
        points[3] = (XPoint){ projected.t[0].x,  projected.t[0].y };

        npoints = sizeof(points)/sizeof(XPoint);
        XDrawLines(display, win, gc, points, npoints, CoordModeOrigin);
    }
}
