
#include <iostream>
#include <fstream>

#include <core.h>
#include <gui.h>


#include "myButton.h"

#include "mywidget.h"

#include "mytextbox.h"

#include "dialog.cpp"

#include "htmlite.h"

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

AG_myTextbox* tb2;
AG_Textbox *tb;

AG_Window *win;
AG_Window *win2;

AG_VBox *vbox;
AG_VBox *vbox2;

AG_Surface *surf1;
AG_Pixmap *pm1;
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
        strcat(a,"<");
        strcat(a,convGraph[curNode]->text.c_str());
        strcat(a,">");
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
MultiLineExample(const char *title, int wordwrap,char* a)
{
	AG_Surface *surf;
	AG_Surface *surf2;
	char *someText;
	size_t size, bufSize;
	unsigned int flags = AG_TEXTBOX_MULTILINE|AG_TEXTBOX_CATCH_TAB;

	if (wordwrap) { flags |= AG_TEXTBOX_WORDWRAP; }

	win = AG_WindowNew(0);
	AG_WindowSetCaption(win, title);

    vbox = AG_VBoxNew(win,0);
    AG_Expand(vbox);

	AG_Font *font;

    if ((font = AG_FetchFont(TESTFONT, 12, 0)) != NULL) {
        AG_SetDefaultFont(font);
    } else {
        AG_TextMsg(AG_MSG_ERROR, "%s: %s",
            TESTFONT, AG_GetError());
    }


    //textbox = AG_TextboxNew(vbox, flags, NULL);
	//AG_Expand(textbox);



	//AG_TextboxBindUTF8(textbox, a, sizeof(a));
    //strcat(a,"\n");
    //AG_TextboxSetString(textbox,a);

    //cout <<s2;

	//AG_TextboxSetCursorPos(textbox, 0);
    //AG_WidgetDisable(textbox);

   //win2 = AG_WindowNew(0);
   //AG_WindowSetCaption(win2, "Варианты ответов");

int w1,he1;

  w1=100;
  he1=100;

  HTMLITE h1=HTMLiteCreateInstance();
  LPCWSTR htmlpath = L"C:\\cpp\\HTMLayout\\html_samples\\a1.html";
  HLTRESULT hr=HTMLiteLoadHtmlFromFile(h1,htmlpath);

  hr=HTMLiteMeasure(h1,w1,he1);

  BITMAPINFOHEADER bmpInfoHeader;
  memset(&bmpInfoHeader,0,sizeof(BITMAPINFOHEADER));
  bmpInfoHeader.biSize=sizeof(BITMAPINFOHEADER);
  bmpInfoHeader.biPlanes = 1;
  bmpInfoHeader.biBitCount=32;
  bmpInfoHeader.biCompression = BI_RGB;
  bmpInfoHeader.biWidth=w1;
  bmpInfoHeader.biHeight=he1;
      bmpInfoHeader.biSizeImage = 0;
      bmpInfoHeader.biXPelsPerMeter = 0;
      bmpInfoHeader.biYPelsPerMeter = 0;
      bmpInfoHeader.biClrUsed = 0;
      bmpInfoHeader.biClrImportant = 0;

  BITMAPINFO bmpInfo;
  memset(&bmpInfo,0,sizeof(BITMAPINFO));
  bmpInfo.bmiHeader = bmpInfoHeader;

  void* pixelbitmap=0;

  HBITMAP hbmp =CreateDIBSection(NULL,(BITMAPINFO*)&bmpInfo,DIB_RGB_COLORS, &pixelbitmap,NULL,0);

/*      unsigned int * pixelbitmap2 = (unsigned int *)pixelbitmap;
      for( int n = 0; n < w1*he1; ++n)
        *pixelbitmap2++ = 0xffffffff;
*/

  hr=HTMLiteRenderOnBitmap(h1,hbmp,
          0,    // x position of area to render
          0,    // y position of area to render
          w1,   // width of area to render
          he1);  // height of area to render
/*
  BITMAP bitmap;
  int res=GetObject(hbmp,sizeof(BITMAP),(LPSTR)&bitmap);
*/

  unsigned int pixels[w1][he1];
  unsigned int pixels2[w1][he1];

  memcpy(pixels,pixelbitmap,sizeof(pixels));

  for (int i=0;i<=w1-1;i++){
      for (int j=0;j<=he1-1;j++){
          pixels2[i][j]=pixels[he1-i-1][j];
      }
   }

  ofstream myfile;
  myfile.open("pixelbitmap");
  myfile.write((char*)pixelbitmap,w1*he1*4);
  myfile.close();

  //hr=HTMLiteRender(h1,CDC,0,0,w1,he1);

  //GetDIBits(CDC,hbmp,0,(WORD)bmpInfoHeader.biHeight,pixelbitmap,&bmpInfo,DIB_RGB_COLORS);
  //GetBitmapBits(hbmp,(WORD)bmpInfoHeader.biHeight*bmpInfoHeader.biWidth*4,pixelbitmap);

  surf1=AG_SurfaceFromPixelsRGB(pixels2,w1,he1,32,w1*4,0x00ff0000,0x0000ff00,0xff000000);
  pm1=AG_PixmapFromSurface(win,0,surf1);

  AG_RegisterClass(&agMyTextboxClass);


  tb = AG_TextboxNew(vbox, flags, NULL);

  tb2 = AG_myTextboxNew(vbox, flags, NULL);

  AG_Expand(tb2);

  //AG_TextboxBindUTF8((*tb2)._inherit, a, sizeof(a));
  AG_EditableBindUTF8((tb2)->_inherit.ed,a,sizeof(a));
  strcat(a,"\n");
    //AG_TextboxSetString(textbox,a);

    //cout <<s2;

	//AG_TextboxSetCursorPos(tb2, 0);
    //AG_WidgetDisable(tb2);


  AG_RegisterClass(&MyButtonClass);

  AG_RegisterClass(&myWidgetClass);



  //myWidget=MyWidgetNew(vbox,"");

  for (int i=0;i<5;i++){
    button.push_back(MyButtonNew(vbox, ""));
  };

    AG_WindowSetGeometryAligned(win, AG_WINDOW_MC, 540, 380);
    AG_SchedEvent(NULL,win,100,"window-user-resize","%i,%i",600,600);

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
