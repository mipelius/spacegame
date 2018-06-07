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

protected:
    Tile2DComponent *clone() override;

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
