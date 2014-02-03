//
// Created by Miika Pelkonen on 2/3/14.
// Copyright (c) 2014 ___MIIKAPELKONEN___. All rights reserved.
//


#ifndef __App_H_
#define __App_H_

class MusicPlayer;
class SamplePlayer;

class App {
private:
    static App* _instance;
    MusicPlayer* musicPlayer;
    SamplePlayer* samplePlayer;
    App();
public:
    static App* instance();

    static void initialize();
    ~App();

    MusicPlayer* getMusicPlayer();
    SamplePlayer* getSamplePlayer();


};

#endif //__App_H_
