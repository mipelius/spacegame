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

#include "RendererBase.h"
#include "Texture.h"

#ifndef __BackgroundRenderer_H_
#define __BackgroundRenderer_H_

class Texture;

class BackgroundRenderer : public RendererBase {
private:
    static const int TEXTURE_CROSSFADE_RENDER_COUNT = 30;

    Texture* texture_;
    Texture* oldTexture_;

    bool textureIsCrossFading_;
    int textureCrossFadingPhase_;

    double ratio_;

    void renderTexture(Texture* texture, double opacity);

public:
    BackgroundRenderer(Rect renderingAreaRect);

    void render();

    void setRatio(double ratio);
    void setBackgroundTexture(Texture *texture);

};


#endif //__BackgroundRenderer_H_
