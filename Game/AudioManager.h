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

#ifndef SPACEGAME_AUDIOMANAGER_H
#define SPACEGAME_AUDIOMANAGER_H

#include "Tile2DComponent.h"
#include <vector>
#include "Vec.h"

class AudioSource;
class AudioClip;

class AudioManager : public Tile2DComponent {
public:
    static AudioManager* getInstance();
    void play(AudioClip *clip, int volume = 128);
    void play(AudioClip *clip, const Vecf& position);
    void play(const std::vector<AudioClip*>& audioClips, int volume = 128);
    void play(const std::vector<AudioClip*>& audioClips, const Vecf& position);


protected:
    void init() override;
    void onDestroy() override;
    Tile2DComponent *clone() override;

private:
    struct AudioContainer {
        AudioSource*    source;
        Uint32          timeStamp;
    };

    static AudioManager* instance_;

    static const int MIN_VOLUME_DIFF_BETWEEN_PLAYING_SAME_CLIP = 64;
    static const int MIN_DELAY_BETWEEN_PLAYING_SAME_CLIP = 48;
    static const int MAX_CHANNELS = 20;
    static const int MAX_COUNT_PLAYING_SAME_CLIP = 3;
    static constexpr float MAX_DISTANCE = 1500.0f;

    int currentSourceIndex_ = 0;
    std::vector<AudioContainer> audioContainers_ = std::vector<AudioContainer>(MAX_CHANNELS);
};


#endif //SPACEGAME_AUDIOMANAGER_H
