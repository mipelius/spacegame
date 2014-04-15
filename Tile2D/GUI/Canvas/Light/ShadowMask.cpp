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
#include "ShadowMask.h"
#include "Canvas.h"
#include "Camera.h"
#include "WorldMap.h"
#include "PointLight.h"
#include "LightMap.h"
#include "PartialLightMap.h"
#include "PartialLightMapUpdate.h"
#include "WorldMapModifiedEventArgs.h"

class ShadowMask::PointLight_MovementEventHandler : public IEventHandler<PointLight, PointLightMovedEventArgs> {

public:

    PointLight_MovementEventHandler(ShadowMask* mask, PartialLightMap* partialLightMap) {
        mask_ = mask;
        partialLightMap_ = partialLightMap;
    }

    virtual void handle(PointLight *owner, PointLightMovedEventArgs args) {
        int actualNewLocationX = (int)(args.newLocation.x / mask_->worldMap_->getBlockW());
        int actualNewLocationY = (int)(args.newLocation.y / mask_->worldMap_->getBlockH());

        if (
                abs(partialLightMap_->getX() - actualNewLocationX) == 0 &&
                abs(partialLightMap_->getY() - actualNewLocationY) == 0
        ) {
            return;
        }

        PartialLightMapUpdate* update = new PartialLightMapUpdate();

        update->x = actualNewLocationX;
        update->y = actualNewLocationY;
        update->isBlockUpdate = false;
        update->map = partialLightMap_;
        update->map->needsUpdate = true;

        mask_->partialLightMapUpdatesQueue.push(update);
    }

private:
    ShadowMask* mask_;
    PartialLightMap* partialLightMap_;
};

class ShadowMask::WorldMap_ModificationEventHandler : public IEventHandler<WorldMap, WorldMapModifiedEventArgs> {

public:

    WorldMap_ModificationEventHandler(ShadowMask* mask) {
        mask_ = mask;
    }

    virtual void handle(WorldMap *owner, WorldMapModifiedEventArgs args) {
        if (args.oldValue && args.newValue && args.oldValue->translucency->get() == args.newValue->translucency->get()) {
            return;
        }

        std::list<PartialLightMap*>* list = mask_->lightMap_->getPartialLightMaps(args.x, args.y);

        if (list && !list->empty()) {
            for (std::list<PartialLightMap*>::iterator i = list->begin(); i != list->end(); i++) {
                PartialLightMapUpdate* update = new PartialLightMapUpdate();
                update->isBlockUpdate = true;
                update->map = (*i);
                update->map->needsUpdate = true;
                mask_->partialLightMapUpdatesQueue.push(update);
            }
        }

    }

private:

    ShadowMask* mask_;
};

GLuint ShadowMask::glShadowTextureId_ = 0;

ShadowMask::ShadowMask(double w, double h, WorldMap * map) :

ambientLight    (   new SimpleProperty<double>( &ambientLight_ )    ),
ambientLight_   (   1.0 )

{
    w_ = w;
    h_ = h;

    lightMap_ = new LightMap(map->getW(), map->getH());
    worldMap_ = map;

    worldMap_->modification->add(new WorldMap_ModificationEventHandler(this));

    if (glShadowTextureId_ == 0) {
        createShadowTexture();
    }

    initialize();
}

ShadowMask::~ShadowMask() {
    delete lightMap_;
}

void ShadowMask::update(Canvas *canvas) {
    Uint32 time = SDL_GetTicks();
    while (SDL_GetTicks() - time < MAX_PARTIAL_LIGHT_MAP_UPDATE_TIME) {
        handleNextUpdate();
    }

    // turn projection upside down

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w_, 0, h_, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    // draw shadow map to texture

    glColor4d(0, 0, 0, 1.0);
    glRectd(0, 0, w_, h_);

    drawShadowMap(canvas);

    glBindTexture(GL_TEXTURE_2D, glTextureId_);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, (int)w_, (int)h_);

    // draw lights

    glColor4d(0, 0, 0, 1.0);
    glRectd(0, 0, w_, h_);

    glEnable(GL_BLEND);
    glBlendFunc(GL_DST_ALPHA, GL_ZERO);
    glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, PointLight::glTextureId_);

    glBegin(GL_QUADS);

    for (std::list<PointLight*>::iterator i = staticLights_.begin(); i != staticLights_.end(); i++) {
        (*i)->draw(canvas);
    }

    glEnd();

    glDisable(GL_BLEND);

    // blend it with map shadows

    glEnable(GL_BLEND);

    glColor4f(0, 0, 0, 1.0);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, glTextureId_);

    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex2d(0, 0);
    glTexCoord2d(1, 0);
    glVertex2d(w_, 0);
    glTexCoord2d(1, 1);
    glVertex2d(w_, h_);
    glTexCoord2d(0, 1);
    glVertex2d(0, h_);
    glEnd();

    glDisable(GL_BLEND);

    // update mask texture

    glDisable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, glTextureId_);
    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, (int)w_, (int)h_);
    glBindTexture(GL_TEXTURE_2D, NULL);

    // set "normal" projection

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w_, h_, 0, -1, 1);

    glMatrixMode(GL_MODELVIEW);
}

void ShadowMask::draw(Canvas *canvas) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, glTextureId_);

    glColor4d(1, 1, 1, 1.0 - ambientLight_);

    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex2d(0, 0);
    glTexCoord2d(1, 0);
    glVertex2d(w_, 0);
    glTexCoord2d(1, 1);
    glVertex2d(w_, h_);
    glTexCoord2d(0, 1);
    glVertex2d(0, h_);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    glDisable(GL_BLEND);
}

void ShadowMask::addLight(PointLight *light) {
    staticLights_.push_back(light);

    PartialLightMap* partialLightMap = new PartialLightMap(
            (int)(light->radius_ * 2 / worldMap_->getBlockW()),
            (int)(light->radius_ * 2 / worldMap_->getBlockH())
    );

    light->partialLightMap_ = partialLightMap;

    // TODO: this doesn't seem to work well... Maybe totally different approach for the dynamic lights??

    // light->movement->add(new PointLight_MovementEventHandler(this, partialLightMap));

    partialLightMap->setCenterLocation(
            (int)(light->location->get().x / worldMap_->getBlockW()),
            (int)(light->location->get().y / worldMap_->getBlockH())
    );

    PartialLightMapUpdate* update = new PartialLightMapUpdate();
    update->x = (int)(light->location->get().x / worldMap_->getBlockW());
    update->y = (int)(light->location->get().y / worldMap_->getBlockH());
    update->map = partialLightMap;
    update->map->needsUpdate = true;
    update->isBlockUpdate = false;

    partialLightMapUpdatesQueue.push(update);
}

void ShadowMask::initialize() {
    GLenum texture_format = GL_BGRA;
    GLint nOfColors = 4;

    Uint32* pixels = new Uint32[(int)w_ * (int)h_];

    // Have OpenGL generate a texture object handle for us
    glGenTextures(1, &glTextureId_);

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, glTextureId_);

    // Set the texture's stretching properties
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Edit the texture object's image data using the information SDL_Surface gives us
    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, (int)w_, (int)h_, 0,
            texture_format, GL_UNSIGNED_BYTE, pixels);

    delete[] pixels;
}

void ShadowMask::handleNextUpdate() {
    if (!partialLightMapUpdatesQueue.empty()) {

        PartialLightMapUpdate* update = partialLightMapUpdatesQueue.front();
        partialLightMapUpdatesQueue.pop();

        if (update->map->needsUpdate) {
            update->map->needsUpdate = false;

            if (update->isBlockUpdate) {
                update->map->clear();
                update->map->update(worldMap_);
                lightMap_->putGreatestValuesFront(update->map);
            }
            else {
                lightMap_->removePartialLightMap(update->map);
                update->map->setCenterLocation(update->x, update->y);

                update->map->clear();
                update->map->update(worldMap_);
                lightMap_->addPartialLightMap(update->map);
            }
        }
    }
}

void ShadowMask::drawShadowMap(Canvas* canvas) {
    Rect rect = canvas->getCamera()->areaRect->get();

    int xStart = 0;
    int xEnd = 0;

    if (rect.x1 > 0) {
        xStart = (int)(rect.x1 / worldMap_->getBlockW());
    }

    if (rect.x2 > worldMap_->getActualW()) {
        xEnd = worldMap_->getW();
    }
    else {
        xEnd = xStart + (int)(rect.getWidth() / worldMap_->getBlockW()) + 2;
    }

    int yStart = 0;
    int yEnd = 0;

    if (rect.y1 > 0) {
        yStart = (int)(rect.y1 / worldMap_->getBlockH());
    }

    if (rect.y2 > worldMap_->getActualW()) {
        yEnd = worldMap_->getW();
    }
    else {
        yEnd = yStart + (int)(rect.getHeight() / worldMap_->getBlockH()) + 2;
    }

    // bind shadowTexture

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, glShadowTextureId_);

    // set blending function

    glEnable(GL_BLEND);
    glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);

    double padding = 1.5;
    double texMargin = 0.15;

    glBegin(GL_QUADS);

    for (int x = xStart; x < xEnd; x++) {
        for (int y = yStart; y < yEnd; y++) {
            unsigned char lightAmount = lightMap_->getLightAmount(x, y);

            double xActual = x * worldMap_->getBlockW() - rect.x1;
            double yActual = y * worldMap_->getBlockH() - rect.y1;

            float x1 = xActual;
            float y1 = yActual;
            float x2 = xActual + worldMap_->getBlockW();
            float y2 = yActual + worldMap_->getBlockH();

            if (lightAmount == 255) {
                glColor4f(0, 0, 0, 1.0);

                float texMargin = 0.499;

                glTexCoord2f(0.0 + texMargin, 0.0 + texMargin);
                glVertex2f(x1, y1);
                glTexCoord2f(1.0 - texMargin, 0.0 + texMargin);
                glVertex2f(x2, y1);
                glTexCoord2f(1.0 - texMargin, 1.0 - texMargin);
                glVertex2f(x2, y2);
                glTexCoord2f(0.0 + texMargin, 1.0 - texMargin);
                glVertex2f(x1, y2);
            }
            else if (lightAmount > 0) {
                float value = (float)(1.0 - pow(1.0 - lightAmount / 255.0, 0.2));

                glColor4f(0.0, 0.0, 0.0, value);

                x1 -= padding * worldMap_->getBlockW();
                y1 -= padding * worldMap_->getBlockH();
                x2 += padding * worldMap_->getBlockW();
                y2 += padding * worldMap_->getBlockH();

                glTexCoord2f(0.0 + texMargin, 0.0 + texMargin);
                glVertex2f(x1, y1);
                glTexCoord2f(1.0 - texMargin, 0.0 + texMargin);
                glVertex2f(x2, y1);
                glTexCoord2f(1.0 - texMargin, 1.0 - texMargin);
                glVertex2f(x2, y2);
                glTexCoord2f(0.0 + texMargin, 1.0 - texMargin);
                glVertex2f(x1, y2);

            }
        }
    }

    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

}

void ShadowMask::createShadowTexture() {
    Uint8 alphaValue;
    double temp;

    Uint32* pixels = new Uint32[SHADOW_TEXTURE_SIZE * SHADOW_TEXTURE_SIZE];

    for (int i = 0; i < SHADOW_TEXTURE_SIZE; i++) {
        for (int j = 0; j < SHADOW_TEXTURE_SIZE; j++) {
            temp = sin(i * M_PI / SHADOW_TEXTURE_SIZE) * sin(j * M_PI / SHADOW_TEXTURE_SIZE);
            temp = temp * temp * temp * temp;
            alphaValue = (Uint8)(255 - temp * 255);
            pixels[j * SHADOW_TEXTURE_SIZE + i] = (0x000000FF - alphaValue) << 24;
        }
    }

    GLenum texture_format = GL_BGRA;
    GLint nOfColors = 4;

    // Have OpenGL generate a texture object handle for us
    glGenTextures(1, &glShadowTextureId_);

    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, glShadowTextureId_);

    // Set the texture's stretching properties
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Edit the texture object's image data
    glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, SHADOW_TEXTURE_SIZE, SHADOW_TEXTURE_SIZE, 0,
            texture_format, GL_UNSIGNED_BYTE, pixels);

    delete[] pixels;
}

void ShadowMask::removeLight(PointLight *light) {
    lightMap_->removePartialLightMap(light->partialLightMap_);
    lightMap_->putGreatestValuesFront(light->partialLightMap_);

    // you should figure out, how to do this, without crashing
    // (it will crash, if partialLightMapUpdatesQueue has update containing pointer to partialLightMap_)
    // delete light->partialLightMap_;

    light->partialLightMap_->needsUpdate = false;

    staticLights_.remove(light);
}
