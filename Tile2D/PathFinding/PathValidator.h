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

#ifndef SPACEGAME_PATHVALIDATOR_H
#define SPACEGAME_PATHVALIDATOR_H

#include "Rect.h"
#include "IPathValidator.h"

class PathValidator : public IPathValidator {
public:
    PathValidator(const Rect &boundingBox, const Vecf &start, const Vecf &goal);

    bool validate (
            const Veci &position,
            const Veci &start,
            const Veci &goal
    ) const override;

private:
    int startSize_ = 0;
    int goalSize_ = 0;
    int normalSize_ = 0;

    int getMaxBoundingSquareSize_(const Veci &position, int maxSize);
    int calcuteNormalSize_(const Rect &boundingBox);
};


#endif //SPACEGAME_PATHVALIDATOR_H
