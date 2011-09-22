
#ifndef __TEXTUTILS_HPP__
#define __TEXTUTILS_HPP__

#include <iostream>
#include <fstream>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include<windows.h>
#include <stdio.h>
#include <string.h>

#include <string>
#include <sstream>

#include <sys/types.h>
//#include <dirent.h>
#include <errno.h>
#include <vector>
#include <iostream>


typedef rapidxml::xml_document<> xmlFile;




inline std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


inline std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}


inline std::string replace(std::string text, std::string s, std::string d)
{
  for(unsigned index=0; index=text.find(s, index), index!=std::string::npos;)
  {
    text.replace(index, s.length(), d);
    index+=d.length();
  }
  return text;
}


/*
inline int getDir (std::string dir, std::vector<std::string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp = opendir(dir.c_str())) == NULL) {
        std::cout << "Error(" << errno << ") opening " << dir << std::endl;

        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(std::string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}
*/
inline int getDir (std::string dir, std::vector<std::string> &files){
	WIN32_FIND_DATAA findData;
	HANDLE h=FindFirstFileA(dir.c_str(), &findData);
	if(h==INVALID_HANDLE_VALUE){
		int err=GetLastError();
		std::cout << "Error(" << err << ") opening " << dir << std::endl;
		return err;
	}
	do{
		files.push_back(std::string(findData.cFileName));
	}while(FindNextFileA(h, &findData)!=0);
	FindClose(h);
	return 0;
}

#endif
