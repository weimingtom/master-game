#include <windows.h>
#include <iostream>
#include <fstream>
#include <locale>

#include <vector>
#include <list>
#include <map>

#include "rapidxml.hpp"
#include "rapidxml_print.hpp"

using namespace std;
using namespace rapidxml;


struct dialogNode {
    int idNum;
    std::vector < int > children;
    std::string text;
    std::string owner;
};

map<int,dialogNode*> dialog_fun();

