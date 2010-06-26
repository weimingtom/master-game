/*	Public domain	*/

/*
 * Implementation of a typical Agar widget which uses surface mappings to
 * efficiently draw surfaces, regardless of the underlying graphics system.
 *
 * If you are not familiar with the way the Agar object system handles
 * inheritance, see demos/objsystem.
 */

#include <agar/core.h>
#include <agar/gui.h>

#include "formatext.h"

#include "htmlayout/htmlite.h"

#include "htmlayout/htmlayout_dom.hpp"

#include "globals.hpp"

/*
 * This is a generic constructor function. It is completely optional, but
 * customary of FooNew() functions to allocate, initialize and attach an
 * instance of the class.
 */
formaText *
formaTextNew(void *parent)
{
	formaText *my;

	/* Create a new instance of the formaText class */
	my = (formaText*)malloc(sizeof(formaText));
	AG_ObjectInit(my, &formaTextClass);

	/* Attach the object to the parent (no-op if parent is NULL) */
	AG_ObjectAttach(parent, my);

	return (my);
}

/*
 * This function requests a minimal geometry for displaying the widget.
 * It is expected to return the width and height in pixels into r.
 *
 * Note: Some widgets will provide FooSizeHint() functions to allow the
 * programmer to request an initial size in pixels or some other metric
 * FooSizeHint() typically sets some structure variable, which are then
 * used here.
 */
static void
SizeRequest(void *p, AG_SizeReq *r)
{
	formaText *my = (formaText*)p;

	if (my->mySurface == -1) {
		/*
		 * We can use AG_TextSize() to return the dimensions of rendered
		 * text, without rendering it.
		 */
		AG_TextFont(AG_FetchFont(NULL, 24, 0));
		AG_TextSize("Custom widget!", &r->w, &r->h);

	} else {
		/*
		 * We can use the geometry of the rendered surface. The
		 * AGWIDGET_SURFACE() macro returns the AG_Surface given a
		 * Widget surface handle.
		 */
		r->w = AGWIDGET_SURFACE(my,my->mySurface)->w;
		r->h = AGWIDGET_SURFACE(my,my->mySurface)->h;
		//r->w=(min(r->w,600));
	}
}

/*
 * This function is called by the parent widget after it decided how much
 * space to allocate to this widget. It is mostly useful to container
 * widgets, but other widgets generally use it to check if the allocated
 * geometry can be handled by Draw().
 */
static int
SizeAllocate(void *p, const AG_SizeAlloc *a)
{
	formaText *my = (formaText*)p;

	/* If we return -1, Draw() will not be called. */
	if (a->w < 5 || a->h < 5)
		return (-1);

	printf("Allocated %dx%d pixels\n", a->w, a->h);
	return (0);
}

/*
 * Draw function. Invoked from GUI rendering context to draw the widget
 * at its current location. All primitive and surface operations operate
 * on widget coordinates.
 */
static void
Draw(void *p)
{
	formaText *my = (formaText*)p;


	/*
	 * Render some text into a new surface. In OpenGL mode, the
	 * AG_WidgetMapSurface() call involves a texture upload.
	 */
	if (my->mySurface == -1) {
		my->mySurface = AG_WidgetMapSurface(my,
		    surf1);
	};


  //AG_WidgetUpdateSurface(my,my->mySurface);
	/* Blit the mapped surface at [0,0]. */
	AG_WidgetBlitSurface(my, my->mySurface, 0, 0);
}

/* Mouse motion event handler */
static void
MouseMotion(AG_Event *event)
{
	formaText *my = (formaText*)AG_SELF();
	int x = AG_INT(1);
	int y = AG_INT(2);

	/* ... */
}

/* Mouse click event handler */
static void
MouseButtonDown(AG_Event *event)
{
	formaText *my = (formaText*)AG_SELF();
	int button = AG_INT(1);
	int x = AG_INT(2);
	int y = AG_INT(3);

	if (button != AG_MOUSE_LEFT) {
		return;
	}
	printf("Click at %d,%d\n", x, y);
	AG_WidgetFocus(my);
}

/* Mouse click event handler */
static void
MouseButtonUp(AG_Event *event)
{
	formaText *my = (formaText*)AG_SELF();
	int button = AG_INT(1);
	int x = AG_INT(2);
	int y = AG_INT(3);

	/* ... */
}

/* Keystroke event handler */
static void
KeyDown(AG_Event *event)
{
	formaText *my = (formaText*)AG_SELF();
	int keysym = AG_INT(1);

	printf("Keystroke: 0x%x\n", keysym);
}

/* Keystroke event handler */
static void
KeyUp(AG_Event *event)
{
	formaText *my = (formaText*)AG_SELF();
	int keysym = AG_INT(1);

	/* ... */
}

/*
 * Initialization routine. Note that the object system will automatically
 * invoke the initialization routines of the parent classes first.
 */
static void
Init(void *obj)
{
	formaText *my = (formaText*)obj;

	/* Allow this widget to grab focus. */
	AGWIDGET(my)->flags |= AG_WIDGET_FOCUSABLE;


	/*
	 * We'll eventually need to create and map a surface, but we cannot
	 * do this from Init(), because it involves texture operations in
	 * GL mode which are thread-unsafe. We wait until Draw() to do that.
	 */
	my->mySurface = -1;

	/*
	 * Map our event handlers. For a list of all meaningful events
	 * we can handle, see AG_Object(3), AG_Widget(3) and AG_Window(3).
	 *
	 * Here we register handlers for the common AG_Window(3) events.
	 */
	AG_SetEvent(my, "mouse-button-up", MouseButtonUp, NULL);
	AG_SetEvent(my, "mouse-button-down", MouseButtonDown, NULL);
	AG_SetEvent(my, "mouse-motion", MouseMotion, NULL);
	AG_SetEvent(my, "key-up", KeyUp, NULL);
	AG_SetEvent(my, "key-down", KeyDown, NULL);
}

/*
 * This structure describes our widget class. It inherits from AG_ObjectClass.
 * Any of the function members may be NULL. See AG_Widget(3) for details.
 */
AG_WidgetClass formaTextClass = {
	{
		"AG_Widget:formaText",	/* Name of class */
		sizeof(formaText),	/* Size of structure */
		{ 0,0 },		/* Version for load/save */
		Init,			/* Initialize dataset */
		NULL,			/* Free dataset */
		NULL,			/* Destroy widget */
		NULL,			/* Load widget (for GUI builder) */
		NULL,			/* Save widget (for GUI builder) */
		NULL			/* Edit (for GUI builder) */
	},
	Draw,				/* Render widget */
	SizeRequest,			/* Default size requisition */
	SizeAllocate			/* Size allocation callback */
};
