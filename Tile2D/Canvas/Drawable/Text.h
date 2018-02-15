// This file is part of SpaceGame.
// Copyright (C) 2015 Miika Pelkonen
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

#ifndef __Text_H_
#define __Text_H_

#include <string>
#include "Vec.h"
#include "DrawableBase.h"

class Font;

class Text : public DrawableBase {
public:
    Text();

    enum class HorizontalAlignment {left, center, right};
    enum class VerticalAlignment {top, center, bottom};

    // getters and setters

    Font *getFontPtr() const;
    void setFontPtr(Font *fontPtr);
    float getFontSize() const;
    void setFontSize(float size);
    const std::string &getString() const;
    void setString(const std::string &string);
    void drawActual(const Canvas &canvas) override;
    HorizontalAlignment getHorizontalAlignment() const;
    void setHorizontalAlignment(HorizontalAlignment horizontalAlignment);
    VerticalAlignment getVerticalAlignment() const;
    void setVerticalAlignment(VerticalAlignment verticalAlignment);

private:

private:
    Font* fontPtr_;

    float fontSize_;
    std::string string_;

    HorizontalAlignment horizontalAlignment_;
    VerticalAlignment verticalAlignment_;

    Vecf size_;

    void updateTextSize_();
    void unsetSize_();
};


#endif //__Text_H_
