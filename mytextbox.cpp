#include <core.h>
#include <gui.h>
#include <agar/gui/widget.h>
#include "mytextbox.h"


#define STYLE(p)                        AGSTYLE(p)
#define WSURFACE(wi,ind)		AGWIDGET_SURFACE((wi),(ind))
#define HEIGHT(p)			AGWIDGET(p)->h
#define WIDTH(p)			AGWIDGET(p)->w

#include <agar/gui/opengl.h>

#define Free(p) AG_Free(p)
#define WIDGET(wi)			AGWIDGET(wi)


static void
BeginScrollbarDrag(AG_Event *event)
{
	AG_myTextbox *tb = (AG_myTextbox*)AG_PTR(1);

	AG_WidgetFocus(tb->_inherit.ed);
	tb->_inherit.ed->flags |= AG_EDITABLE_NOSCROLL;
}

static void
EndScrollbarDrag(AG_Event *event)
{
	AG_myTextbox *tb = (AG_myTextbox*)AG_PTR(1);

	tb->_inherit.ed->flags &= ~(AG_EDITABLE_NOSCROLL);
}


AG_myTextbox *
AG_myTextboxNew(void *parent, Uint flags, const char *label)
{
	AG_myTextbox *tb;

	tb = (AG_myTextbox*)malloc(sizeof(AG_myTextbox));
	AG_ObjectInit(tb, &agMyTextboxClass);

    tb->_inherit.ed->flags |= AG_EDITABLE_MULTILINE|AG_EDITABLE_WORDWRAP;
/*
	if (flags & AG_TEXTBOX_MULTILINE) {
		tb->_inherit.ed->flags |= AG_EDITABLE_MULTILINE;

		tb->_inherit.vBar = AG_ScrollbarNew(tb, AG_SCROLLBAR_VERT, 0);
		AG_BindInt(tb->_inherit.vBar, "value", &tb->_inherit.ed->y);
		AG_BindInt(tb->_inherit.vBar, "max", &tb->_inherit.ed->yMax);
		AG_BindInt(tb->_inherit.vBar, "visible", &tb->_inherit.ed->yVis);
		AG_SetEvent(tb->_inherit.vBar, "scrollbar-drag-begin", BeginScrollbarDrag, "%p", tb);
		AG_SetEvent(tb->_inherit.vBar, "scrollbar-drag-end", EndScrollbarDrag, "%p", tb);

		if (!(flags & AG_TEXTBOX_WORDWRAP)) {
			tb->_inherit.hBar = AG_ScrollbarNew(tb, AG_SCROLLBAR_HORIZ, 0);
			AG_BindInt(tb->_inherit.hBar, "value", &tb->_inherit.ed->x);
			AG_BindInt(tb->_inherit.hBar, "max", &tb->_inherit.ed->xMax);
			AG_BindInt(tb->_inherit.hBar, "visible", &WIDTH(tb->_inherit.ed));
			AG_SetEvent(tb->_inherit.hBar, "scrollbar-drag-begin", BeginScrollbarDrag, "%p", tb);
			AG_SetEvent(tb->_inherit.hBar, "scrollbar-drag-end", EndScrollbarDrag, "%p", tb);
			//AG_TextboxSizeHintLines(tb, 4);
			AG_EditableSizeHintLines(tb->_inherit.ed,4);
		}

	}*/


	AG_ObjectAttach(parent, tb);
	return (tb);
}

/*AG_myTextboxUTF8(AG_Editable *ed, char *buf, size_t bufSize)
{
	AG_ObjectLock(ed);
	AG_BindString(ed, "string", buf, bufSize);
	ed->encoding = AG_ENCODING_UTF8;
	AG_ObjectUnlock(ed);
}
*/

static void
Init(void *obj)
{
	AG_myTextbox *tb = (AG_myTextbox*)obj;

	//(AG_Widget*)(tb)->flags |= AG_WIDGET_TABLE_EMBEDDABLE;
}

static __inline__ void
GetStringUCS4(AG_Editable *ed, const char *s, Uint32 **ucs, size_t *len)
{
	if (ed->flags & AG_EDITABLE_STATIC) {
		if (ed->ucsBuf == NULL) {
			ed->ucsBuf = AG_ImportUnicode(AG_UNICODE_FROM_UTF8,s,0);
			ed->ucsLen = AG_LengthUCS4(ed->ucsBuf);
		}
		*ucs = ed->ucsBuf;
		*len = ed->ucsLen;
	} else {
		*ucs = AG_ImportUnicode(AG_UNICODE_FROM_UTF8, s, 0);
		*len = AG_LengthUCS4(*ucs);
	}
}

static __inline__ void
FreeStringUCS4(AG_Editable *ed, Uint32 *ucs)
{
	if (!(ed->flags & AG_EDITABLE_STATIC))
		Free(ucs);
}

static __inline__ int
WrapAtChar(AG_Editable *ed, int x, Uint32 *s)
{
	AG_Glyph *gl;
	Uint32 *t;
	int x2;

	if (!(ed->flags & AG_EDITABLE_WORDWRAP) ||
	    x == 0 || !isspace((int)*s)) {
		return (0);
	}
	for (t = &s[1], x2 = x;
	     *t != '\0';
	     t++) {
		gl = AG_TextRenderGlyph(*t);
		x2 += gl->advance;
		AG_TextUnusedGlyph(gl);
		if (isspace((int)*t) || *t == '\n') {
			if (x2 > WIDTH(ed)) {
				return (1);
			} else {
				break;
			}
		}
	}
	return (0);
}

static void
Draw(void *p)
{
	AG_myTextbox *tb = (AG_myTextbox*)p;

	STYLE(tb)->TextboxBackground(tb, tb->_inherit.r, (tb->_inherit.flags & AG_TEXTBOX_COMBO));

	if (tb->_inherit.labelText != NULL && tb->_inherit.label == -1) {
		AG_PushTextState();
		AG_TextColor(TEXTBOX_TXT_COLOR);
		tb->_inherit.label = AG_WidgetMapSurface(tb,
		    AG_TextRender(tb->_inherit.labelText));
		AG_PopTextState();
	}

	if (tb->_inherit.label != -1) {
		AG_Surface *lblSu = WSURFACE(tb,tb->_inherit.label);

		AG_PushClipRect(tb, tb->_inherit.rLbl);
		AG_WidgetBlitSurface(tb, tb->_inherit.label,
		    tb->_inherit.lblPadL,
		    HEIGHT(tb)/2 - lblSu->h/2);
		AG_PopClipRect();
	}

	AG_PushClipRect(tb, tb->_inherit.r);

	if (tb->_inherit.flags & AG_TEXTBOX_MULTILINE) {
		int d;

		if (tb->_inherit.vBar != NULL && AG_ScrollbarVisible(tb->_inherit.vBar)) {
			d = WIDTH(tb->_inherit.vBar);
			AG_DrawBox(tb,
			    AG_RECT(WIDTH(tb)-d, HEIGHT(tb)-d, d, d), -1,
			    AG_COLOR(TEXTBOX_COLOR));
		} else if (tb->_inherit.hBar != NULL && AG_ScrollbarVisible(tb->_inherit.hBar)) {
			d = HEIGHT(tb->_inherit.hBar);
			AG_DrawBox(tb,
			    AG_RECT(WIDTH(tb)-d, HEIGHT(tb)-d, d, d), -1,
			    AG_COLOR(TEXTBOX_COLOR));
		}
		AG_WidgetUpdate(tb);
	}
	//AG_WidgetDraw(tb->_inherit.ed);
	if (tb->_inherit.hBar != NULL) { AG_WidgetDraw(tb->_inherit.hBar); }
	if (tb->_inherit.vBar != NULL) { AG_WidgetDraw(tb->_inherit.vBar); }

	AG_PopClipRect();

	AG_Editable *ed = tb->_inherit.ed;
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
	//AG_TextColor(TEXTBOX_TXT_COLOR);
	AG_TextColorRGB(0,0,0);
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
			ed->xMax = max(ed->xMax, x);
			ed->yMax++;
			x = 0;
		}
		if (c == '\n') {
			y += agTextFontLineSkip;
			ed->xMax = max(ed->xMax, x+10);
			ed->yMax++;
			x = 0;
			continue;
		} else if (c == '\t') {
			x += agTextTabWidth;
			continue;
		}
		c = (ed->flags & AG_EDITABLE_PASSWORD) ? '*' : c;
        if (c==60){
            AG_TextColorRGB(255,255,255);
        }
        if (c==62){
            AG_TextColorRGB(0,0,0);
        }

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



AG_WidgetClass agMyTextboxClass = {
	{
		"AG_Widget:AG_Textbox:AG_myTextbox",
		sizeof(AG_myTextbox),
		{ 0,0 },
		Init,
		NULL,		/* free */
		NULL,
		NULL,		/* load */
		NULL,		/* save */
		NULL		/* edit */
	},
	//AG_WidgetInheritDraw,
	Draw,
	AG_WidgetInheritSizeRequest,
	AG_WidgetInheritSizeAllocate
};
