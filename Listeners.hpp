
#ifndef LISTENERS_H
#define LISTENERS_H

#include <Rocket/Core/EventListener.h>
#include <Rocket/Core/Types.h>

/**
	@author Pete
 */

class ClickListener : public Rocket::Core::EventListener
{
public:
	/// Registers an elemenet as being a container of draggable elements.
	static void RegisterClickableContainer(Rocket::Core::Element* element);

protected:
	virtual void ProcessEvent(Rocket::Core::Event& event);
};


class MouseOverListener : public Rocket::Core::EventListener
{
public:
	static void RegisterMouseOverContainer(Rocket::Core::Element* element);

protected:
	virtual void ProcessEvent(Rocket::Core::Event& event);
};

class MouseOutListener : public Rocket::Core::EventListener
{
public:

	static void RegisterMouseOutContainer(Rocket::Core::Element* element);

protected:
	virtual void ProcessEvent(Rocket::Core::Event& event);
};


#endif
