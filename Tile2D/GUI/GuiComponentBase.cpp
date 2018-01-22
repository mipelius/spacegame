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

#include "precompile.h"
#include "GuiComponentBase.h"
#include "Window.h"

GuiComponentBase::GuiComponentBase()
{
    parentGuiComponent_ = nullptr;
    opacity_ = 1.0;

    marginTop_ = 0;
    marginBottom_ = 0;
    marginLeft_ = 0;
    marginRight_ = 0;

    anchor_ = Anchor::TOP_LEFT;

    w_ = SIZE_MAX_WIDTH;
    h_ = SIZE_MAX_HEIGHT;
    window_ = nullptr;

    isVisible_ = true;
    isBoundsVisible_ = false;
}

void GuiComponentBase::setMargin(float top, float right, float bottom, float left) {

}

void GuiComponentBase::setWindow(Window *window) {
    this->window_ = window;
}

void GuiComponentBase::addComponent(GuiComponentBase* guiComponent) {
    guiComponent->setParent(this);
    this->children_.push_back(guiComponent);
}

void GuiComponentBase::setParent(GuiComponentBase *guiComponent) {
    parentGuiComponent_ = guiComponent;
}

Rect GuiComponentBase::getRenderingAreaRect() const {
    Rect parentRect = Rect(0, 0, 0, 0);

    if (parentGuiComponent_ != nullptr) {
        parentRect.copy(parentGuiComponent_->getRenderingAreaRect());
    }
    else {
        parentRect = Rect(0, 0, window_->getW(), window_->getH());
    }

    float w, h;

    if (w_ >= 0) w = w_;
    else w = parentRect.getWidth();

    if (h_ >= 0) h = h_;
    else h = parentRect.getHeight();

    if (w + marginLeft_ + marginRight_ > parentRect.getWidth()) w -= (marginLeft_ + marginRight_);
    if (h + marginTop_ + marginBottom_ > parentRect.getHeight()) h -= (marginTop_ + marginBottom_);

    float x1 = 0.0;
    float y1 = 0.0;

    switch (this->anchor_) {
        case TOP_LEFT:
            x1 = parentRect.x1 + marginLeft_;
            y1 = parentRect.y1 + marginTop_;
            break;
        case TOP_RIGHT:
            x1 = parentRect.x2 - marginRight_ - w;
            y1 = parentRect.y1 + marginTop_;
            break;
        case BOTTOM_LEFT:
            x1 = parentRect.x1 + marginLeft_;
            y1 = parentRect.y2 - marginBottom_ - h;
            break;
        case BOTTOM_RIGHT:
            x1 = parentRect.x2 - marginRight_ - w;
            y1 = parentRect.y2 - marginBottom_ - h;
            break;
    }

    float x2 = x1 + w;
    float y2 = y1 + h;

    return {x1, y1, x2, y2};

}

Window *GuiComponentBase::getWindow() {
    if (window_ != nullptr) return window_;
    return parentGuiComponent_->getWindow();
}

void GuiComponentBase::render() {
    if (!isVisible_) return;

    bool glBlendEnabled = false;

    if (opacity_ < 1.0) {
        glBlendEnabled = true;
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    glColor(1.0, 1.0, 1.0);

    Rect rect = getRenderingAreaRect();

    auto x = (GLint)(rect.x1);
    auto y = (GLint)(getWindow()->getH() - rect.y2);
    auto w = (GLint)(rect.getWidth());
    auto h = (GLint)(rect.getHeight());

    glViewport(x, y, w, h);

    renderActual();

    for (auto child : children_) {
        child->render();
    }

    if (isBoundsVisible_) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, rect.getWidth(), rect.getHeight(), 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);

        glColor(1, 1, 1);

        glRectd(0, 0, rect.getWidth(), 1);
        glRectd(0, 1, 1, rect.getHeight()-1);
        glRectd(rect.getWidth()-1, 1, rect.getWidth(), rect.getHeight()-1);
        glRectd(0, rect.getHeight()-1, rect.getWidth(), rect.getHeight());
    }

    if (glBlendEnabled) {
        glDisable(GL_BLEND);
    }
}

void GuiComponentBase::glColor(float red, float green, float blue) const {
    glColor4f(red, green, blue, opacity_);
}