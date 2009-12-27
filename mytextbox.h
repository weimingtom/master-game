/*	Public domain	*/
#include <agar/core/types.h>
#include <agar/gui/widget.h>
//#include "myeditable.h"


typedef struct ag_mytextbox {
	struct ag_textbox _inherit;
	} AG_myTextbox;

extern AG_WidgetClass agMyTextboxClass;
#define AG_myTextboxBindUTF8(tb,p,sz) AG_EditableBindUTF8((tb)->_inherit.ed,(p),(sz))
AG_myTextbox *AG_myTextboxNew(void *, Uint, const char *);

