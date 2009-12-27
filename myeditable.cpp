/*
 * Low-level single/multi-line text input widget. This is the base widget
 * used by AG_Textbox(3), editable cells in AG_Table(3), etc.
 */
#include "myeditable.h"

myEditable *
myEditableNew(void *parent, Uint flags)
{
	AG_Editable *ed;

	ed = Malloc(sizeof(AG_Editable));
	AG_ObjectInit(ed, &agEditableClass);

	if ((flags & AG_EDITABLE_NO_HFILL) == 0)
		AG_ExpandHoriz(ed);
	if (flags & AG_EDITABLE_VFILL)
		AG_ExpandVert(ed);
	if (flags & AG_EDITABLE_CATCH_TAB)
		WIDGET(ed)->flags |= AG_WIDGET_CATCH_TAB;

	ed->flags |= flags;

	AG_ObjectAttach(parent, ed);
	return (ed);
}

/*
static void
Draw(void *obj)
{
	AG_Editable *ed = obj;
	AG_Variable *stringb;
	char *s;
	int i, dx, dy, x, y;
	Uint32 *ucs;
	size_t len;
#ifdef HAVE_OPENGL
	GLboolean blend_sv;
	GLint blend_sfactor, blend_dfactor;
	GLfloat texenvmode;
#endif

	stringb = AG_GetVariable(ed, "string", &s);
	GetStringUCS4(ed, s, &ucs, &len);

#ifdef HAVE_OPENGL
	if (agView->opengl)  {
		glGetTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, &texenvmode);
		glGetBooleanv(GL_BLEND, &blend_sv);
		glGetIntegerv(GL_BLEND_SRC, &blend_sfactor);
		glGetIntegerv(GL_BLEND_DST, &blend_dfactor);

		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
#endif
	AG_PushClipRect(ed, ed->r);

	AG_PushTextState();
	AG_TextColor(TEXTBOX_TXT_COLOR);
	x = 0;
	y = -ed->y*agTextFontLineSkip;
	ed->xMax = 10;
	ed->yMax = 1;
	for (i = 0; i <= len; i++) {
		AG_Glyph *gl;
		Uint32 c = ucs[i];

		if (i == ed->pos && AG_WidgetIsFocused(ed)) {
			if ((ed->flags & AG_EDITABLE_BLINK_ON) &&
			    ed->y >= 0 && ed->y <= ed->yMax-1) {
				AG_DrawLineV(ed,
				    x - ed->x, (y + 1),
				    (y + agTextFontLineSkip - 1),
				    AG_COLOR(TEXTBOX_CURSOR_COLOR));
			}
			ed->xCurs = x;
			if (ed->flags & AG_EDITABLE_MARKPREF) {
				ed->flags &= ~(AG_EDITABLE_MARKPREF);
				ed->xCursPref = x;
			}
			ed->yCurs = y/agTextFontLineSkip + ed->y;
		}
		if (i == len)
			break;

		if (WrapAtChar(ed, x, &ucs[i])) {
			y += agTextFontLineSkip;
			ed->xMax = MAX(ed->xMax, x);
			ed->yMax++;
			x = 0;
		}
		if (c == '\n') {
			y += agTextFontLineSkip;
			ed->xMax = MAX(ed->xMax, x+10);
			ed->yMax++;
			x = 0;
			continue;
		} else if (c == '\t') {
			x += agTextTabWidth;
			continue;
		}

		c = (ed->flags & AG_EDITABLE_PASSWORD) ? '*' : c;
		gl = AG_TextRenderGlyph(c);
		dx = WIDGET(ed)->rView.x1 + x - ed->x;
		dy = WIDGET(ed)->rView.y1 + y;

		if (x < (ed->x - gl->su->w*2) ||
		    y < -(gl->su->h) ||
		    dx > (WIDGET(ed)->rView.x2 + gl->su->w) ||
		    dy >  WIDGET(ed)->rView.y2) {
			x += gl->advance;
			AG_TextUnusedGlyph(gl);
			continue;
		}
		if (!agView->opengl) {
			AG_SurfaceBlit(gl->su, NULL, agView->v, dx,dy);
		}
#ifdef HAVE_OPENGL
		else {
			glBindTexture(GL_TEXTURE_2D, gl->texture);
			glBegin(GL_TRIANGLE_STRIP);
			{
				glTexCoord2f(gl->texcoord[0], gl->texcoord[1]);
				glVertex2i(dx, dy);
				glTexCoord2f(gl->texcoord[2], gl->texcoord[1]);
				glVertex2i(dx+gl->su->w, dy);
				glTexCoord2f(gl->texcoord[0], gl->texcoord[3]);
				glVertex2i(dx, dy+gl->su->h);
				glTexCoord2f(gl->texcoord[2], gl->texcoord[3]);
				glVertex2i(dx+gl->su->w, dy+gl->su->h);
			}
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
		}
#endif // HAVE_OPENGL
		x += gl->advance;
		AG_TextUnusedGlyph(gl);
	}
	if (ed->yMax == 1)
		ed->xMax = x;

	if ( !(ed->flags & (AG_EDITABLE_NOSCROLL|AG_EDITABLE_NOSCROLL_ONCE)) ) {
		if (ed->flags & AG_EDITABLE_MULTILINE) {
			if (ed->yCurs < ed->y) {
				ed->y = ed->yCurs;
				if (ed->y < 0) { ed->y = 0; }
			} else if (ed->yCurs > ed->y + ed->yVis - 1) {
				ed->y = ed->yCurs - ed->yVis + 1;
			}
		}
		if (!(ed->flags & AG_EDITABLE_WORDWRAP)) {
			if (ed->xCurs < ed->x) {
				ed->x = ed->xCurs;
				if (ed->x < 0) { ed->x = 0; }
			} else if (ed->xCurs > ed->x + WIDTH(ed) - 10) {
				ed->x = ed->xCurs - WIDTH(ed) + 10;
			}
		}
	} else {
		if (ed->yCurs < ed->y) {
			if (ed->flags & AG_EDITABLE_MULTILINE) {
				AG_EditableMoveCursor(ed,
				    ed->xCursPref - ed->x, 1,
				    0);
			}
		} else if (ed->yCurs > ed->y + ed->yVis - 1) {
			if (ed->flags & AG_EDITABLE_MULTILINE) {
				AG_EditableMoveCursor(ed,
				    ed->xCursPref - ed->x,
				    ed->yVis*agTextFontLineSkip - 1,
				    0);
			}
		} else if (ed->xCurs < ed->x+10) {
			if (!(ed->flags & AG_EDITABLE_WORDWRAP)) {
				AG_EditableMoveCursor(ed,
				    ed->x+10,
				    (ed->yCurs - ed->y)*agTextFontLineSkip + 1,
				    1);
			}
		} else if (ed->xCurs > ed->x+WIDTH(ed)-10) {
			if (!(ed->flags & AG_EDITABLE_WORDWRAP)) {
				AG_EditableMoveCursor(ed,
				    ed->x+WIDTH(ed)-10,
				    (ed->yCurs - ed->y)*agTextFontLineSkip + 1,
				    1);
			}
		}
	}
	ed->flags &= ~(AG_EDITABLE_NOSCROLL_ONCE);
	AG_UnlockVariable(stringb);
	AG_PopTextState();

	AG_PopClipRect();

#ifdef HAVE_OPENGL
	if (agView->opengl) {
		if (blend_sv) {
			glEnable(GL_BLEND);
		} else {
			glDisable(GL_BLEND);
		}
		glBlendFunc(blend_sfactor, blend_dfactor);
	}
#endif
	FreeStringUCS4(ed, ucs);
}
*/

AG_WidgetClass myEditableClass = {
	{
		"AG_Widget:AG_Editable:myEditable",
		sizeof(myEditable),
		{ 0,0 },
		Init,
		NULL,		/* free */
		Destroy,
		NULL,		/* load */
		NULL,		/* save */
		NULL		/* edit */
	},
	AG_WidgetInheritDraw,
	AG_WidgetInheritSizeRequest,
	AG_WidgetInheritSizeAllocate
};
