/*	Public domain	*/

/* Structure describing an instance of the MyWidget class. */
typedef struct formatext {
	struct ag_widget _inherit;	/* Inherit from AG_Widget */
	int mySurface;			/* Surface handle */

} formaText;

extern AG_WidgetClass formaTextClass;
formaText *formaTextNew(void *);
