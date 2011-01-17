//#include <windows.h>
#ifndef __DIALOG_HPP__
#define __DIALOG_HPP__

#include <iostream>
#include <fstream>

#include <vector>
#include <list>
//#include <map>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

#include <stdio.h>
#include <string.h>

#include <string>
#include <sstream>

#include <set>
#include <algorithm>


typedef std::set<std::string> tags;

using namespace std;
using namespace rapidxml;


struct dialogNode {
    int idNum;
    int orderNum;
    std::vector < int > children;
    std::string text;
    tags precond;
    tags effMin;
    tags effPlus;

    std::string owner;
};

char* chooseAnswer ( int cn);
std::vector<char*> currentAnswers ();
std::vector<dialogNode*> dialog_fun();
dialogNode getConvNode(int n);
int getCurNode();
void addTags(tags newTags);
tags getTags();
std::string getTagsAsString();

#endif
