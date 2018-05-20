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

#include "Tile2D.h"
#include "TileMap.h"
#include "IPathValidator.h"

std::list<Vecf> PathFinder::constructPath(
        const Vecf &startPoint,
        const Vecf &goalPoint,
        const unsigned int &maxNodesToExplore,
        const IPathValidator *validator
) {
    // --- set up data structures ---

    std::list<Vecf> path;

    const int& w = Tile2D::tileMap().getTileSet()->getTileW();
    const int& h = Tile2D::tileMap().getTileSet()->getTileH();

    Veci start          = {(int)(startPoint.x   / w), (int)(startPoint.y    / h)};
    Veci goal           = {(int)(goalPoint.x    / w), (int)(goalPoint.y     / h)};

    std::list<Node*> allocatedNodes;
    OpenSet openSet;
    ClosedSet closedSet;

    // --- set up start node ---

    auto startNode = nodePool_.malloc(); allocatedNodes.push_back(startNode);
    startNode->cameFrom = nullptr;
    startNode->gCost = 0;
    startNode->fCost = heuristicCost_(start, goal);
    startNode->position = start;

    openSet.add(startNode);

    Node *goalNode = nullptr;

    Veci neighbourPositions[NEIGHBOUR_POSITION_COUNT] = {};

    // --- find the path ---

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

        for (auto i = 0u; i < NEIGHBOUR_POSITION_COUNT; ++i) {
            Veci &neighbourPosition = neighbourPositions[i];

            if (closedSet.has(neighbourPosition)) {
                continue;
            }

            Node *currentNeighbour = openSet.get(neighbourPosition);
            if (currentNeighbour == nullptr) {
                currentNeighbour = nodePool_.malloc(); allocatedNodes.push_back(currentNeighbour);
                currentNeighbour->position = neighbourPosition;
                currentNeighbour->gCost = FLT_MAX;
                currentNeighbour->fCost = FLT_MAX;
                currentNeighbour->cameFrom = nullptr;

                if (!validator->validate(neighbourPosition, start, goal)) {
                    closedSet.add(currentNeighbour);
                    continue;
                }
            }

            bool isDiagonalNeighbour = i % 2 == 0;

            float tentative_gCost =
                    current->gCost + (isDiagonalNeighbour ? DIAGONAL_COST : NONDIAGONAL_COST);

            if (tentative_gCost >= currentNeighbour->gCost) {
                continue;
            }

            currentNeighbour->cameFrom = current;
            currentNeighbour->gCost = tentative_gCost;
            currentNeighbour->fCost = tentative_gCost + heuristicCost_(neighbourPosition, goal);

            openSet.add(currentNeighbour);
        }
    }

    // --- trace back (if path was found) and clean up ---

    float offsetX = w / 2.0f;
    float offsetY = h / 2.0f;

    Node* current = goalNode;

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