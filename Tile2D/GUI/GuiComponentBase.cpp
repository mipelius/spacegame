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

GuiComponentBase::GuiComponentBase() :
    // properties

    marginTop       (   Property<double>  (&marginTop_        )   ),
    marginRight     (   Property<double>  (&marginRight_      )   ),
    marginLeft      (   Property<double>  (&marginLeft_       )   ),
    marginBottom    (   Property<double>  (&marginBottom_     )   ),

    anchor          (   Property<Anchor>  (&anchor_           )   ),

    w               (   Property<double>  (&w_                )   ),
    h               (   Property<double>  (&h_                )   ),

    opacity         (   Property<double>  (&opacity_          )   ),

    isVisible       (   BooleanProperty   (&isVisible_        )   ),
    isBoundsVisible (   BooleanProperty   (&isBoundsVisible_  )   )

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

GuiComponentBase::~GuiComponentBase() {

}

void GuiComponentBase::setMargin(double top, double right, double bottom, double left) {
    marginTop.set(top);
    marginRight.set(right);
    marginBottom.set(bottom);
    marginLeft.set(left);
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
        parentRect = Rect(0, 0, window_->w.get(), window_->h.get());
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

    GLint x = (GLint)(rect.x1);
    GLint y = (GLint)(getWindow()->h.get() - rect.y2);
    GLint w = (GLint)(rect.getWidth());
    GLint h = (GLint)(rect.getHeight());

    glViewport(x, y, w, h);

    renderActual();

    for (std::list<GuiComponentBase*>::iterator i = children_.begin(); i != children_.end(); i++) {
        (*i)->render();
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

void GuiComponentBase::glColor(double red, double green, double blue) {
    glColor4d(red, green, blue, opacity_);
}