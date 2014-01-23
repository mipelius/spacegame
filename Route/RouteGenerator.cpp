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
#include "RouteGenerator.h"
#include "Map.h"
#include "Node.h"

static const int ADJACENT_NODES_RELATIVE_LOCATIONS_COUNT = 8;
static const int ADJACENT_NODES_RELATIVE_LOCATIONS[ADJACENT_NODES_RELATIVE_LOCATIONS_COUNT][2] = {
        {-1, -1}, // north west
        {0, -1},  // north
        {1, -1},  // north east
        {-1, 1},  // south west
        {0, 1},   // south
        {1, 1},   // south east
        {-1, 0},  // east
        {1, 0},   // west
};

const int DIAGONAL_COST = 10;
const int ORTHOGONAL_COST = 14;

RouteGenerator::RouteGenerator(Map *map) {
    this->map = map;
}

int RouteGenerator::heuristicFunction(Node *startNode, Node *goalNode) {
    int x = abs(startNode->x - goalNode->x);
    x *= DIAGONAL_COST;

    int y = abs(startNode->y - goalNode->y);
    y *= DIAGONAL_COST;

    return x + y;
}

Node *RouteGenerator::generateRoute(Point startPoint, Point goalPoint) {
    // if there is block at the start point or goal point, it's not possible to generate route -> return nullptr

    if (map->getValueActual((int)startPoint.x, (int)startPoint.y)) return nullptr;
    if (map->getValueActual((int)goalPoint.x, (int)goalPoint.y)) return nullptr;

    // initialize lists and both start node and goal node

    std::list<Node*> openList = std::list<Node*>();
    std::list<Node*> closedList = std::list<Node*>();

    Node* startNode = Node::byPoint(startPoint, map->getBlockW(), map->getBlockH());
    Node* goalNode = Node::byPoint(goalPoint, map->getBlockW(), map->getBlockH());

    Node* currentNode;

    // the actual algorithm begins

    // 1) put the start node on the open list
    openList.push_back(startNode);

    // 2) loop the following as long as the goal node has not been reached

    while (!(!closedList.empty() && closedList.back()->equals(goalNode))) {
        // 2.a) find the node having lowest fCost from the open list -> set current node to point that node

        Node* lowestCostNode = openList.front();
        for (std::list<Node*>::iterator it = openList.begin(); it != openList.end(); it++) {
            if ((*it)->fCost < lowestCostNode->fCost) lowestCostNode = (*it);
        }

        currentNode = lowestCostNode;

        // 2.b) switch current node from open list to closed list

        openList.remove(currentNode);
        closedList.push_back(currentNode);

        // 2.c) do the following to all adjacent walkable nodes

        for(int i=0; i<ADJACENT_NODES_RELATIVE_LOCATIONS_COUNT; i++) {
            int x = currentNode->x + ADJACENT_NODES_RELATIVE_LOCATIONS[i][0];
            int y = currentNode->y + ADJACENT_NODES_RELATIVE_LOCATIONS[i][1];
            unsigned char value = map->getValue(x, y);

            if (value != 0) continue; // not walkable

            Node* adjacentNode = new Node(x, y);

            // if it is already on the closed list ignore it

            bool isOnTheClosedList = false;
            for (std::list<Node*>::iterator it = closedList.begin(); it != closedList.end(); it++) {
                if (adjacentNode->equals((*it))) {
                    isOnTheClosedList = true;
                    break;
                }
            }

            if (isOnTheClosedList) {
                delete adjacentNode; // delete because it is the unused copy of the node on the closed list
                continue;
            }

            // otherwise, check if it is on the open list

            bool isOnTheOpenList = false;
            for (std::list<Node*>::iterator it = openList.begin(); it != openList.end(); it++) {
                if (adjacentNode->equals((*it))) {
                    isOnTheOpenList = true;
                    delete adjacentNode; // we don't want to use the copy (with uninitialized cost values)
                    adjacentNode = (*it);
                    break;
                }
            }

            // if it is not on the open list set it's previousNode to current node, and generate the g, h and f costs
            // and finally add it to open list

            if (!isOnTheOpenList) {
                adjacentNode->previousNode = currentNode;
                bool movementIsDiagonal = (
                        abs(ADJACENT_NODES_RELATIVE_LOCATIONS[i][0]) +
                        abs(ADJACENT_NODES_RELATIVE_LOCATIONS[i][1]) != 2
                );
                adjacentNode->gCost = currentNode->gCost + (movementIsDiagonal ? DIAGONAL_COST : ORTHOGONAL_COST);
                adjacentNode->hCost = heuristicFunction(adjacentNode, goalNode);
                adjacentNode->fCost = adjacentNode->gCost + adjacentNode->hCost;
                openList.push_back(adjacentNode);
            }

            // if it is on the open list and if the g cost would be better when going through the current node
            // change it's previousNode to current node and update f and g costs

            else {
                bool movementIsDiagonal = (
                        abs(ADJACENT_NODES_RELATIVE_LOCATIONS[i][0]) +
                        abs(ADJACENT_NODES_RELATIVE_LOCATIONS[i][1]) != 2
                );
                int newPathGCost = currentNode->gCost + (movementIsDiagonal ? DIAGONAL_COST : ORTHOGONAL_COST);
                if (newPathGCost < adjacentNode->gCost) {
                    adjacentNode->previousNode = currentNode;
                    adjacentNode->gCost = newPathGCost;
                    adjacentNode->fCost = adjacentNode->gCost + adjacentNode->hCost;
                }
            }
        }

        // if open list is empty, there is no path -> return nullptr

        if (openList.empty()) return nullptr;
    }

    // 3) the path is found, but it's only possible to go it trough backwards
    //    go through the nodes backwards and set the nextNode-values

    Node* target = closedList.back();
    currentNode = target;
    Node* nextNode = nullptr;

    while(currentNode->previousNode != nullptr) {
        currentNode->nextNode = nextNode;
        currentNode->generator = this;
        nextNode = currentNode;
        currentNode = currentNode->previousNode;
    }

    currentNode->nextNode = nextNode;
    currentNode->generator = this;

    // TODO: clean up both open and closed lists

    return currentNode; // return the first node of the path
}
