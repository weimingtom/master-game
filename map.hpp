
#ifndef MAP_HPP
#define MAP_HPP

#include <Rocket/Core/Types.h>
#include <Rocket/Core/Texture.h>

#include <ShellOpenGL.h>

#include <GL/glu.h>



class Map
{
public:
	Map();
	~Map();

	/// Initialise a new game
	void Initialise();

	/// Update the game
	void Update();

	/// Render the game
	void Render();

private:
	// Texture that contains the sprites
	Rocket::Core::TextureHandle texture;

public:
	int left;
	int top;
	int width;
	int height;

	GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];



};

#endif
