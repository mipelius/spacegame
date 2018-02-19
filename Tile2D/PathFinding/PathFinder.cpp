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

#include "PathFinder.h"

#include <cfloat>
#include "Tile2D.h"
#include "TileMap.h"
#include "TileSet.h"

float PathFinder::heuristicCost_(Veci start, Veci goal) {
    float dx = (float)(start.x - goal.x);
    float dy = (float)(start.y - goal.y);
    return fabsf(dx) + fabsf(dy);
}

std::list<Vecf> PathFinder::getPath(
        const Vecf &startPoint,
        const Vecf &goalPoint,
        const unsigned int &maxNodesToExplore,
        const bool &useInnerBounding,
        const Rect &boundingBox
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

    if (useInnerBounding) {
        boundingBoxTopLeftCorner        += {1, 1};
        boundingBoxBottomRightCorner    -= {1, 1};
    }

    if (
            !Tile2D::tileMap().canMove(start, {0, 0}, {0, 0}) ||
            !Tile2D::tileMap().canMove(goal,  boundingBoxTopLeftCorner, boundingBoxBottomRightCorner)
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

        if (closedSet.size() >= maxNodesToExplore) {
            break;
        }

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

                if (!Tile2D::tileMap().canMove(pos, boundingBoxTopLeftCorner, boundingBoxBottomRightCorner)) {
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

Tile *PathFinder::castLine(const Vecf &startPoint, const Vecf &goalPoint) {
    auto& tileMap = Tile2D::tileMap();

    Tile* result = nullptr;

    if (!tileMap.isLoaded()) {
        return result;
    }

    Veci start = {
            (int)(startPoint.x / tileMap.getTileSet()->getTileW()),
            (int)(startPoint.y / tileMap.getTileSet()->getTileH())
    };

    Veci goal = {
            (int)(goalPoint.x / tileMap.getTileSet()->getTileW()),
            (int)(goalPoint.y / tileMap.getTileSet()->getTileH())
    };

    // horizontal case
    if (start.y == goal.y) {
        int stepX = goal.x - start.x > 0 ? 1 : -1;
        for (auto x = start.x; start.x != goal.x; x += stepX) {
            result = tileMap.getValue(x, start.y);
            if (result != nullptr) {
                return result;
            }
        }
    }

    // vertical case
    if (start.x == goal.x) {
        int stepY = goal.y - start.y > 0 ? 1 : -1;
        for (auto y = start.y; start.y != goal.y; y += stepY) {
            result = tileMap.getValue(start.x, y);
            if (result != nullptr) {
                return result;
            }
        }
    }
    // other cases
    // TODO

    return result;
}
