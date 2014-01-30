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

#ifndef __EyeBrainCell_H_
#define __EyeBrainCell_H_

class TargetSelectorBrainCell;

#include "Point.h"
#include "BrainCell.h"

class EyeBrainCell : public BrainCell {
private:
    bool _targetIsInSight;
    Point _targetLocation;
    TargetSelectorBrainCell*_targetSelectorBrainCell;

    void updateIsInSight();
protected:
    void operate();
public:
    EyeBrainCell(double tickSeconds);
    bool seesTarget();
    Point getTargetLocation();

    void setTargetSelectorBrainCell(TargetSelectorBrainCell* targetSelectorBrainCell);
};


#endif //__EyeBrainCell_H_
