#include "config.h"

// Set Resolution from config.ini
void setResolution()
{
    // load resolution config
    char* optString;
    std::ifstream in("config.ini");

    if(in)
    {
        optString=new char[256];
        in.getline(optString,256);

        std::vector<std::string> name_value = split(std::string(optString),'=');
        fullscreen = atoi(name_value[1].c_str());

        delete []optString;

        optString=new char[256];
        in.getline(optString,256);

        name_value = split(std::string(optString),'=');
        screen_width = atoi(name_value[1].c_str());

        delete []optString;

        optString=new char[256];
        in.getline(optString,256);

        name_value = split(std::string(optString),'=');
        screen_height = atoi(name_value[1].c_str());

        delete []optString;
    }
    else
    {
        screen_width = 800;
        screen_height = 600;
        printf("\nWarning! Cannot find config.ini. Resolution is 800x600\n\n");
    }
}
