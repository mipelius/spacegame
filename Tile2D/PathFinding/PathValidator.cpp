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

#include <Tile2D/Util/Tile2DMath.h>
#include "PathValidator.h"
#include "Tile2D.h"
#include "TileMap.h"

PathValidator::PathValidator(const Rect &boundingBox, const Vecf &start, const Vecf &goal) {
    auto& tilemap = Tile2D::tileMap();

    auto startMapCoords = tilemap.getMapCoords(start);
    auto goalMapCoords = tilemap.getMapCoords(goal);

    normalSize_ = calcuteNormalSize_(boundingBox);
    startSize_ = getMaxBoundingSquareSize_(startMapCoords, normalSize_);
    goalSize_ = getMaxBoundingSquareSize_(goalMapCoords, normalSize_);
}

int PathValidator::calcuteNormalSize_(const Rect& boundingBox) {
    auto& tilemap = Tile2D::tileMap();

    auto w = tilemap.getTileSet()->getTileW();
    auto h = tilemap.getTileSet()->getTileH();

    Veci boundingBoxTopLeftCorner       = {(int)floor(boundingBox.x1 / w), (int)floor(boundingBox.y1 / h)};
    Veci boundingBoxBottomRightCorner   = {(int)ceil(boundingBox.x2 / w),  (int)ceil(boundingBox.y2 / h)};

    auto absBounds = {
            abs(boundingBoxTopLeftCorner.x),
            abs(boundingBoxTopLeftCorner.y),
            abs(boundingBoxBottomRightCorner.x),
            abs(boundingBoxBottomRightCorner.y)
    };

    int size = 0;
    for (auto absBound : absBounds) {
        if (absBound > size) {
            size = absBound;
        }
    }
    return size;
}

int PathValidator::getMaxBoundingSquareSize_(const Veci &position, int maxSize) {
    int result = 0;

    for (auto size = 1; size <= maxSize; ++size) {
        if (
                !Tile2D::tileMap().canMove(
                        position,
                        {-size, -size},
                        {size, size}
                )
        ) {
            break;
        }
        result = size;
    }

    return result;
}

bool PathValidator::validate(
        const Veci &position,
        const Veci &start,
        const Veci &goal
) const {
    auto distanceToStart = (position - start).length();
    auto distanceToGoal = (position - goal).length();

    int distance;
    int minSize;


    if (distanceToStart < distanceToGoal) {
        distance = distanceToStart;
        minSize = startSize_;
    }
    else {
        distance = distanceToGoal;
        minSize = goalSize_;
    }

    int size;
    if (distance < normalSize_) {
        size = Mathi::max(minSize, distance);
    }
    else {
        size = normalSize_;
    }

    return Tile2D::tileMap().canMove(
        position,
        {-size, -size},
        {size, size}
    );
}
