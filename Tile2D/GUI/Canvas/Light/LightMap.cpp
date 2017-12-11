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

#include "LightMap.h"
#include "TileMap.h"
#include "PartialLightMap.h"

LightMap::LightMap(int w, int h) :
    data_(new Array2d<std::list<PartialLightMap *>*>(w, h))
{
    for (int x=0; x<w; x++) {
        for (int y=0; y<h; y++) {
            data_->setValue(x, y, new std::list<PartialLightMap *>());
        }
    }
}

LightMap::~LightMap() {
    for (int x=0; x<data_->getW(); x++) {
        for (int y=0; y<data_->getH(); y++) {
            delete data_->getValue(x, y);
        }
    }

    delete data_;
}

unsigned char LightMap::getLightAmount(int x, int y) {
    if (!data_->isInsideBounds(x, y)) {
        return 0x0;
    }

    std::list<PartialLightMap *>* list = data_->getValue(x, y);

    if (!list || list->empty()) {
        return 0x0;
    }

    PartialLightMap* partialLightMap = list->front();

    int actualX = x - partialLightMap->getX();
    int actualY = y - partialLightMap->getY();

    return partialLightMap->getValue(actualX, actualY);
}

void LightMap::addPartialLightMap(PartialLightMap *lightMap) {
    for (int x=0; x<lightMap->getW(); x++) {
        for (int y=0; y<lightMap->getH(); y++) {
            int actualX = x + lightMap->getX();
            int actualY = y + lightMap->getY();

            if (!data_->isInsideBounds(actualX, actualY)) {
                continue;
            }

            std::list<PartialLightMap *>* list = data_->getValue(actualX, actualY);
            list->push_back(lightMap);
        }
    }

    putGreatestValuesFront(lightMap);
}

void LightMap::putGreatestValuesFront(PartialLightMap *lightMap) {
    for (int x=0; x<lightMap->getW(); x++) {
        for (int y=0; y<lightMap->getH(); y++) {
            int actualX = x + lightMap->getX();
            int actualY = y + lightMap->getY();

            if (!data_->isInsideBounds(actualX, actualY)) {
                continue;
            }

            std::list<PartialLightMap *>* list = data_->getValue(actualX, actualY);

            if (list->empty()) {
                continue;
            }

            PartialLightMap* greatestValueMap = list->front();

            unsigned greatestValue = greatestValueMap->getValue(
                    actualX - greatestValueMap->getX(),
                    actualY - greatestValueMap->getY()
            );

            for (std::list<PartialLightMap *>::iterator i = list->begin(); i != list->end(); i++) {
                unsigned newValue = (*i)->getValue(
                        actualX - (*i)->getX(),
                        actualY - (*i)->getY()
                );

                if (newValue > greatestValue) {
                    greatestValue = newValue;
                    greatestValueMap = (*i);
                }
            }

            list->remove(greatestValueMap);
            list->push_front(greatestValueMap);
        }
    }
}

void LightMap::removePartialLightMap(PartialLightMap *lightMap) {
    for (int x=0; x<lightMap->getW(); x++) {
        for (int y=0; y<lightMap->getH(); y++) {
            int actualX = x + lightMap->getX();
            int actualY = y + lightMap->getY();

            if (data_->isInsideBounds(actualX, actualY)) {
                std::list<PartialLightMap *>* list = data_->getValue(actualX, actualY);

                list->remove(lightMap);
            }
        }
    }
}

std::list<PartialLightMap *> *LightMap::getPartialLightMaps(int x, int y) {
    if (data_->isInsideBounds(x, y)) {
        return data_->getValue(x, y);
    }
    return nullptr;
}
