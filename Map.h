//
// Created by Miika Pelkonen on 12/15/13.
// Copyright (c) 2013 ___MIIKAPELKONEN___. All rights reserved.
//


#ifndef __Map_H_
#define __Map_H_

class Map {
public:
    Map(char* path);
    void setValue(int x, int y, unsigned char value);
    char getValue(int x, int y);
    short getW();
    short getH();
    char *getColumn(int x);


private:
    short w;
    short h;
    unsigned char **p_values;
    void initialize(short w, short h);

};

#endif //__Map_H_
