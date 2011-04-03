
#ifndef MAP_HPP
#define MAP_HPP

#include <Rocket/Core/Types.h>
#include <Rocket/Core/Texture.h>

#include <ShellOpenGL.h>

#include <GL/glu.h>

#include "textUtils.hpp"

#include "micropather.h"

#include "events.hpp"

#include "globals.hpp"


class Map : public micropather::Graph{
public:
	Map();
	~Map();

	/// Initialise a new game
	void Initialise();

	/// Finalise a new game
	void Finalise();

	/// Update the game
	void Update();

	/// Render the game
	void Render();

	/// micropather's

    virtual float LeastCostEstimate( void* nodeStart, void* nodeEnd );

	virtual void AdjacentCost( void* node, std::vector< micropather::StateCost > *neighbors );

	virtual void PrintStateInfo( void* node );

    void NodeToXY( void* node, short* x, short* y);

    void* XYToNode( short x, short y );

    int Passable( short nx, short ny );

    ComplexTask* planPath(short x, short y);


private:
	// Texture that contains the sprites
	Rocket::Core::TextureHandle texture;

    Rocket::Core::Time update_start;
	// How often the invaders move
	Rocket::Core::Time update_freq;



public:
	int left;
	int top;
	int width;
	int height;

	int cursorX;
	int cursorY;

    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    std::set<vertex_tuple> walls;

    rapidxml::xml_document<> doc;
    std::vector<char> xml_copy;

    Entity guest1;

    Slot<Rocket::Core::Time> UpdateTimerSlot;

    //int mapMode;




};

#endif
