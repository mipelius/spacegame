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

#include "GameObjectGroup.h"

void GameObjectGroup::add(GameObject *gameObject) {
    this->gameObjects->push_back(gameObject);
}

GameObjectGroup::GameObjectGroup(Point focus, Point location, CollisionShape* collisionShape) :
GameEntity(focus, location, collisionShape) {
    // Should it list of GameEntities? -> would provide support for group of groups
    // change it later if you feel so...
    this->gameObjects = new std::list<GameObject*>();
}


void GameObjectGroup::render(double x, double y) {
    GameEntity::render(x, y);

    glTranslatef(x, y, 0);
    glRotatef((GLfloat)angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(-x, -y, 0);

    for (std::list<GameObject*>::iterator it = gameObjects->begin(); it != gameObjects->end(); it++) {
        Point location = (*it)->getLocation();
                (*it)->render(
                (int)(x + location.x - focus.x),
                (int)(y + location.y - focus.y)
        );
    }
    glTranslatef(x, y, 0);
    glRotatef((GLfloat)-angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(-x, -y, 0);

}

double GameObjectGroup::getMass() {
    double mass = 0.0;
    for (std::list<GameObject*>::iterator it = gameObjects->begin(); it != gameObjects->end(); it++) {
        mass += (*it)->getMass();
    }
    return mass;
}
