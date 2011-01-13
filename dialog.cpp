#include "dialog.hpp"

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}

std::vector<dialogNode*> convGraph = dialog_fun();
int curNode=0;

//formaText *outwin;
std::vector<char*> currentAnswers()
{
    std::vector<char*> ansList;
    ansList.reserve(10);
    for (int i=0;i<convGraph[curNode]->children.size();i++)
    {
        //printf("%i \n",convGraph[curNode]->children[i]);
        //ansList[i]=(char*)malloc(200);
        char* temp=(char*)malloc(1000);
        strcpy(temp,convGraph[convGraph[curNode]->children[i]]->text.c_str());
        ansList.push_back(temp);
        //printf(ansList[i]);
    }
    return ansList;

}

char*
chooseAnswer( int cn)
{
    //std::string c;

    char* text=(char*)malloc(3000);

    strcpy(text,"");
    //static char b[10000];
    if (convGraph[curNode]->children.size()==0)
    {
        strcat(text,"Разговор окончен");
        strcat(text,"\n");

    } else
    {
        printf("children size: %i \n",convGraph[curNode]->children.size());
        printf("choooseAnswer: cn: %d, curNode:%d \n",cn,curNode);

        //curNode=convGraph[curNode]->children[convGraph[curNode]->children.size()-1-cn];
        curNode=convGraph[curNode]->children[cn];
        //strcat(text,"<p owner=\"");
        //strcat(text,convGraph[curNode]->owner.c_str());
        //strcat(text,"\">");
        strcat(text,convGraph[curNode]->text.c_str());
        //strcat(text,"</p>");
        strcat(text,"\n");

    }
/*        if (convGraph[curNode]->children.size()==0)
        {
            strcpy(text,"");
            strcat(text,"Разговор окончен");
            strcat(text,"\n");

        } else

        if (convGraph[curNode]->owner=="pc"){
            strcpy(text,"");
            curNode=convGraph[curNode]->children[0];
            strcat(text,convGraph[curNode]->text.c_str());
            strcat(text,"\n");
        }

        /*

        if (convGraph[curNode]->owner=="npc"){
            for (int i=0;i<convGraph[curNode]->children.size();i++){
                printf("%d",convGraph[curNode]->children[i]);
                //strcat(text,"<pc>");
                strcat(text,"<p owner=\"");
                strcat(text,convGraph[convGraph[curNode]->children[i]]->owner.c_str());
                strcat(text,"\">");
                strcat(text,convGraph[convGraph[curNode]->children[i]]->text.c_str());
                strcat(text,"</p>");
                //strcat(text,"</pc>");
                strcat(text,"\n");
            }
        }
        */


      //strcpy(b,"");
      //strcat(b,"<html><head><META http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"><link rel=\"stylesheet\" type=\"text/css\" href=\"res\\mysite.css\"></head><body><div#test>");
      //strcat(b,text);
      //strcat(b,"</div#test></body></html>");
      //h1->load(LPCBYTE(b),strlen(b));

      //outwin->curNode=curNode;
      //strcpy(text,convGraph[curNode]->text.c_str());
      printf(text);
      return text;

}
/*
void dialogWindow()
{
	AG_Surface *surf;
	AG_Surface *surf2;
	char *someText;
	size_t size, bufSize;



	dialogWin = AG_WindowNew(0);
	AG_WindowSetCaption(dialogWin, "Диалог");

    AG_HBox* hbox = AG_HBoxNew(dialogWin,0);
    AG_Expand(hbox);

    AG_RegisterClass(&formaTextClass);
    outwin=formaTextNew(hbox);

    //AG_Scrollbar* DialogScrollbar=AG_ScrollbarNew(hbox,AG_SCROLLBAR_VERT,AG_SCROLLBAR_AUTOSIZE);
    AG_Scrollbar* DialogScrollbar=AG_ScrollbarNewInt(hbox,AG_SCROLLBAR_VERT,AG_SCROLLBAR_AUTOSIZE|AG_SCROLLBAR_VFILL, &outwin->scrollPos, &outwin->scrollMin, &outwin->scrollMax, &outwin->scrollVisible);
    AG_SetEvent(DialogScrollbar,"scrollbar-changed",scrollEvent,"%p",outwin);
    AG_Expand(outwin);


    AG_WindowSetGeometryAligned(dialogWin, AG_WINDOW_MC, 320, 240);

    AG_WindowShow(dialogWin);

    chooseAnswer(outwin->h1,outwin->dialogText,0);
    updateSurface(outwin,outwin->_inherit.w,outwin->_inherit.h);


}
*/

std:: vector<dialogNode*> dialog_fun()
{
    //std::locale rus("rus_rus.866");
    //setlocale( LC_ALL,"Russian" );

    //std::locale l(getenv("LANG"));
    //std::locale::global(l);

    //SetConsoleCP(866);
    //SetConsoleOutputCP(866);
    //char cd[_MAX_PATH];
    //getcwd(cd, _MAX_PATH);
    //cout << cd;
    //strcat(cd,".\\assets\\dialog2.xml");

    std::ifstream in(".\\assets\\dialog2.xml");
    if (!in){
        printf("error opening file");
    }
    string s;
    string s1="";

    while(getline(in, s)) { // Discards newline char
    s1=s1+s+"\n";
    }
    //std::cout << s1;


    vector<char> xml_copy(s1.begin(), s1.end());
    xml_copy.push_back('\0');

    rapidxml::xml_document<> doc;    // character type defaults to char
    doc.parse<0>(&xml_copy[0]);    // 0 means default parse flags

    //std::cout << "Name of my first node is: " << doc.first_node()->name() << "\n";

    xml_node<char> *n1;
    xml_node<char> *v;

    std::vector<dialogNode*> conversationGraph;

    conversationGraph.reserve(90);

    n1=doc.first_node("Conversation");

    list < rapidxml::xml_node <char>* > q;
    q.push_back(n1);


    while( !q.empty( ) ){
            v = q.front();
            q.pop_front();      // remove it from the list

            dialogNode *dn=new dialogNode;
            int curId,curNum;
            if (v->first_attribute("idNum")){
                curId=atoi(v->first_attribute("idNum")->value());
            } else {
                curId=0;
            }

            if (v->first_attribute("orderNum")){
                curNum=atoi(v->first_attribute("orderNum")->value());
            } else {
                curNum=0;
            }


            dn->idNum=curId;
            dn->orderNum=curNum;

            if (v->first_attribute("nodeType")){
                dn->owner=std::string(v->first_attribute("nodeType")->value());
            }


            if (v->first_node("conversationText")){
                print(back_inserter(dn->text), *v->first_node("conversationText"),0);
                string str="<conversationText>";
                dn->text.erase(0,str.size());
                dn->text.erase(dn->text.size()-str.size()-2);


            }

            if (v->first_node("conversationComments")){
                string curTags;
                print(back_inserter(curTags), *v->first_node("conversationComments"),0);

             /*   if (!strcmp(&dn->tags[dn->tags.size()-3],"/") )           //нет информации о тегах
                 {
                    dn->tags.clear();
                    cout << "empty";
                }
                    else
                {  */

                    string tagStr="<conversationComments>";
                    curTags.erase(curTags.size()-tagStr.size()-2);
                    curTags.erase(0,tagStr.size());
                    std::vector<std::string> sepTags=split(curTags,' ');
                    dn->tags=std::set<string>(sepTags.begin(),sepTags.end());
                    set<string>::iterator vi;
                    for (vi=dn->tags.begin();vi!=dn->tags.end();vi++)
                        cout << *vi << "\n";
               // }

            }

                               //обход детей

            n1=v->first_node("subNodes")->first_node("ContentNode");


            while (n1)
            {
                q.push_back(n1);
                dn->children.push_back(atoi(n1->first_attribute("idNum")->value()));
                n1=n1->next_sibling();
            }
            conversationGraph[curId]=dn;
     }


    return conversationGraph;
};


dialogNode getConvNode(int n)
{
    return *convGraph[n];
}

int getCurNode()
{
    return curNode;
}
