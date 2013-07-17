#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <GL/gl.h>
#include <GL/glx.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xmu/StdCmap.h>
#include <X11/keysym.h>

#include "img/imgloader.h"
#include "img/colormap.h"
#include "volren/volren.h"
#include "volren/matrix.h"
#include "volren/coord.h"

#define DEFAULT_WINDOW_WIDTH 400
#define DEFAULT_WINDOW_HEIGHT 400

#define UPDATE_WINDOW_EVENT 99

Display *dpy;
Window win;
Img *img = 0;
uint32_t *imgData = 0;
Bool doubleBuffer = True;
int window_width;
int window_height;

int load_image(const char *fn)
{
    ColorMap colorMap;
    int result = 1;

    img = ImgLoader::open(fn);
    img->setTransparency(Img::TRANSPARENCY_VOXEL);
    if (img)
    {
        result = img->read(0);
        if (result == 0)
        {
            imgData =
             new uint32_t[img->getWidth() * img->getHeight() * img->getDepth()];
            if (imgData)
            {
                img->getData(imgData, &colorMap);
            }
        }
        else
        {
            result = -1;
        }
    }

    return result;
}

int init_texture()
{
    GLuint id;

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_3D, id);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage3D(GL_TEXTURE_3D, 0,
                 GL_RGBA, img->getWidth(), img->getHeight(), img->getDepth(), 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, imgData);

    return id;
}

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
  init_texture();
  glEnable(GL_TEXTURE_3D);
}

void draw_brick(void)
{
    int i, n;
    VRState state;
    VRView view;
    VRVolumeData vd;
    Brick br;

    // Initialize state
    view.delta = 1.0 / img->getDepth();
    state.view = &view;

    // Initialize volume data
    vd.nxBricks = 1;
    vd.nyBricks = 1;
    vd.nzBricks = 1;

    // Initialize brick
    br.xOff = 0.0;
    br.yOff = 0.0;
    br.zOff = 0.0;
    br.txScl = 1.0;
    br.tyScl = 1.0;
    br.tzScl = 1.0;
    br.txOff = 0.0;
    br.tyOff = 0.0;
    br.tzOff = 0.0;

    render_brick(&state, &vd, &br, 1);
}

void redraw(void)
{
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(1.0, 1.0, 1.0);
  draw_brick();
#if 0
  glBegin(GL_QUADS);
    glVertex2f(0.0, 0.0);
    glVertex2f(1.0, 0.0);
    glVertex2f(1.0, 1.0);
    glVertex2f(0.0, 1.0);
  glEnd();
#endif

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
  event.xclient.data.l[0] = UPDATE_WINDOW_EVENT;

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

  /* Load image */
  if (load_image(argv[1]) != 0)
  {
      fprintf(stderr, "Error - Unable to load image %s\n", argv[1]);
      return 1;
  }

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
	  else if(event.xclient.data.l[0] == UPDATE_WINDOW_EVENT) {
            redraw();
          }
          break;
      }
    } while (XPending(dpy));

    redraw();
  }

  return 0;
}

