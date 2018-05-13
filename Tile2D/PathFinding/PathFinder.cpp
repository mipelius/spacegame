// MIT License
//
// This file is part of SpaceGame.
// Copyright (c) 2014-2018 Miika Pelkonen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#include "PathFinder.h"

#include <cfloat>
#include "Tile2DMath.h"
#include "Tile2D.h"
#include "TileMap.h"

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
            !Tile2D::tileMap().canMove(goal,  {0, 0}, {0, 0})
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

                // bounding box will be squeezed near the start and near the goal
                Veci currentBoundingBoxTopLeftCorner = getCurrentBoundingBoxCorner(
                        boundingBoxTopLeftCorner, pos, start, goal);
                Veci currentBoundingBoxBottomRightCorner = getCurrentBoundingBoxCorner(
                        boundingBoxBottomRightCorner, pos, start, goal);

                if (!Tile2D::tileMap().canMove(
                        pos,
                        currentBoundingBoxTopLeftCorner,
                        currentBoundingBoxBottomRightCorner)
                ) {
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