/*
 * This source file is part of libRocket, the HTML/CSS Interface Middleware
 *
 * For the latest information, see http://www.librocket.com
 *
 * Copyright (c) 2008-2010 CodePoint Ltd, Shift Technology Ltd
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef ROCKETINVADERSEVENT_H
#define ROCKETINVADERSEVENT_H

#undef __WIN32__
#undef _WIN32
#undef ROCKET_PLATFORM_WIN32
#define ROCKET_PLATFORM_UNIX


#include <Rocket/Core/EventListener.h>

/**
	@author Peter Curry
 */

class Event : public Rocket::Core::EventListener
{
public:
	Event(const Rocket::Core::String& value);
	virtual ~Event();

	/// Sends the event value through to Invader's event processing system.
	virtual void ProcessEvent(Rocket::Core::Event& event);

	/// Destroys the event.
	virtual void OnDetach(Rocket::Core::Element* element);

private:
	Rocket::Core::String value;
};

#endif
