// This file is part of SpaceGame.
// Copyright (C) 2018 Miika Pelkonen
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

#include <cfloat>
#include "PathFinder.h"
#include "Vec.h"
#include "Pool.h"
#include "Tile2D.h"

float PathFinder::heuristicCost_(Veci start, Veci goal) {
    float dx = start.x - goal.x;
    float dy = start.y - goal.y;
    return sqrt(dx * dx + dy * dy);
}

bool PathFinder::canMove_(
        const Veci& pos,
        const Veci& boundingBoxTopLeftCorner,
        const Veci& boundingBoxBottomRightCorner
) {
    Tile* tile = nullptr;
    Veci currentPos;

    for (auto x = boundingBoxTopLeftCorner.x; x <= boundingBoxBottomRightCorner.x; ++x) {
        for (auto y = boundingBoxTopLeftCorner.y; y <= boundingBoxBottomRightCorner.y; ++y) {
            currentPos = pos + Veci(x, y);
            tile = Tile2D::tileMap().getValue(currentPos.x, currentPos.y);

            if (tile == nullptr || tile->getDensity() > 0.0f) {
                return false;
            }
        }
    }

    return true;
}

std::list<Vecf> PathFinder::getPath(
        const Vecf&     startPoint,
        const Vecf&     goalPoint,
        const Rect&     boundingBox
) {
    std::list<Vecf> path;

    if (!Tile2D::tileMap().isLoaded()) {
        return path;
    }

    const int& w = Tile2D::tileMap().getTileSet()->getTileW();
    const int& h = Tile2D::tileMap().getTileSet()->getTileH();

    Veci start          = {(int)(startPoint.x   / w), (int)(startPoint.y    / h)};
    Veci goal           = {(int)(goalPoint.x    / w), (int)(goalPoint.y     / h)};

    Veci boundingBoxTopLeftCorner       = {(int)floor(boundingBox.x1 / w), (int)floor(boundingBox.y1 / h)};
    Veci boundingBoxBottomRightCorner   = {(int)ceil(boundingBox.x2 / w),  (int)ceil(boundingBox.y2 / h)};

    if (
            !canMove_(start, boundingBoxTopLeftCorner, boundingBoxBottomRightCorner) ||
            !canMove_(goal,  boundingBoxTopLeftCorner, boundingBoxBottomRightCorner)
    ) {
        return path;
    }

    std::list<Node*> allocatedNodes;
    OpenSet openSet;
    ClosedSet closedSet;

    auto startNode = nodePool_.malloc(); allocatedNodes.push_back(startNode);
    startNode->cameFrom = nullptr;
    startNode->gCost = 0;
    startNode->fCost = heuristicCost_(start, goal);
    startNode->position = start;

    openSet.add(startNode);

    static const float NONDIAGONAL_COST = 1.0;
    static const float DIAGONAL_COST = sqrtf(2.0);

    Node *goalNode = nullptr;

    Veci neighbourPositions[neighbourPositionCount] = {};

    while (!openSet.empty()) {
        Node* current = openSet.popBestOut();

        if (current->position == goal) {
            goalNode = current;
            break;
        }

        closedSet.add(current);

        calculateNeigbourPositions_(neighbourPositions, current->position);

        for (auto i = 0u; i < neighbourPositionCount; ++i) {
            Veci &pos = neighbourPositions[i];
            bool isDiagonalNeighbour = i % 2 == 0;

            if (closedSet.has(pos)) {
                continue;
            }

            Node *currentNeighbour = openSet.get(pos);
            if (currentNeighbour == nullptr) {
                currentNeighbour = nodePool_.malloc(); allocatedNodes.push_back(currentNeighbour);
                currentNeighbour->position = pos;
                currentNeighbour->gCost = FLT_MAX;
                currentNeighbour->fCost = 0;
                currentNeighbour->cameFrom = nullptr;

                if (!canMove_(pos, boundingBoxTopLeftCorner, boundingBoxBottomRightCorner)) {
                    closedSet.add(currentNeighbour);
                    continue;
                }
            }

            float tentative_gCost =
                    current->gCost + (isDiagonalNeighbour ? DIAGONAL_COST : NONDIAGONAL_COST);

            if (tentative_gCost >= currentNeighbour->gCost) {
                continue;
            }

            currentNeighbour->cameFrom = current;
            currentNeighbour->gCost = tentative_gCost;
            currentNeighbour->fCost = tentative_gCost + heuristicCost_(pos, goal);

            openSet.add(currentNeighbour);
        }
    }

    Node* current = goalNode;

    float offsetX = w / 2;
    float offsetY = h / 2;

    while (current != nullptr) {
        Vecf position = {w * current->position.x + offsetX, h * current->position.y + offsetY};
        path.push_front(position);
        current = current->cameFrom;
    }

    for (auto node : allocatedNodes) {
        nodePool_.free(node);
    }

    return path;
}
