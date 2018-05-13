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



#ifndef SPACEGAME_PATHFINDER_H
#define SPACEGAME_PATHFINDER_H

#include <map>
#include <queue>
#include <list>
#include "Tile2DMath.h"
#include "Tile.h"
#include "Vec.h"
#include "Pool.h"
#include "Rect.h"

class PathFinder {
    friend class Tile2D;
public:
    std::list<Vecf> getPath(
            const Vecf &start,
            const Vecf &goal,
            const unsigned int &maxNodesToExplore,
            const bool &useInnerBounding,
            const Rect &boundingBox
    );

private:
    PathFinder() = default;
    ~PathFinder() = default;

    inline Veci getCurrentBoundingBoxCorner(
            Veci originalBoundingBoxCorner,
            const Veci& pos,
            const Veci& start,
            const Veci& goal
    ) {
        auto distanceToStart = (int)heuristicCost_(pos, start) - 1;
        auto distanceToGoal = (int)heuristicCost_(pos, goal) - 1;

        auto threshold = Mathi::min(distanceToStart, distanceToGoal);
        if (threshold < 0) {
            threshold = 0;
        }

        if (threshold < abs(originalBoundingBoxCorner.x)) {
            originalBoundingBoxCorner.x = Mathi::sign(originalBoundingBoxCorner.x) * threshold;
        }
        if (threshold < abs(originalBoundingBoxCorner.y)) {
            originalBoundingBoxCorner.y = Mathi::sign(originalBoundingBoxCorner.y) * threshold;
        }

        return originalBoundingBoxCorner;
    }

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
        inline size_t size() {
            return map.size();
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
