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


#ifndef SPACEGAME_PATHFINDER_H
#define SPACEGAME_PATHFINDER_H

#include <map>
#include <queue>
#include <list>
#include "Vec.h"
#include "Pool.h"
#include "Rect.h"

class PathFinder {
    friend class Tile2D;
public:
    std::list<Vecf> getPath(
            const Vecf&     start,
            const Vecf&     goal,
            const Rect&     boundingBox = {-0.5f, -0.5f, 0.5f, 0.5f},
            const bool&     useInnerBounding = false
    );

private:
    PathFinder() = default;
    ~PathFinder() = default;

    struct Node {
        Node *cameFrom;
        float gCost;
        float fCost;
        Veci position;
    };

    Pool<Node> nodePool_ = Pool<Node>(1000000);  // ~= 24 MB

    float heuristicCost_(Veci start, Veci goal);

    struct CmpNode {
    public:
        bool operator() (const Node* node1, const Node* node2) {
            return node1->fCost > node2->fCost;
        }
    };

    struct CmpVeci {
        bool operator() (const Veci& a, const Veci& b) const {
            if (a.x != b.x) {
                return a.x < b.x;
            }
            else {
                return a.y < b.y;
            }
        }
    };

    class ClosedSet {
    private:
        std::map<Veci, Node*, CmpVeci> map;

    public:
        inline bool has(Veci position) {
            return map.count(position) > 0;
        }

        inline void add(Node* node) {
            map.insert({node->position, node});
        }
    };

    class OpenSet {
    private:
        std::map<Veci, Node*, CmpVeci> map;
        std::priority_queue<Node*, std::vector<Node*>, CmpNode> queue;

    public:
        OpenSet() {}

        inline Node* get(Veci position) {
            auto it = map.find(position);
            if (it != map.end()) {
                return (*it).second;
            }
            else {
                return nullptr;
            }
        }

        inline void add(Node* node) {
            map.insert({node->position, node});
            queue.push(node);
        }

        inline Node* popBestOut() {
            Node* best = queue.top();
            queue.pop();
            map.erase(best->position);

            return best;
        }

        inline bool empty() {
            return map.empty();
        }
    };

    static const unsigned int neighbourPositionCount = 8;

    inline void calculateNeigbourPositions_(
            Veci *neighbourPositions,
            const Veci &currentPosition
    ) {
        neighbourPositions[0] = {currentPosition.x - 1, currentPosition.y - 1}; // TOP LEFT         DIAGONAL
        neighbourPositions[1] = {currentPosition.x - 0, currentPosition.y - 1}; // TOP MIDDLE
        neighbourPositions[2] = {currentPosition.x + 1, currentPosition.y - 1}; // TOP RIGHT        DIAGONAL
        neighbourPositions[3] = {currentPosition.x + 1, currentPosition.y + 0}; // MIDDLE RIGHT
        neighbourPositions[4] = {currentPosition.x + 1, currentPosition.y + 1}; // BOTTOM RIGHT     DIAGONAL
        neighbourPositions[5] = {currentPosition.x - 0, currentPosition.y + 1}; // BOTTOM MIDDLE
        neighbourPositions[6] = {currentPosition.x - 1, currentPosition.y + 1}; // BOTTOM LEFT      DIAGONAL
        neighbourPositions[7] = {currentPosition.x - 1, currentPosition.y + 0}; // MIDDLE LEFT
    }
};


#endif //SPACEGAME_PATHFINDER_H
