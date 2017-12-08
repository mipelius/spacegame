// This file is part of SpaceGame.
// Copyright (C) 2017 Miika Pelkonen
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

#include <Tile2D/Physics/BodyCollisionEventArgs.h>
#include <Tile2D/Core/GameObject.h>
#include "Body.h"
#include "DrawableGroup.h"
#include "Text.h"
#include "Plot.h"
#include "Background.h"
#include "Sprite.h"
#include "SceneTitleScreen.h"
#include "CollisionShape.h"

#include "Tile2D.h"
#include "Camera.h"

class MyCollisionEventHandler : public IEventHandler<Body, BodyCollisionEventArgs> {
public:
    void handle(Body *owner, BodyCollisionEventArgs args) override {
        std::cout << "oh my gosh!\n";
    }
};

void SceneTitleScreen::init() {
    // just testing the engine...
    std::cout << "Title Screen loaded\n";

    // setup canvas and camera

    auto camera = Tile2D::create<Camera>();
    camera->areaRect.set({0, 0, (double)Tile2D::window().w.get(), (double)Tile2D::window().h.get()});
    Tile2D::canvas().setCamera(camera);

    // background

    auto background = Tile2D::create<GameObject>();
    auto bg = Tile2D::create<Background>();
    bg->texturePtr.set(Tile2D::resources().textures["bg2"]);
    background->addComponent(bg);

    // collection of sprites

    auto collectionOfSprites = Tile2D::create<GameObject>();

    auto sprite1 = Tile2D::create<Sprite>();
    sprite1->position.set({0, 0});
    sprite1->rect.set({0, 0, 100, 100});
    sprite1->texturePtr.set(Tile2D::resources().textures["spaceship"]);

    auto sprite2 = Tile2D::create<Sprite>();
    sprite2->position.set({200, 200});
    sprite2->rect.set({0, 0, 100, 100});
    sprite2->angle.set(37.0);
    sprite2->texturePtr.set(Tile2D::resources().textures["spaceship"]);

    auto spriteCollection = Tile2D::create<DrawableGroup>();
    spriteCollection->addDrawable(sprite1);
    spriteCollection->addDrawable(sprite2);
    spriteCollection->angle.set(10.0);
    spriteCollection->position.set({200, 100});

    collectionOfSprites->addComponent(spriteCollection);

    // text

    auto textObj = Tile2D::create<GameObject>();

    auto text = Tile2D::create<Text>();
    text->string.set("Hello World!");
    text->position.set({100, 600});
    text->fontPtr.set(Tile2D::resources().fonts["smallfont"]);
    text->color.set(Color(1.0, 0.0, 0.0));
    text->size.set(3.0);

    textObj->addComponent(text);

    // plots

    for (auto i = 0u; i < 100; i++) {
        auto plot = Tile2D::create<Plot>();
        double x = 100 + i * 5;
        double y = 100 + cos((double)i / 10.0) * 100;
        plot->position.set({x, y});
        plot->size.set(10.0);

        auto body = Tile2D::create<Body>();
        body->position.set({x, y});
        plot->position.bind(body->position);

        body->mass.set(100.0 + rand() % 300);

        auto colShape = Tile2D::create<CollisionShape>();
        colShape->points.set
                ({
                        Vec(-1, -1) * 5,
                        Vec(-1,  1) * 5,
                        Vec( 1,  1) * 5,
                        Vec( 1, -1) * 5
                 });

        body->setCollisionShape(colShape);
        static MyCollisionEventHandler handler;
        body->bodyCollision.add(&handler);

        auto gameObj = Tile2D::create<GameObject>();

        gameObj->addComponent(body);
        gameObj->addComponent(plot);
    }
}

void SceneTitleScreen::destroy() {

}
