
#ifndef MAP_HPP
#define MAP_HPP

#include <Rocket/Core/Types.h>
#include <Rocket/Core/Texture.h>

#include <ShellOpenGL.h>

#include <GL/glu.h>

#include "textUtils.hpp"

#include "micropather.h"

#include "events.hpp"


#define CANALISATION 1
#define MAP 2
#define ELECTRICITY 3
#define TUBE 1
#define VALVE 2
#define WALL 1
#define RES_GEN_START 2
#define GEN 3
#define WATER_GEN 4
#define CONTROL 5
#define PUMP 6
#define LIGHT 7
#define OUTER_LINK 8
#define PATIENT 9
#define ALARM_MAGISTRAL 1
#define RESERVE_MAGISTRAL 2
#define DIV_CORB 3
#define CORB 4

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
        void Render(int mode, int element_type);

        /// micropather's

        virtual float LeastCostEstimate( void* nodeStart, void* nodeEnd );

        virtual void AdjacentCost( void* node, std::vector< micropather::StateCost > *neighbors );

        virtual void PrintStateInfo( void* node );

        void NodeToXY( void* node, short* x, short* y);

        void* XYToNode( short x, short y );

        int Passable( short nx, short ny );

        ComplexTask* planPath(short x, short y);
		void try_insert_element(vertex_tuple &v1,int mode, int mode_element);
		void try_delete_element(vertex_tuple &v1, int mode);


    private:
        // Texture that contains the sprites
        Rocket::Core::TextureHandle texture;
        Rocket::Core::Vector2i texture_dimensions;


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
		std::set<vertex_tuple> tubes;
		std::set<vertex_tuple> valves;
		vertex_tuple res_gen_start;
		bool if_res_gen_start;
		vertex_tuple gen;
		bool if_gen;
		vertex_tuple water_gen;
		bool if_water_gen;
		vertex_tuple control;
		bool if_control;
		vertex_tuple pump;
		bool if_pump;
		vertex_tuple light;
		bool if_light;
		vertex_tuple outer_link;
		bool if_outer_link;
		vertex_tuple patient;
		bool if_patient;
		std::set<vertex_tuple> alarm_magistral;
		std::set<vertex_tuple> reserve_magistral;
		std::set<vertex_tuple> div_corb;
		std::set<vertex_tuple> corb;

        //objectTable

        std::vector<powerGridNode*> powerGrid;


        xmlFile doc;
        std::vector<char> xml_copy;

        //Entity guest1;

        PowerSource ps1;

        PowerConsumer pc1;

        Slot<Rocket::Core::Time> UpdateTimerSlot;

        Rocket::Core::Time update_start;

        Rocket::Core::Time update_freq;





};

#endif
