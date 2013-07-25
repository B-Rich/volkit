#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

#include <GL/gl.h>
#include <GL/glx.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xmu/StdCmap.h>
#include <X11/keysym.h>

#include "volimg/imgloader.h"
#include "volimg/colormap.h"
#include "volsurf/volsurf.h"

#define DEFAULT_WINDOW_WIDTH  400
#define DEFAULT_WINDOW_HEIGHT 400
#define MAX_LEVEL             2000
#define MIN_LEVEL             0
#define STEP_LEVEL            10

#define UPDATE_WINDOW_EVENT 99

Display *dpy;
Window win;
int window_width;
int window_height;
Img *img = 0;
ColorMap colorMap;
bool initialized = false;
Bool doubleBuffer = True;
float x_angle = 0.0;
float y_angle = 0.0;
float curr_level = 700;
Grid grid;

int load_image(const char *fn)
{
    int result = 1;

    img = ImgLoader::open(fn);
    img->setTransparency(Img::TRANSPARENCY_VOXEL);
    if (img)
    {
        result = img->read(0);
    }

    return result;
}

int startup()
{
    int i, j, k;
    float dx, dy, dz;
    int index = 0;

    grid.nx = 100; grid.ny = 75; grid.nz = 50;
    grid.dx = 0.02; grid.dy = 0.02; grid.dz = 0.02;
    grid.rx = 1; grid.ry = 1; grid.rz = 1;
    grid.data = new uint32_t[grid.nx * grid.ny * grid.nz];
    for (k = 0; k < grid.nz; k++)
    {
        dz = 2 * (k - grid.nz / 2) / float(grid.nz);
        for (j = 0; j < grid.ny; j++)
        {
            dy = 2 * (j - grid.ny / 2) / float(grid.ny);
            for (i = 0; i < grid.nx; i++)
            {
                dx = 2 * (i - grid.nx / 2) / float(grid.nx);
                grid.data[index] = 1000 * sqrt(dx * dx + dy * dy + dz * dz);
                index++;
            }
        }
    }
}

void init()
{
    glViewport(0, 0, window_width, window_height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(-1.0, -1.0, 0.0);
}

void redraw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);
    vs_draw_iso_surface(&grid, curr_level);

    if (doubleBuffer)
    {
        glXSwapBuffers(dpy, win);
    }
    else
    {
        glFlush();
    }
}

Colormap getSharableColormap(XVisualInfo *vi, Display *dpy)
{
    Status status;
    XStandardColormap *standardCmaps;
    Colormap cmap;
    int i, numCmaps;

#ifdef TODO
    /* True color only */
    if (vi->class != TrueColor) {
        fprintf(stderr, "Only true color supported\n");
        exit(1);
    }
#endif

    /* If no standard colormap but TrueColor make an unshared one */
    status = XmuLookupStandardColormap(dpy, vi->screen, vi->visualid,
                                       vi->depth, XA_RGB_DEFAULT_MAP,
                                       False, True);
    if (status == 1)
    {
        status = XGetRGBColormaps(dpy, RootWindow(dpy, vi->screen),
                                  &standardCmaps, &numCmaps,
                                  XA_RGB_DEFAULT_MAP);
        if (status == 1)
        {
            for (i = 0; i < numCmaps; i++)
            {
                if (standardCmaps[i].visualid == vi->visualid)
                {
                    cmap = standardCmaps[i].colormap;
                    XFree(standardCmaps);
                    return cmap;
                }
            }
        }
    }

    /* Generate new colormap */
    cmap = XCreateColormap(dpy, RootWindow(dpy, vi->screen),
                           vi->visual, AllocNone);

    return cmap;
}

void generateTimeoutEvent(Display *dpy, Window win)
{
    XEvent event;
    long event_mask;

    event.type = ClientMessage;
    event.xclient.type = ClientMessage;
    event.xclient.serial = 0;
    event.xclient.send_event = True;
    event.xclient.display = dpy;
    event.xclient.window = win;
    event.xclient.message_type = 0;
    event.xclient.format = 32;
    event.xclient.data.l[0] = UPDATE_WINDOW_EVENT;

    event_mask = ExposureMask | StructureNotifyMask;

    XSendEvent(dpy, win, False, event_mask, &event);
}

int main(int argc, char *argv[])
{
    GLXContext cx;
    Bool iconic = False, keepAspect = False;
    XSizeHints sizeHints = {0};
    int configuration[] = {
        GLX_DOUBLEBUFFER,
        GLX_RGBA,
        GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE,
        1, None
    };

    XVisualInfo *vi;
    Colormap cmap;
    XSetWindowAttributes swa;
    XWMHints *wmHints;
    Atom wmDeleteWindow;
    XEvent event;
    KeySym ks;
    char *display = NULL, *geometry = NULL;
    int flags, x, y, i;
    int width = DEFAULT_WINDOW_WIDTH;
    int height = DEFAULT_WINDOW_HEIGHT;
    char *extensions;
    int postRedraw = 0;

    /* Process command line */
    for (i = 0; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            if(!strcmp(argv[i], "-geometry"))
            {
                i++;
                if (i >= argc || argv[i][0] == '-')
                {
                    fprintf(stderr, "Invalid geometry option\n");
                    return 1;
                }
                geometry = argv[i];
            }
            else if(!strcmp(argv[i], "-display"))
            {
                i++;
                if (i >= argc || argv[i][0] == '-')
                {
                    fprintf(stderr, "Invalid display option\n");
                    return 1;
                }
                display = argv[i];
            }
            else if(!strcmp(argv[i], "-iconic"))
            {
                iconic = True;
            }
            else if(!strcmp(argv[i], "-keepaspect"))
            {
                keepAspect = True;
            }
            else if(!strcmp(argv[i], "-single"))
            {
                doubleBuffer = False;
            }
        }
    }

    /* Open connection to XServer */
    dpy = XOpenDisplay(display);
    if (dpy == NULL)
    {
        perror("Open display");
        return 1;
    }

    /* Check for GLX extension */
    if (!glXQueryExtension(dpy, NULL, NULL))
    {
        perror("GLX extension");
        return 1;
    }

    /* Find OpenGL capable visual */
    if (!doubleBuffer)
    {
        goto SingleBufferOverride;
    }

    vi = glXChooseVisual(dpy, DefaultScreen(dpy), configuration);
    if (vi == NULL)
    {

/* Label */
SingleBufferOverride:

        vi = glXChooseVisual(dpy, DefaultScreen(dpy), &configuration[1]);
        if (vi == NULL)
        {
            perror("GLX visual");
            return 1;
        }
        doubleBuffer = False;
    }
    cmap = getSharableColormap(vi, dpy);

    /* Create OpenGL rendering context */
    cx = glXCreateContext(dpy, vi, NULL, True);
    if (cx == NULL)
    {
        fprintf(stderr, "Unable to initialize OpenGL rendering context\n");
        return 1;
    }

    /* Setup window */
    flags = XParseGeometry(geometry, &x, &y,
	                   (unsigned int *) &width,
                           (unsigned int *) &height);
    if (WidthValue & flags)
    {
        sizeHints.flags |= USSize;
        sizeHints.width = width;
    }

    if (HeightValue & flags)
    {
        sizeHints.flags |= USSize;
        sizeHints.height = height;
    }

    if (XValue & flags)
    {
        if (XNegative & flags)
        {
            x = DisplayWidth(dpy, DefaultScreen(dpy)) + x - sizeHints.width;
        }
        sizeHints.flags |= USPosition;
        sizeHints.x = x;
    }

    if (YValue & flags)
    {
        if (YNegative & flags)
        {
            y = DisplayHeight(dpy, DefaultScreen(dpy)) + y - sizeHints.height;
        }
        sizeHints.flags |= USPosition;
        sizeHints.y = y;
    }

    if (keepAspect)
    {
        sizeHints.flags |= PAspect;
        sizeHints.min_aspect.x = sizeHints.max_aspect.x = width;
        sizeHints.min_aspect.y = sizeHints.max_aspect.y = height;
    }

    swa.colormap = cmap;
    swa.border_pixel = 0;
    swa.event_mask = ExposureMask | StructureNotifyMask |
	             KeyPressMask | KeyReleaseMask;

    win = XCreateWindow(dpy, RootWindow(dpy, vi->screen),
                        sizeHints.x, sizeHints.y, width, height,
                        0, vi->depth, InputOutput, vi->visual,
                        CWBorderPixel | CWColormap | CWEventMask, &swa);

    /* Setup window properties */
    XSetStandardProperties(dpy, win, "xvol", argv[0],
                           None, argv, argc, &sizeHints);
    wmHints = XAllocWMHints();
    wmHints->initial_state = iconic ? IconicState : NormalState;
    wmHints->flags = StateHint;
    XSetWMHints(dpy, win, wmHints);
    wmDeleteWindow = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(dpy, win, &wmDeleteWindow, 1);

    /* Set GLX context */
    glXMakeCurrent(dpy, win, cx);
    window_width = width;
    window_height = height;

    extensions = (char*) glGetString(GL_EXTENSIONS);
    if (strstr(extensions, "EXT_texture3D") == NULL)
    {
        fprintf(stderr, "No support for 3D textures found\n");
        return 1;
    }

#ifdef NOT_YET
    /* Load image */
    if (load_image(argv[1]) != 0)
    {
        fprintf(stderr, "Error - Unable to load image %s\n", argv[1]);
        return 1;
    }

    if (argc > 2)
    {
        if (colorMap.loadColormap(argv[2]) != 0)
        {
            fprintf(stderr, "Error - Unable to load colormap %s\n", argv[2]);
            return 1;
        }
    }
#endif

    startup();

    /* Draw window */
    XMapWindow(dpy, win);

    /* Event loop */
    for (;;) {
        do
        {
            XNextEvent(dpy, &event);
            switch(event.type) {
                case ConfigureNotify:
                    window_width = event.xconfigure.width;
                    window_height = event.xconfigure.height;
                    init();
                    /* Fall trough */

                case Expose:
                    postRedraw = 1;
                    break;

                case KeyPress:
                    ks = XLookupKeysym((XKeyEvent *) &event, 0);
                    if (ks == XK_Tab || ks == XK_BackSpace)
                    {
                        if (ks == XK_Tab)
                        {
                            curr_level += STEP_LEVEL;
                            if (curr_level > MAX_LEVEL)
                            {
                                curr_level = MAX_LEVEL;
                            }
                        }
                        else if (ks == XK_BackSpace)
                        {
                            curr_level -= STEP_LEVEL;
                            if (curr_level < MIN_LEVEL)
                            {
                                curr_level = MIN_LEVEL;
                            }
                        }
                        printf("Level: %g\n", curr_level);
                        init();
                        postRedraw = 1;
                    }

                    if (ks == XK_Left)
                    {
                        y_angle += 0.1;
                        postRedraw = 1;
                    }
                    else if (ks == XK_Right)
                    {
                        y_angle -= 0.1;
                        postRedraw = 1;
                    }

                    if (ks == XK_Down)
                    {
                        x_angle += 0.1;
                        postRedraw = 1;
                    }
                    else if (ks == XK_Up)
                    {
                        x_angle -= 0.1;
                        postRedraw = 1;
                    }
                    break;

                case ClientMessage:
                    if (event.xclient.data.l[0] == wmDeleteWindow)
                    {
                        exit(0);
                    }
                    else if(event.xclient.data.l[0] == UPDATE_WINDOW_EVENT)
                    {
                        postRedraw = 1;
                    }
                    break;
            }

        } while (XPending(dpy));

        if (postRedraw)
        {
            redraw();
            postRedraw = 0;
        }
    }

    return 0;
}

