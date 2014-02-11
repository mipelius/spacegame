//
// Created by Miika Pelkonen on 2/3/14.
// Copyright (c) 2014 ___MIIKAPELKONEN___. All rights reserved.
//


#ifndef __App_H_
#define __App_H_

class MusicPlayer;
class SamplePlayer;
class Window;

class App {
private:
    static App* instance_;
    MusicPlayer* musicPlayer_;
    SamplePlayer* samplePlayer_;
    Window* window_;
    App();

public:
    static App* getInstance();

    static void initialize();
    ~App();

    MusicPlayer* getMusicPlayer();
    SamplePlayer* getSamplePlayer();
    Window* getWindow();


};

#endif //__App_H_
