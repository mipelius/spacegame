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
#include <list>
#include <queue>
#include "Tile2DMath.h"
#include "Tile.h"
#include "Vec.h"
#include "Pool.h"
#include "Rect.h"

class IPathValidator;

class PathFinder {
    friend class Tile2D;

public:
    std::list<Vecf> constructPath(
            const Vecf &startPoint,
            const Vecf &goalPoint,
            const unsigned int &maxNodesToExplore,
            const IPathValidator *validator
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

    static constexpr float NONDIAGONAL_COST = 1.0f;
    static constexpr float DIAGONAL_COST = 1.4142136f;

    static const unsigned int NEIGHBOUR_POSITION_COUNT = 8;

    inline float heuristicCost_(Veci start, Veci goal) {
        auto dx = (float)(start.x - goal.x);
        auto dy = (float)(start.y - goal.y);
        return fabsf(dx) + fabsf(dy);
    }

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

    // --- closed/open set implementations ---

    // NOTE: closed and open set have separate implementations since they are
    //       utilized differently in the algorithm (hence having restricted and different public interfaces)
    //       and might have different optimization needs

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

    struct CmpNode {
    public:
        bool operator() (const Node* node1, const Node* node2) {
            return node1->fCost > node2->fCost;
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

    // --------------------------------------------------
};


#endif //SPACEGAME_PATHFINDER_H
