/*	Public domain	*/

#include <agar/gui/widget.h>

/* Structure describing an instance of the MyWidget class. */
typedef struct my_button {
	struct ag_button _inherit;	/* Inherit from AG_Widget */
} MyButton;

extern AG_WidgetClass MyButtonClass;
MyButton *MyButtonNew(void *, const char *);
