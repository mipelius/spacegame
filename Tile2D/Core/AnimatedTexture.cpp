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
#include "AnimatedTexture.h"
#import "ITexture.h"

AnimatedTexture::AnimatedTexture(int frames, int fps, Texture* texture) {
    this->_frames = frames;
    this->_fps = fps;

    this->texture = texture;

    stop();
}

void AnimatedTexture::update(double timeElapsedSec) {
    if (_isStopped || !_isPlaying) return;
    _timeElapsedAfterPreviousFrame += timeElapsedSec;
    if (_timeElapsedAfterPreviousFrame >= 1.0 / _fps) {
        _timeElapsedAfterPreviousFrame = 0;
        _currentFrame++;
        if (_currentFrame >= _frames) _currentFrame = 0;
    }
}

void AnimatedTexture::stop() {
    _timeElapsedAfterPreviousFrame = 0;
    _currentFrame = 0;
    _isStopped = true;
    _isPlaying = false;
}

void AnimatedTexture::play() {
    _isStopped = false;
    _isPlaying = true;
}

void AnimatedTexture::pause() {
    _isPlaying = false;
}

void AnimatedTexture::glTexCorner(ITexture::Corner corner) {
    GLfloat w = 1.0 / _frames;
    GLfloat left = _currentFrame * w + 0.01;
    GLfloat right = (_currentFrame + 1) * w - 0.01;

    switch (corner) {
        case (TOP_LEFT):
            glTexCoord2d(left, 0.01);
            break;
        case (TOP_RIGHT):
            glTexCoord2d(right, 0.01);
            break;
        case (BOTTOM_LEFT):
            glTexCoord2d(right, 0.99);
            break;
        case (BOTTOM_RIGHT):
            glTexCoord2d(left, 0.99);
            break;
    }
}

void AnimatedTexture::glUnbind() {
    this->texture->glUnbind();
}

void AnimatedTexture::glBind() {
    this->texture->glBind();
}