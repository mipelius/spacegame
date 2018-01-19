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

#include <list>

#ifndef __AnimationManager_H_
#define __AnimationManager_H_

class IAnimation;

class AnimationManager {

public:
    AnimationManager() = default;

    void remove(IAnimation* animation);
    void add(IAnimation* animation);
    void update(float seconds);

private:
    std::list<IAnimation*> animations_;
    std::list<IAnimation*> animationsToRemove_;
};


#endif //__AnimationManager_H_
