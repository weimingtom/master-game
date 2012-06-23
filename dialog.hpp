//#include <windows.h>
#ifndef __DIALOG_HPP__
#define __DIALOG_HPP__

#include <vector>
#include <list>
//#include <map>

#include "textUtils.hpp"

#include <set>
#include <algorithm>

#include <iterator>



typedef std::set<std::string>  tags;

using namespace std;
using namespace rapidxml;


struct dialogNode {
    int idNum;
    int orderNum;
    std::vector < int > children;
    std::string text;
    tags precond;
    tags neg_precond;
    tags effMin;
    tags effPlus;
    bool active;
    bool oneTime;

    std::string owner;
};

char* chooseAnswer ( int cn);
std::vector<char*> currentAnswers ();
std::vector<dialogNode*> dialog_fun();
dialogNode getConvNode(int n);
int getCurNode();
void addTags(tags newTags);
void removeTags(tags newTags);
tags getTags();
std::string getTagsAsString();

#endif
