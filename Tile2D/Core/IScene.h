//
// Created by Miika Pelkonen on 12/11/2017.
//

#ifndef __ISCENE_H
#define __ISCENE_H

class IScene {
    friend class SceneManager;

private:
    virtual void init() = 0;
    virtual void destroy() = 0;
};

#endif //__ISCENE_H
