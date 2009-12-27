/*	Public domain	*/

typedef struct myeditable {
	struct ag_editable _inherit;
} myEditable;
extern AG_WidgetClass myEditableClass;

myEditable *myEditableNew(void *, Uint);
