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

#ifndef SPACEGAME_AUDIOSOURCE_H
#define SPACEGAME_AUDIOSOURCE_H

#include "ISerializable.h"
#include "Tile2DComponent.h"
#include "AudioClip.h"

class AudioSource :
        public Tile2DComponent,
        public ISerializable
{
private:
    AudioClip* clip_ = nullptr;
    bool loopEnabled_ = false;
    bool playOnAwake_ = false;
    int volume_ = 128;
    int SDL_mixer_channel_ = -1;

protected:
    void init() override;
    void onDestroy() override;
    Tile2DComponent *clone() override;

public:
    void play();
    void stop();
    void pause();
    bool isPlaying();

    void deserialize(const json::Object &jsonObject) override;

    AudioClip *getClip() const;
    void setClip(AudioClip *clip);

    bool isLoopEnabled() const;
    void setLoopEnabled(bool loopEnabled);

    bool isPlayOnAwake() const;
    void setPlayOnAwake(bool playOnAwake);

    int getVolume() const;
    void setVolume(int volume);

private:

};


#endif //SPACEGAME_AUDIOSOURCE_H
