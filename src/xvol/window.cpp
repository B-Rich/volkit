#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/gl.h>
#include <GL/glx.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xmu/StdCmap.h>
#include <X11/keysym.h>

#define DEFAULT_WINDOW_WIDTH 400
#define DEFAULT_WINDOW_HEIGHT 400

Display *dpy;
Window win;
Bool doubleBuffer = True;
int window_width;
int window_height;

void init(void)
{
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glViewport(0, 0, window_width, window_height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void redraw(void)
{
  glClear(GL_COLOR_BUFFER_BIT);

  glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    glVertex2f(0.0, 0.0);
    glVertex2f(1.0, 0.0);
    glVertex2f(1.0, 1.0);
    glVertex2f(0.0, 1.0);
  glEnd();

  if (doubleBuffer)
    glXSwapBuffers(dpy, win);
  else
    glFlush();
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
		vi->depth, XA_RGB_DEFAULT_MAP, False, True);
  if (status == 1) {
    status = XGetRGBColormaps(dpy, RootWindow(dpy, vi->screen),
		&standardCmaps, &numCmaps, XA_RGB_DEFAULT_MAP);

    if (status == 1)
      for (i = 0; i < numCmaps; i++)
        if (standardCmaps[i].visualid == vi->visualid) {
          cmap = standardCmaps[i].colormap;
          XFree(standardCmaps);
          return cmap;
        }
  }

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
#ifdef TODO
  event.xclient.data.l[0] = UPDATE_WINDOW_EVENT;
#endif

  event_mask = ExposureMask | StructureNotifyMask;

  XSendEvent(dpy, win, False, event_mask, &event);
}

int main(int argc, char *argv[])
{
  GLXContext cx;
  Bool iconic = False, keepAspect = False;
  XSizeHints sizeHints = {0};
  int configuration[] = { GLX_DOUBLEBUFFER, GLX_RGBA,
  GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1, None};

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

  /* Process command line */
  for (i = 0; i < argc; i++) {
    if (argv[i][0] == '-') {
      if(!strcmp(argv[i], "-geometry")) {
        i++;
        if (i >= argc || argv[i][0] == '-') {
          fprintf(stderr, "Invalid geometry option\n");
          exit(1);
        }
        geometry = argv[i];
      }
      else if(!strcmp(argv[i], "-display")) {
        i++;
        if (i >= argc || argv[i][0] == '-') {
          fprintf(stderr, "Invalid display option\n");
          exit(1);
        }
        display = argv[i];
      }
      else if(!strcmp(argv[i], "-iconic"))
        iconic = True;
      else if(!strcmp(argv[i], "-keepaspect"))
        keepAspect = True;
      else if(!strcmp(argv[i], "-single"))
        doubleBuffer = False;
    }
  }

  /* Open connection to XServer */
  dpy = XOpenDisplay(display);
  if (dpy == NULL) {
    perror("Open display");
    exit(1);
  }

  /* Check for GLX extension */
  if (!glXQueryExtension(dpy, NULL, NULL)) {
    perror("GLX extension");
    exit(1);
  }

  /* Find OpenGL capable visual */
  if (!doubleBuffer)
    goto SingleBufferOverride;
  vi = glXChooseVisual(dpy, DefaultScreen(dpy), configuration);
  if (vi == NULL) {
  SingleBufferOverride:
    vi = glXChooseVisual(dpy, DefaultScreen(dpy), &configuration[1]);
    if (vi == NULL) {
      perror("GLX visual");
      exit(1);
    }
    doubleBuffer = False;
  }
  cmap = getSharableColormap(vi, dpy);

  /* Create OpenGL rendering context */
  cx = glXCreateContext(dpy, vi, NULL, True);
  if (cx == NULL) {
    fprintf(stderr, "Unable to initialize OpenGL rendering context\n");
    exit(1);
  }

  /* Setup window */
  flags = XParseGeometry(geometry, &x, &y,
	(unsigned int *) &width, (unsigned int *) &height);
  if (WidthValue & flags) {
    sizeHints.flags |= USSize;
    sizeHints.width = width;
  }
  if (HeightValue & flags) {
    sizeHints.flags |= USSize;
    sizeHints.height = height;
  }
  if (XValue & flags) {
    if (XNegative & flags)
      x = DisplayWidth(dpy, DefaultScreen(dpy)) + x - sizeHints.width;
    sizeHints.flags |= USPosition;
    sizeHints.x = x;
  }
  if (YValue & flags) {
    if (YNegative & flags)
      y = DisplayHeight(dpy, DefaultScreen(dpy)) + y - sizeHints.height;
    sizeHints.flags |= USPosition;
    sizeHints.y = y;
  }
  if (keepAspect) {
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

  /* Draw window */
  XMapWindow(dpy, win);

  /* Event loop */
  for (;;) {
    do {
      XNextEvent(dpy, &event);
      switch(event.type) {
        case ConfigureNotify:
          width = event.xconfigure.width;
          height = event.xconfigure.height;
          init();
        case Expose:
	  redraw();
          break;
        case KeyPress:
	  ks = XLookupKeysym((XKeyEvent *) &event, 0);
	  redraw();
          break;
        case KeyRelease:
	  ks = XLookupKeysym((XKeyEvent *) &event, 0);
	  redraw();
          break;
        case ClientMessage:
	  if (event.xclient.data.l[0] == wmDeleteWindow) {
            exit(0);
          }
#ifdef TODO
	  else if(event.xclient.data.l[0] == UPDATE_WINDOW_EVENT) {
            redraw();
          }
#endif
          break;
      }
    } while (XPending(dpy));

    redraw();
  }

  return 0;
}

