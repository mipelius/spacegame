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

#include "precompile.h"
#include "Route.h"
#include "Node.h"

Route::Route(Node *firstNode, Node *lastNode) {
    this->_firstNode = firstNode;
    this->_lastNode = lastNode;

    _currentNode = nullptr;
}

Route::~Route() {
    Node* currentNode = _firstNode;

    while(currentNode && currentNode->getNextNode()) {
        Node* nodeToDelete = currentNode;
        currentNode = currentNode->getNextNode();
        delete nodeToDelete;
    }

    delete currentNode;
}

Node* Route::getCurrentNode() {
    return this->_currentNode;
}

void Route::previousNode() {
    _currentNode = _currentNode->getPreviousNode();
}

void Route::nextNode() {
    _currentNode = _currentNode->getNextNode();
}

void Route::begin() {
    _currentNode = _firstNode;
}

void Route::end() {
    _currentNode = _lastNode;
}
