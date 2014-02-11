// This file is part of SpaceGame.
// Copyright (C) 2014  Miika Pelkonen
//
// SpaceGame is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// SpaceGame is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with SpaceGame.  If not, see <http://www.gnu.org/licenses/>.

#include "precompile.h"
#include "Event.h"
#include "EventArgs.h"
#include "EventHandler.h"

void Event::raise(EventArgs *eventArgs) {
    for (std::list<EventHandler*>::iterator i = eventHandlers_.begin(); i != eventHandlers_.end(); i++) {
        (*i)->handle(eventOwner_, eventArgs);
    }
}

Event::Event(IEventOwner *eventOwner) {
    this->eventOwner_ = eventOwner;
}

void Event::add(EventHandler* eventHandler) {
    eventHandlers_.push_back(eventHandler);
}

Event::~Event() {
    for (std::list<EventHandler*>::iterator i = eventHandlers_.begin(); i != eventHandlers_.end(); i++) {
        delete (*i);
    }
}
