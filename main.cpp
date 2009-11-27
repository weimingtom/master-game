
#include <iostream>

#include <core.h>
#include <gui.h>


#include "myButton.h"

#include "mywidget.h"

#include "dialog.cpp"

#define TESTFONT "CONSOLA.ttf"


using namespace std;

map<int,dialogNode*> convGraph = dialog_fun();
char *a;
wchar_t b;
std::string c;

char *cmdPromptText;
int curNode=0;

std::vector <MyButton*> button;

MyWidget* myWidget;

AG_Window *win;
AG_Window *win2;

void
SayHello(AG_Event *event)
{
    int cn = AG_INT(1);

    std::string c;
    AG_Surface *surf;
	AG_Surface *surf2;

    curNode=convGraph[curNode]->children[cn];
    if (convGraph[curNode]->owner=="npc")
    {
        strcat(a,convGraph[curNode]->text.c_str());
        strcat(a,"\n");
        curNode=convGraph[curNode]->children[0];
    }
    //strcpy(a,"");
    strcat(a,convGraph[curNode]->text.c_str());
    strcat(a,"\n");

	for (int i=0;i<button.size();i++)
	{
        if (i<convGraph[curNode]->children.size())
        {
            c=convGraph[convGraph[curNode]->children[i]]->text.c_str();
            //surf = AG_TextRender(c.c_str());
            //surf2=AG_SurfaceStdRGBA(surf->w*1.5,surf->h*1.5);
            //AG_ButtonSurface(button[i],surf2);
            button[i]->_inherit.lbl=AG_LabelNew(button[i],0,c.c_str());
            //AG_ButtonText((AG_Button*)button[i],c.c_str());
            AG_WidgetUpdate(button[i]);
            AG_WidgetShow(button[i]);
            AG_WidgetEnable(button[i]);
        } else
        {
            AG_WidgetHide(button[i]);
            AG_WidgetDisable(button[i]);
        }


	};

   //AG_LabelJustify(button[1]->lbl, AG_TEXT_CENTER);
   //AG_LabelValign(button[1]->lbl, button[1]->valign);
   //b=wchar_t(c.c_str());


}
static void
SingleLineExample(void)
{
	AG_Window *win;
	AG_Textbox *commandPrompt;
    unsigned int flags =AG_EDITABLE_INT_ONLY;

	win = AG_WindowNew(0);
	/*
	 * Create a single-line Textbox. TextboxSizeHint() requests an initial
	 * textbox size large enough to display the given string entirely.
	 */
	commandPrompt = AG_TextboxNew(win, flags, NULL);
	AG_TextboxSetIntOnly(commandPrompt,1);

    AG_TextboxBindUTF8(commandPrompt, cmdPromptText, sizeof(cmdPromptText));

	AG_TextboxSizeHint(commandPrompt, "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
	AG_WidgetFocus(commandPrompt);

    //AG_Expand(commandPrompt);

	AG_WindowSetPosition(win, AG_WINDOW_LOWER_LEFT, 0);
	AG_WindowShow(win);

	AG_SetEvent(commandPrompt, "textbox-return", SayHello,"1");

}

static void
MultiLineExample(const char *title, int wordwrap,char* a)
{
	AG_Textbox *textbox;
	AG_Surface *surf;
	AG_Surface *surf2;
    AG_VBox *vbox;
	char *someText;
	size_t size, bufSize;
	unsigned int flags = AG_TEXTBOX_MULTILINE|AG_TEXTBOX_CATCH_TAB;

	if (wordwrap) { flags |= AG_TEXTBOX_WORDWRAP; }

	win = AG_WindowNew(0);
	AG_WindowSetCaption(win, title);

    vbox = AG_VBoxNew(win,0);
    AG_Expand(vbox);
	/*
	 * Create a multiline textbox.
	 *
	 * We use the CATCH_TAB flag so that tabs are entered literally in
	 * the string.
	 */

	AG_Font *font;

    if ((font = AG_FetchFont(TESTFONT, 12, 0)) != NULL) {
        AG_SetDefaultFont(font);
    } else {
        AG_TextMsg(AG_MSG_ERROR, "%s: %s",
            TESTFONT, AG_GetError());
    }


	textbox = AG_TextboxNew(vbox, flags, NULL);
	AG_Expand(textbox);


	AG_TextboxBindUTF8(textbox, a, sizeof(a));
    strcat(a,"\n");
    //AG_TextboxSetString(textbox,a);

    //cout <<s2;

	AG_TextboxSetCursorPos(textbox, 0);
    AG_WidgetDisable(textbox);

   //win2 = AG_WindowNew(0);
   //AG_WindowSetCaption(win2, "Варианты ответов");
  AG_RegisterClass(&MyButtonClass);

  AG_RegisterClass(&myWidgetClass);

  //myWidget=MyWidgetNew(vbox,"");

  for (int i=0;i<5;i++){
    button.push_back(MyButtonNew(vbox, ""));
  };

   //button.push_back(AG_ButtonNew(win2, 0, ""));

   //AG_LabelJustify(button[1]->lbl, button[1]->justify);
   //AG_LabelValign(button[1]->lbl, button[1]->valign);
   //b=wchar_t(c.c_str());

   //surf = AG_TextRender(c.c_str());
   //surf2=AG_SurfaceStdRGBA(surf->w*1.5,surf->h*1.5);
   //AG_ButtonSurface(button[1],surf2);
   //button[1]->lbl=AG_LabelNew(button[1],0,c.c_str());
    //AG_LabelJustify(button[1]->lbl, button[1]->justify);
    //AG_LabelValign(button[1]->lbl, button[1]->valign);

    //AG_WindowSetGeometryAligned(win2, AG_WINDOW_MC, 500, 300);

    //AG_Expand(button[0]);
    //AG_Expand(button[1]);

   // AG_WidgetUpdate(button[1]);

    AG_WindowSetGeometryAligned(win, AG_WINDOW_MC, 540, 380);
    AG_WindowShow(win);


//    AG_WindowShow(win2);

}



int main(int argc, char *argv[])
{
	if (AG_InitCore("", 0) == -1) {
		return (1);
	}
	if (AG_InitVideo(640, 480, 32, AG_VIDEO_RESIZABLE) == -1) {
		return (-1);
	}

    /*AG_Font *font;

	if ((font = AG_FetchFont(TESTFONT, 12, 0)) != NULL) {
        AG_SetDefaultFont(font);
    } else {
        AG_TextMsg(AG_MSG_ERROR, "%s: %s",
            TESTFONT, AG_GetError());
    }
    */
	AG_BindGlobalKey(SDLK_ESCAPE, KMOD_NONE, AG_Quit);

    a=new char[1000];
//    b=new Uint32[10];

    cmdPromptText=new char[1000];

    strcpy(a,"");
    //strcpy(cmdPromptText,"1");

	MultiLineExample("Лог", 1,a);

    strcat(a,"\n");
    strcat(a,convGraph[curNode]->text.c_str());
    //AG_TextColorRGB (255, 0, 0);
	/*for (int i=0;i<convGraph[curNode]->children.size();i++)
	{
        strcat(a,"\n");
        strcat(a, convGraph[convGraph[curNode]->children[i]]->text.c_str());
	};
    */
    //SingleLineExample();

	for (int i=0;i<button.size();i++)
	{
        AG_SetEvent(button[i], "button-pushed", SayHello,"%i",i);
	}

	//AG_SetEvent(MyButton, "button-pushed", SayHello,"%i",0);

    AG_PostEvent(button[0],button[0],"button-pushed","%i",0);
	AG_EventLoop();
	AG_Destroy();
	return (0);

}
