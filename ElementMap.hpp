
#ifndef ELEMENTMAP_HPP
#define ELEMENTMAP_HPP

#include <Rocket/Core/Element.h>
#include <Rocket/Core/EventListener.h>
#include <Rocket/Core/Types.h>


class Map;

/**
	@author Peter Curry
 */

class ElementMap : public Rocket::Core::Element, public Rocket::Core::EventListener
{
public:
	ElementMap(const Rocket::Core::String& tag);
	virtual ~ElementMap();

	/// Intercepts and handles key events.
	void ProcessEvent(Rocket::Core::Event& event);
	/// Receive notifications when child elements are added
	/// This will only get called when we're added to the tree,
	/// which allows us to bind to onload
	void OnChildAdd(Rocket::Core::Element* element);

protected:
	/// Updates the Map.
	virtual void OnUpdate();
	/// Renders the Map.
	virtual void OnRender();

private:
    Map* map;

public:
    int mode;


};

#endif
