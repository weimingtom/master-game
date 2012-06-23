#include "dialog.hpp"



std::vector<dialogNode*> convGraph = dialog_fun();
int curNode=999;
tags dialogState;


std::vector<char*> currentAnswers()
{
    std::vector<char*> ansList;
    ansList.reserve(10);
	//ansList.resize(10);
    for (int i=0;i<convGraph[curNode]->children.size();i++)
    {


        char* temp=(char*)malloc(1000);
        strcpy(temp,convGraph[convGraph[curNode]->children[i]]->text.c_str());
        ansList.push_back(temp);
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
    if ((convGraph[curNode]->children.size()==0))
    {
        strcat(text,"Разговор окончен");
        strcat(text,"\n");

    } else
    {
        printf("children size: %i \n",convGraph[curNode]->children.size());
        printf("choooseAnswer: cn: %d, curNode:%d \n",cn,curNode);

        if (convGraph[curNode]->oneTime)
            convGraph[curNode]->active=false;

        curNode=convGraph[curNode]->children[cn];
        //strcat(text,"<p owner=\"");
        //strcat(text,convGraph[curNode]->owner.c_str());
        //strcat(text,"\">");
        strcat(text,convGraph[curNode]->text.c_str());
        //strcat(text,"</p>");
        strcat(text,"\n");

    }
      printf(text);
      return text;

}

std:: vector<dialogNode*> dialog_fun()
{

    std::ifstream in("./assets/dialog2.xml");
    if (!in){
        printf("error opening file");
    }
    string s;
    string s1="";

    while(getline(in, s)) { // Discards newline char
    s1=s1+s+"\n";
    }



    s1=replace(s1,"&gt;",">");
    s1=replace(s1,"&lt;","<");
    //s1=replace(s1,"&quot;",'o');
    s1=replace(s1,"&quot;"," \" ");

    //std::cout << s1;


    vector<char> xml_copy(s1.begin(), s1.end());
    xml_copy.push_back('\0');

    rapidxml::xml_document<> doc;    // character type defaults to char
    doc.parse<0>(&xml_copy[0]);    // 0 means default parse flags

    //std::cout << "Name of my first node is: " << doc.first_node()->name() << "\n";

    xml_node<char> *n1;
    xml_node<char> *v;

    std::vector<dialogNode*> conversationGraph;

    //conversationGraph.reserve(1000);
	conversationGraph.resize(1000);

    n1=doc.first_node("Conversation");

    list < rapidxml::xml_node <char>* > q;
    q.push_back(n1);


    while( !q.empty( ) ){
            v = q.front();
            q.pop_front();      // remove it from the list

            dialogNode *dn=new dialogNode;
            dn->active=true;
            dn->oneTime=false;

            int curId,curNum;
            if (v->first_attribute("idNum")){
                curId=atoi(v->first_attribute("idNum")->value());
            } else {
                curId=999;               //TODO:придумать раздачу незанятых номеров
            }


            if (curId==0)           //пустая реплика со ссылкой
            {
                cout<<"\n" <<"node 0"<<"\n";
                if (v->parent()->parent()->first_attribute("idNum"))

                {

                    int cind=atoi(v->parent()->parent()->first_attribute("idNum")->value());

                    cout <<"\n" << "parent node of link:" << cind <<"\n";

                    std::vector<int> *cc = &conversationGraph[cind]->children;
                    std::vector<int>::iterator ci;
                    ci = find (cc->begin(), cc->end(), curId);
                    cc->erase(ci);

                    int clink=atoi(v->first_attribute("linkTo")->value());

                    cc->push_back(clink);


                }
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
                    if (dn->text.size()>str.size()*2)
                    {

                        dn->text.erase(0,str.size());
                        dn->text.erase(dn->text.size()-str.size()-2);
                    }

            dn->text=replace(dn->text,"&quot;","\"");


            }

            if (v->first_node("conversationComments")){

                if (v->first_node("conversationComments")->first_node("single"))
                {
                    dn->oneTime=true;
                }

                if (v->first_node("conversationComments")->first_node("pre"))
                {
                    string curTags;
                    print(back_inserter(curTags), *v->first_node("conversationComments")->first_node("pre"),0);

                    string tagStr="<pre>";
                    curTags.erase(curTags.size()-tagStr.size()-2);
                    curTags.erase(0,tagStr.size());
                    std::vector<std::string> sepTags=split(curTags,' ');
                    dn->precond=std::set<string>(sepTags.begin(),sepTags.end());
                }


                if (v->first_node("conversationComments")->first_node("Npre"))
                {
                    string curTags;
                    print(back_inserter(curTags), *v->first_node("conversationComments")->first_node("Npre"),0);

                    string tagStr="<Npre>";
                    curTags.erase(curTags.size()-tagStr.size()-2);
                    curTags.erase(0,tagStr.size());
                    std::vector<std::string> sepTags=split(curTags,' ');
                    dn->neg_precond=std::set<string>(sepTags.begin(),sepTags.end());
                }



                    if (v->first_node("conversationComments")->first_node("ep"))
                {

                    string curTags;
                    print(back_inserter(curTags), *v->first_node("conversationComments")->first_node("ep"),0);


                    string tagStr="<ep>";
                    curTags.erase(curTags.size()-tagStr.size()-2);
                    curTags.erase(0,tagStr.size());
                    std::vector<std::string> sepTags=split(curTags,' ');
                    dn->effPlus=std::set<string>(sepTags.begin(),sepTags.end());

                }

                    if (v->first_node("conversationComments")->first_node("em"))
                {
                    string curTags;
                    print(back_inserter(curTags), *v->first_node("conversationComments")->first_node("em"),0);


                    string tagStr="<em>";
                    curTags.erase(curTags.size()-tagStr.size()-2);
                    curTags.erase(0,tagStr.size());
                    std::vector<std::string> sepTags=split(curTags,' ');
                    dn->effMin=std::set<string>(sepTags.begin(),sepTags.end());
                }
            }

            n1=v->first_node("subNodes")->first_node("ContentNode");


            while (n1)
            {
                q.push_back(n1);
                dn->children.push_back(atoi(n1->first_attribute("idNum")->value()));
                n1=n1->next_sibling();
            }

            if (curId==999)          //начальное значение текущей реплики
            {
                dialogNode *dn1=new dialogNode;
                dn1->children.push_back(1); //пустая реплика в начале, нужна для правильного чередования pc/npc. Ссылается на 1ую содержательную реплику.
                conversationGraph[998]=dn1;
                dn->children[0]=998;

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


tags getTags()
{
    return dialogState;
}

std::string getTagsAsString()
{
    tags::iterator ti;
    std::string result;

    for (ti=dialogState.begin();ti!=dialogState.end();ti++)
        result=result+" "+*ti;
    return result;
}



void addTags(tags newTags)
{
    dialogState.insert(newTags.begin(),newTags.end());
}

void removeTags(tags newTags)
{
    tags result;
    tags::iterator ti;
    for (ti=newTags.begin();ti!=newTags.end();ti++)
    {
        dialogState.erase(*ti);
    }

    //ti=set_difference(dialogState.begin(),dialogState.end(),newTags.begin(),newTags.end(),result.begin());
    //dialogState.remove(newTags.begin(),newTags.end());
}

