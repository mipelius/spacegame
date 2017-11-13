//
// Created by Miika Pelkonen on 12/11/2017.
//

#ifndef __ISCENE_H
#define __ISCENE_H

class IScene {
    virtual void Init() = 0;
    virtual void Destroy() = 0;
};

#endif //__ISCENE_H
