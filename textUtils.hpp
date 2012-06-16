
#ifndef __TEXTUTILS_HPP__
#define __TEXTUTILS_HPP__

#if defined ROCKET_PLATFORM_WIN32
#include <windows.h>
#endif


#include <iostream>
#include <fstream>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

#include <stdio.h>
#include <string.h>

#include <string>
#include <sstream>

#include <sys/types.h>

#if defined __MY_UNIX__
#include <dirent.h>
#endif

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



inline int getDir (std::string dir, std::vector<std::string> &files)
{
#if defined	__MY_UNIX__

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

#endif
#if defined __MY_WIN32__

	WIN32_FIND_DATAA findData;
	//HANDLE h=FindFirstFileA(dir.c_str(), &findData);

	std::string str1=dir;

	str1+="*.*";

	//if(str1.length()-1>=0)
	//  if(str1[str1.length()-1]=='/' || str1[str1.length()-1]=='\\')
	//    str1[str1.length()-1]='\0'; 

	HANDLE h=FindFirstFileA(str1.c_str(), &findData);
	if(h==INVALID_HANDLE_VALUE){
		int err=GetLastError();
		std::cout << "Error(" << err << ") opening " << dir << std::endl;
		return err;
	}
	do{
		//if(findData.dwFileAttributes & FILE_ATTRIBUTE_NORMAL){
		files.push_back(std::string(findData.cFileName));
		//}
	}while(FindNextFileA(h, &findData)!=0);
	FindClose(h);
	return 0;

#endif

}
/*
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
*/
#endif
