/*	Public domain	*/

/*
 * Implementation of a typical Agar widget which uses surface mappings to
 * efficiently draw surfaces in either framebuffer or GL display mode.
 *
 * If you are not familiar with the way the Agar object system handles
 * inheritance, see demos/objsystem.
 */

#include <core.h>
#include <gui.h>
#include <agar/gui/widget.h>
#include "myButton.h"


#define WSURFACE(wi,ind)		AGWIDGET_SURFACE((wi),(ind))

using namespace std;
#include <string>

/*
 * This is a generic constructor function. It is completely optional, but
 * customary of FooNew() functions to allocate, initialize and attach an
 * instance of the class.
 */
MyButton *
MyButtonNew(void *parent, const char *foo)
{
	MyButton *my;

	/* Create a new instance of the MyButton class */
	my = (MyButton*)malloc(sizeof(MyButton));
	AG_ObjectInit(my, &MyButtonClass);

	/* Set some constructor arguments */
	AG_ButtonText((AG_Button*)my,foo);
	/* Attach the object to the parent (no-op if parent is NULL) */
	AG_ObjectAttach(parent, my);

	return (my);
}

/*
 * Initialization routine. Note that the object system will automatically
 * invoke the initialization routines of the parent classes first.
 */
static void
Init(void *obj)
{
	MyButton *my = (MyButton*)obj;
}


static void
SizeRequest(void *p, AG_SizeReq *r)
{
	MyButton *bu = (MyButton*)p;
	AG_SizeReq rLbl;

	r->w = bu->_inherit.lPad + bu->_inherit.rPad;
	r->h = bu->_inherit.tPad + bu->_inherit.bPad;

	if (bu->_inherit.surface != -1) {
		r->w += WSURFACE(bu,bu->_inherit.surface)->w;
		r->h += WSURFACE(bu,bu->_inherit.surface)->h;
	} else {
		if (bu->_inherit.lbl != NULL) {
			AG_WidgetSizeReq(bu->_inherit.lbl, &rLbl);
			r->w += rLbl.w;
		}
		std::string txt(bu->_inherit.lbl->text);
		int i=1;
		int j=txt.find("\n",0);
		while(j != txt.npos)
		{
            i++;
            j++;
            j=txt.find("\n",j);
		}
		r->h += agTextFontHeight*i;
	}
}

/*
 * This structure describes our widget class. It inherits from AG_ObjectClass.
 * Any of the function members may be NULL. See AG_Widget(3) for details.
 */
AG_WidgetClass MyButtonClass = {
	{
		"AG_Widget:AG_Button:MyButton",	/* Name of class */
		sizeof(MyButton),	/* Size of structure */
		{ 0,0 },		/* Version for load/save */
		Init,			/* Initialize dataset */
		NULL,			/* Free dataset */
		NULL,			/* Destroy widget */
		NULL,			/* Load widget (for GUI builder) */
		NULL,			/* Save widget (for GUI builder) */
		NULL			/* Edit (for GUI builder) */
	},
	AG_WidgetInheritDraw,
	SizeRequest,
	AG_WidgetInheritSizeAllocate
};
