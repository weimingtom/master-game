#ifndef __SPRITESHPP__
#define __SPRITESHPP__


#include "GL/gl.h"
#include <Rocket/Core/Texture.h>

struct Sprite
{
    int left;
    int top;
    int bottom;
    int right;

    int textureHandle;
    Rocket::Core::Vector2i texture_dimensions;
};


extern std::map<int,Sprite*> spriteTable;
#endif
