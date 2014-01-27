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

#ifndef __SpaceGame_H_
#define __SpaceGame_H_

class std::list;
class Spaceship;
class Renderer;
class GameWorld;
class Map;
class HumanController;
class Team;

class Game {
    friend class TestRequest;
private:
    Map* map;
    GameWorld* world;
    Renderer* renderer;

    Team *enemyTeam;
    Team *playerTeam;

    Spaceship* player;
    HumanController* playerController;

    std::list<Spaceship*> *otherSpaceships;

    Spaceship* boss;
public:
    Game();
    ~Game();
    void launch();
};


#endif //__SpaceGame_H_
