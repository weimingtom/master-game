
#include <iostream>
#include <sstream>

#include <fstream>

#include <core.h>
#include <gui.h>
#include <agar/vg.h>


#include "myButton.h"

#include "mywidget.h"

#include "mytextbox.h"

#include "dialog.cpp"

#include "htmlite.h"

#include "htmlayout_dom.hpp"

#include "geomap.hpp"

#define TESTFONT "CONSOLA.ttf"

#include "events.hpp";

#include "globals.hpp"

#include "task.hpp"

using namespace std;

using namespace htmlayout;

map<int,dialogNode*> convGraph = dialog_fun();
char *a;
char *b;

char *cmdPromptText;
int curNode=0;

std::vector <MyButton*> button;

MyWidget* myWidget;

AG_myTextbox* tb2;
AG_Textbox *tb;

AG_Window *win;
AG_Window *win2;
AG_Window *win3;

AG_VBox *vbox;
AG_VBox *vbox2;

AG_Surface *surf1;
AG_Pixmap *pm1;

HTMLite h1;
BITMAPINFO bmpInfo;
HBITMAP hbmp;
unsigned int pixels2[300][800];

template <typename PARAM>
Uint32 timerFunc(void *obj, Uint32 ival, void *arg)
{
    static Slot<PARAM> b1=*(Slot<PARAM>*)arg;
    b1(ival);
    return 1;
};


void textResize(AG_Event *event)
{

  int ww = AG_INT(3);

  int w1,he1;

  w1=800;
  he1=300;

  HELEMENT phe;
  phe=h1.getRootElement();
  dom::element div = dom::element(phe).find_first("div#test");


  string search;
  stringstream s;
  //s<<pm1->pre_w;
  s<<min(ww-15,785);
  s<<"px";
  search=s.str();
    const size_t newsize = 100;
    size_t origsize = strlen(search.c_str()) + 1;
    size_t convertedChars = 0;
    wchar_t wcstring[newsize];
    mbstowcs( wcstring, search.c_str(), newsize);
    div.set_style_attribute("width",wcstring);

  HTMLayoutUpdateElementEx(div, REDRAW_NOW);

  POINT scrollPos;
  RECT viewRect;
  SIZE contentSize;
  HTMLayoutGetScrollInfo(phe, &scrollPos, &viewRect,&contentSize );
  POINT pt; pt.x = 0;
  pt.y = max((long int)0,contentSize.cy-300);
  //pt.y=38;

  HTMLayoutSetScrollPos(phe,pt,FALSE);

  h1.measure(w1,he1);

  DeleteObject(hbmp);

  void* pixelbitmap=0;

  hbmp =CreateDIBSection(NULL,(BITMAPINFO*)&bmpInfo,DIB_RGB_COLORS, &pixelbitmap,NULL,0);
  h1.render(hbmp,0,0,w1,he1);

  unsigned int pixels[he1][w1];
  //unsigned int pixels2[he1][w1];
  memcpy(pixels,pixelbitmap,sizeof(pixels));

for (int j=0;j<=he1-1;j++)
  for (int i=0;i<=w1-1;i++){
      {
          pixels2[j][i]=pixels[he1-1-j][i];
      }
   }


  //surf1=AG_SurfaceFromPixelsRGB(pixels2,w1,he1,32,(w1)*4,0x00ff0000,0x0000ff00,0xff000000);
  AG_PixmapUpdateCurrentSurface(pm1);
  delete [] pixels;
  delete [] pixelbitmap ;
  //delete &w1;
  //delete &he1;
  //delete [] &pt;

}

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
        strcat(a,"<p#npc>");
        strcat(a,convGraph[curNode]->text.c_str());
        strcat(a,"</p#npc>");
        strcat(a,"\n");
        curNode=convGraph[curNode]->children[0];
    }
    //strcpy(a,"");
    strcat(a,"<p>");
    strcat(a,convGraph[curNode]->text.c_str());
    strcat(a,"</p>");
    strcat(a,"\n");
  strcpy(b,"");
  //memset(b1,' ',1000);
  strcat(b,"<html><head><META http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"><link rel=\"stylesheet\" type=\"text/css\" href=\"mysite.css\"></head><body><div#test>");
  strcat(b,a);
  strcat(b,"</div#test></body></html>");
  h1.load(LPCBYTE(b),strlen(b));

  AG_SizeReq alloc;
  AG_WidgetSizeReq(win,&alloc);
  //AG_PostEvent(win,win, "window-user-resize", "%i%i%i%i",alloc.w,alloc.h,alloc.w-15,alloc.h);
  AG_Event ev;
  AG_EventArgs(&ev,"%i%i%i%i",0,0,win->wid.w,win->wid.h);
  textResize(&ev);
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
   // h1.load((LPCBYTE)a,(DWORD)sizeof(a));
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

	win2 = AG_WindowNew(0);
	AG_WindowSetCaption(win2, "Карта");


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

  w1=800;
  he1=300;



  //HTMLITE h1=HTMLiteCreateInstance();

  //LPCWSTR htmlpath = L"C:\\Agar\\hellocpp2\\bin\\Debug\\a2.html";
  //LPCWSTR htmlpath = L"a1.html";
  //HLTRESULT hr=HTMLiteLoadHtmlFromFile(h1,htmlpath);
  //h1.load(htmlpath);
  b=new char[1000];
  strcpy(b,"");
  //memset(b1,' ',1000);
  strcat(b,"<html><head><link rel=\"stylesheet\" type=\"text/css\" href=\"mysite.css\"></head><body><div#test>");
  strcat(b,a);
  strcat(b,"</div#test></body></html>");
  //std::cout << b1;
  h1.load(LPCBYTE(b),strlen(b));
  //HLTRESULT hr=HTMLiteLoadHtmlFromMemory()
  //hr=HTMLiteMeasure(h1,w1,he1);
  h1.measure(w1,he1);

  //hr=HTMLiteGetDocumentMinWidth(h1,&w1);
  //hr=HTMLiteGetDocumentMinHeight(h1,&he1);

  HELEMENT phe;

  //hr=HTMLiteGetRootElement(h1, phe);
  phe=h1.getRootElement();


  dom::element div = dom::element(phe).find_first("div#test");
  //dom::element div = dom::element(body).find_first("div");
  //div.set_attribute("style",L"background-color:#000000");
  //const wchar_t* res1=div.get_style_attribute("width");
  //div.set_style_attribute("width",0);
  //
  div.set_style_attribute("background-color",L"#000000");
  //div.set_style_attribute("width",L"100px");
  //div.update(true);

  HTMLayoutUpdateElementEx(div, REDRAW_NOW);

  POINT pt; pt.x = 0; pt.y = 0;

  HTMLayoutSetScrollPos(phe,pt,FALSE);


  //hr=HTMLiteMeasure(h1,w1,he1);
  h1.measure(w1,he1);
  //HTMLayoutGetStyleAttribute(HELEMENT he, LPCSTR name, LPCWSTR* p_value);
  //HTMLayoutSetAttributeByName(*phe, , LPCWSTR value);
  //he1=min(he1,300);

  //he1=100;
  //w1=100;

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

  //BITMAPINFO bmpInfo;
  memset(&bmpInfo,0,sizeof(BITMAPINFO));
  bmpInfo.bmiHeader = bmpInfoHeader;

  void* pixelbitmap=0;
/*
      unsigned int * pixelbitmap2 = (unsigned int *)pixelbitmap;
      for( int n = 0; n < w1*he1; ++n)
        *pixelbitmap2++ = 0xffffffff;
*/


  hbmp =CreateDIBSection(NULL,(BITMAPINFO*)&bmpInfo,DIB_RGB_COLORS, &pixelbitmap,NULL,0);

  h1.render(hbmp,0,0,w1,he1);

  /*hr=HTMLiteRenderOnBitmap(h1,hbmp,
          0,    // x position of area to render
          0,    // y position of area to render
          w1,   // width of area to render
          he1);  // height of area to render
*/
/*
  BITMAP bitmap;
  int res=GetObject(hbmp,sizeof(BITMAP),(LPSTR)&bitmap);
*/
  unsigned int pixels[he1][w1];
  //unsigned int pixels2[he1][w1];

  memcpy(pixels,pixelbitmap,sizeof(pixels));

for (int j=0;j<=he1-1;j++)
  for (int i=0;i<=w1-1;i++){
      {
          //pixels2[j][i]=pixels[w1-j-1][i];
          //pixels2[i][j]=pixels[i][j];
          pixels2[j][i]=pixels[he1-1-j][i];
          //pixels2[i][j]=pixels[i][he1-1-j];
      }
   }



  ofstream myfile;
  myfile.open("pixelbitmap");
  myfile.write((char*)pixelbitmap,w1*he1*4);
  myfile.close();

  //surf1=AG_SurfaceFromPixelsRGBA(pixels2,w1,he1,32,w1*4,0x000000ff,0x00ff0000,0x0000ff00,0xff000000);
  surf1=AG_SurfaceFromPixelsRGB(pixels2,w1,he1,32,(w1)*4,0x00ff0000,0x0000ff00,0xff000000);

  pm1=AG_PixmapFromSurface(vbox,0,surf1);
  //AG_Expand(pm1);

  AG_RegisterClass(&agMyTextboxClass);


  //tb = AG_TextboxNew(vbox, flags, NULL);

  /*tb2 = AG_myTextboxNew(vbox, flags, NULL);

  AG_Expand(tb2);

  //AG_TextboxBindUTF8((*tb2)._inherit, a, sizeof(a));
  AG_EditableBindUTF8((tb2)->_inherit.ed,a,sizeof(a));
  */
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

    AG_WindowSetGeometryAligned(win, AG_WINDOW_MC, 640, 480);

    AG_WindowShow(win);



    AG_WindowSetGeometryAligned(win2, AG_WINDOW_MC, 640, 480);


    AG_WindowShow(win2);

    mapdraw();


	win3 = AG_WindowNew(0);

    AG_WindowSetGeometryAligned(win3, AG_WINDOW_MC, 640, 480);

	AG_WindowSetCaption(win3, "Карта");


	AG_Box *hb = AG_BoxNewHoriz(win3, 0);
	AG_Expand(hb);

	AG_GLView *glv;
	glv = AG_GLViewNew(hb, 0);
    AG_Expand(glv);
	AG_WidgetFocus(glv);

    AG_GLViewDrawFn(glv, MapDrawFunction,NULL);

    AG_GLViewScaleFn(glv, MapScaleFunction, NULL);

    AG_WindowShow(win3);
}



int main(int argc, char *argv[])
{

	if (AG_InitCore("", 0) == -1) {
		return (1);
	}
	if (AG_InitVideo(1024, 768, 32, AG_VIDEO_OPENGL|AG_VIDEO_RESIZABLE) == -1) {
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

    strcat(a,"\n");
    strcat(a,convGraph[curNode]->text.c_str());

	MultiLineExample("Лог", 1,a);


	for (int i=0;i<button.size();i++)
	{
        AG_SetEvent(button[i], "button-pushed", SayHello,"%i",i);
	}

	AG_SetEvent(win, "window-user-resize", textResize,"%i%i");

    AG_PostEvent(button[0],button[0],"button-pushed","%i",0);

    //UpdateTimerSlot.add(guest1);

    goToPoint order1=goToPoint(400,400,&guest1);

    UpdateTimerSlot.addTask<goToPoint>(order1);

    AG_Timeout *TO = new AG_Timeout;

    AG_SetTimeout(TO, timerFunc<int>, &UpdateTimerSlot, 0);

    AG_ScheduleTimeout(NULL, TO, 100);

	AG_EventLoop();
	AG_Destroy();
	return (0);

}
