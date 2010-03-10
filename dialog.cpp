#include "dialog.h"
#include <direct.h>


 map<int,dialogNode*> dialog_fun()
{
    //std::locale rus("rus_rus.866");
    //setlocale( LC_ALL,"Russian" );

    //std::locale l(getenv("LANG"));
    //std::locale::global(l);

    //SetConsoleCP(866);
    //SetConsoleOutputCP(866);
    char cd[_MAX_PATH];
    getcwd(cd, _MAX_PATH);
    //cout << cd;
    strcat(cd,"\\dialog2.xml");
    ifstream in(cd);
    //ifstream in("C:\\Agar\\hellocpp2\\bin\\Debug\\dialog2.xml");
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

    dialogNode *dn;

    map<int,dialogNode*> conversationGraph;

    n1=doc.first_node("Conversation");

    list < rapidxml::xml_node <char>* > q;
    q.push_back(n1);


    while( !q.empty( ) ){
            v = q.front( );
            q.pop_front( );      // remove it from the list

            dn=new dialogNode;
            int curId;
            if (v->first_attribute("idNum")){
                curId=atoi(v->first_attribute("idNum")->value());
            } else {
                curId=0;
            }

            dn->idNum=curId;

            if (v->first_node("conversationText")){
                print(back_inserter(dn->text), *v->first_node("conversationText"),0);
                string str="<conversationText>";
                dn->text.erase(0,str.size());
                dn->text.erase(dn->text.size()-str.size()-2,dn->text.size());
            }                   //обход детей

            n1=v->first_node("subNodes")->first_node("ContentNode");


            while (n1){
                //cout << n1->name()<<"\n";
                //cout << n1->first_attribute("idNum")->value()<<"\n";
                q.push_back(n1);
                dn->owner=std::string(n1->first_attribute("nodeType")->value());
                dn->children.push_back(atoi(n1->first_attribute("idNum")->value()));
                n1=n1->next_sibling();
            }
            conversationGraph[curId]=dn;
     }


    int curState=0;
        int i=1;
        for (vector<int>::iterator it = conversationGraph[curState]->children.begin(); it!=conversationGraph[curState]->children.end(); ++it)
        {
            cout << i << ":" <<conversationGraph[*it]->text <<"\n";

            i++;
        }
    return conversationGraph;
};
