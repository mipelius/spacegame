// This file is part of SpaceGame.
// Copyright (C) 2014  Miika Pelkonen
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

#include <math.h>
#include "Vector.h"

Vector::Vector(double x, double y) {
    this->x = x;
    this->y = y;
}

Vector Vector::operator + (const Vector &otherVector) {
    return Vector(x + otherVector.x, y + otherVector.y);
}

Vector Vector::operator - (const Vector &otherVector) {
    return Vector(x - otherVector.x, y - otherVector.y);
}

Vector Vector::operator -= (Vector const &otherVector) {
    x = x - otherVector.x;
    y = y - otherVector.y;
    return Vector(x, y);
}

Vector Vector::operator += (const Vector &otherVector) {
    x = x + otherVector.x;
    y = y + otherVector.y;
    return Vector(x, y);
}

Vector Vector::operator * (const Vector &otherVector) {
    return Vector(
            (x < 0 && otherVector.x < 0) ? -x * otherVector.x : x * otherVector.x,
            (y < 0 && otherVector.y < 0) ? -y * otherVector.y : y * otherVector.y
    );
}

Vector Vector::operator * (const double &amount) {
    return Vector(x * amount, y * amount);
}

Vector Vector::operator *= (const Vector &otherVector) {
    x = (x < 0 && otherVector.x < 0) ? -x * otherVector.x : x * otherVector.x;
    y = (y < 0 && otherVector.y < 0) ? -y * otherVector.y : y * otherVector.y;
    return Vector(x, y);
}

Vector Vector::byAngle(double angleDegrees, double amount) {
    double angle = angleDegrees / 360 * 2 * M_PI;
    return Vector(
            cos(angle) * amount,
            sin(angle) * amount
    );
}
