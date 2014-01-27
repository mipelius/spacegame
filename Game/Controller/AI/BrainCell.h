// This file is part of SpaceGame.
// Copyright (C) 2014 Miika Pelkonen
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

#ifndef __BrainCell_H_
#define __BrainCell_H_

class SpaceGameObject;
class CpuController;
class Brains;
class Team;

#include <list>

class BrainCell {
friend class Brains;
private:
    double timerSeconds;
    double tickSeconds;
protected:
    std::list<Team*> *getEnemyTeams();
    CpuController* getController();
    void setTarget(SpaceGameObject* target);
    SpaceGameObject* getTarget();
    Brains* _brains;
    virtual void operate();
public:
    BrainCell(double tickSeconds);
    void operate(double timeElapsedSeconds);
};


#endif //__BrainCell_H_
