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

GuiComponentBase::GuiComponentBase() {
    parentGuiComponent_ = nullptr;
    anchor_ = Anchor::TOP_LEFT;
    marginTop_ = 0;
    marginBottom_ = 0;
    marginLeft_ = 0;
    marginRight_ = 0;
    w_ = SIZE_MAX_WIDTH;
    h_ = SIZE_MAX_HEIGHT;
    window_ = nullptr;
}

void GuiComponentBase::setMargin(double top, double right, double bottom, double left) {
    marginTop_ = top;
    marginRight_ = right;
    marginBottom_ = bottom;
    marginLeft_ = left;
}

void GuiComponentBase::setAnchor(GuiComponentBase::Anchor anchor) {
    anchor_ = anchor;
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

Rect GuiComponentBase::getRenderingAreaRect() {
    Rect parentRect = Rect(0, 0, 0, 0);

    if (parentGuiComponent_) {
        parentRect.copy(parentGuiComponent_->getRenderingAreaRect());
    }
    else {
        parentRect = Rect(0, 0, window_->getW(), window_->getH());
    }

    double w, h;

    if (w_ >= 0) w = w_;
    else w = parentRect.getWidth();

    if (h_ >= 0) h = h_;
    else h = parentRect.getHeight();

    if (w + marginLeft_ + marginRight_ > parentRect.getWidth()) w -= (marginLeft_ + marginRight_);
    if (h + marginTop_ + marginBottom_ > parentRect.getHeight()) h -= (marginTop_ + marginBottom_);

    double x1 = 0.0;
    double y1 = 0.0;

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

    double x2 = x1 + w;
    double y2 = y1 + h;

    return Rect(x1, y1, x2, y2);

}

Window *GuiComponentBase::getWindow() {
    if (window_) return window_;
    return parentGuiComponent_->getWindow();
}

void GuiComponentBase::setW(double w) {
    w_ = w;
}

void GuiComponentBase::setH(double h) {
    h_ = h;
}

void GuiComponentBase::toggleVisibility() {
    isVisible_ = !isVisible_;
}

void GuiComponentBase::show() {
    isVisible_ = true;
}

void GuiComponentBase::hide() {
    isVisible_ = false;
}

void GuiComponentBase::render() {
    Rect rect = getRenderingAreaRect();

    GLint x = (GLint)(rect.x1);
    GLint y = (GLint)(getWindow()->getH() - rect.y2);
    GLint w = (GLint)(rect.getWidth());
    GLint h = (GLint)(rect.getHeight());

    glViewport(x, y, w, h);
}
