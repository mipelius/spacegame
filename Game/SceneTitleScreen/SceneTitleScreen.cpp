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


#include "Background.h"
#include "Resources.h"
#include "Tile2D.h"
#include "SceneTitleScreen.h"
#include "Text.h"
#include "MovingCameraBehaviour.h"
#include "Button.h"
#include "UIPrefabs.h"
#include "Window.h"
#include "SceneManager.h"
#include "Camera.h"

void SceneTitleScreen::init() {
    // uncomment next line to load quick testing scene
    // Tile2D::sceneManager().loadScene(Scenes::quickTesting);

    // background and moving camera

    auto windowSize = Tile2D::window().getSize();

    camera_ = new Camera();
    camera_->setAreaRect({0, 0, (float)windowSize.x, (float)windowSize.y});
    Tile2D::canvas().setCamera(camera_);

    auto background = Tile2D::createGameObject();
    auto bg = background->attachComponent<Background>();
    bg->setOpacity(0.5f);
    bg->setColor({0.5f, 1.0f, 1.0f});
    bg->setTexturePtr(Tile2D::resources().textures["bg1"]);

    auto titleScreenBehaviour = Tile2D::createGameObject();
    titleScreenBehaviour->attachComponent<MovingCameraBehaviour>();

    // create transition handlers

    class ButtonClickTransitionHandler : public IEventHandler<Button, Button::ButtonEventArgs> {

    public:
        std::vector<GameObject*> UIObjectsToHide;
        std::vector<GameObject*> UIObjectsToShow;

        void handle(Button *owner, Button::ButtonEventArgs args) const override {
            for (auto UIObjectToHide : UIObjectsToHide) {
                UIObjectToHide->setIsActive(false);
            }
            for (auto UIObjectToHide : UIObjectsToShow) {
                UIObjectToHide->setIsActive(true);
            }
        }

        IEventHandler<Button, Button::ButtonEventArgs> *clone() override {
            return nullptr;
        }
    };

    auto onClickSettingsHandler = new ButtonClickTransitionHandler();
    auto onClickCreditsHandler = new ButtonClickTransitionHandler();
    auto settings_onClickBackToMainMenuHandler = new ButtonClickTransitionHandler();
    auto credits_onClickBackToMainMenuHandler = new ButtonClickTransitionHandler();

    // main menu UI

    auto textWelcome = UIPrefabs::text(
            Vecf(0.0f, -300.0f),
            "Welcome to the",
            4.0f,
            Text::HorizontalAlignment::center,
            Text::VerticalAlignment::bottom
    );

    auto textSpaceGame = UIPrefabs::text(
            Vecf(0.0f, -300.0f),
            "Space Game",
            10.0f,
            Text::HorizontalAlignment::center,
            Text::VerticalAlignment::top
    );

    auto buttonStart = UIPrefabs::button(
            Vecf(0.0f, -70.0f),
            "Start game",
            320.0f,
            [] (Button* button, Button::ButtonEventArgs args) {
                Tile2D::sceneManager().loadScene(1);
            }
    );

    auto buttonSettings = UIPrefabs::button(
            Vecf(0.0f, 0.0f),
            "Settings",
            220.0f,
            onClickSettingsHandler
    );

    auto buttonCredits = UIPrefabs::button(
            Vecf(0.0f, 70.0f),
            "Credits",
            200.0f,
            onClickCreditsHandler
    );


    auto buttonQuit = UIPrefabs::button(
            Vecf(0.0f, 140.0f),
            "Quit",
            120.0f,
            [] (Button* button, Button::ButtonEventArgs args) {
                Tile2D::quit();
            }
    );

    // settings menu

    auto resolutionString =
                std::string("Resolution: ")             +
                std::to_string(windowSize.x) +
                " x "                                   +
                std::to_string(windowSize.y);

    auto buttonResolution = UIPrefabs::button(
            Vecf(0.0f, -30.0f),
            resolutionString.data(),
            400.0f,
            [] (Button* button, Button::ButtonEventArgs args) {
                auto resolutions = Tile2D::window().getAllowedFullScreenResolutions();
                auto currentResolution = Tile2D::window().getSize();

                long i = 0;

                for ( ; i < resolutions.size(); ++i) {
                    if (currentResolution == resolutions[i]) {
                        break;
                    }
                }
                auto nextResolutionIndex = (i + resolutions.size() - 1) % resolutions.size();
                auto newSize = resolutions[nextResolutionIndex];
                Tile2D::window().setSize(newSize);
                Tile2D::window().saveSettings();

                Tile2D::canvas().getCamera()->setAreaRect(
                        {
                                0.0f,
                                0.0f,
                                (float)newSize.x,
                                (float)newSize.y
                        }
                );

                auto resolutionString =
                        std::string("Resolution: ")                     +
                        std::to_string(Tile2D::window().getSize().x)    +
                        " x "                                           +
                        std::to_string(Tile2D::window().getSize().y);

                auto text = button->gameObject()->getComponent<Text>();
                text->setString(resolutionString);
            },
            false
    );

    auto settings_buttonBackToMainMenu = UIPrefabs::button(
            Vecf(0.0f, 35.0f),
            "Back to main menu",
            500.0f,
            settings_onClickBackToMainMenuHandler,
            false
    );

    // credits menu

    auto textCredits = UIPrefabs::text(
            Vecf(0.0f, -90.0f),
            "Programming, graphics, music and soundfx",
            4.0f,
            Text::HorizontalAlignment::center,
            Text::VerticalAlignment::top,
            false
    );
    auto textCreditsMipelius = UIPrefabs::text(
            Vecf(0.0f, -30.0f),
            "mipelius",
            4.0f,
            Text::HorizontalAlignment::center,
            Text::VerticalAlignment::top,
            false
    );

    auto credits_buttonBackToMainMenu = UIPrefabs::button(
            Vecf(0.0f, 80.0f),
            "Back to main menu",
            200.0f,
            credits_onClickBackToMainMenuHandler,
            false
    );

    // define transitions

    onClickSettingsHandler->UIObjectsToHide = { buttonStart, buttonSettings, buttonCredits, buttonQuit };
    onClickSettingsHandler->UIObjectsToShow = { buttonResolution, settings_buttonBackToMainMenu };
    settings_onClickBackToMainMenuHandler->UIObjectsToHide = { buttonResolution, settings_buttonBackToMainMenu };
    settings_onClickBackToMainMenuHandler->UIObjectsToShow = { buttonStart, buttonSettings, buttonCredits, buttonQuit };

    onClickCreditsHandler->UIObjectsToHide = { buttonStart, buttonSettings, buttonCredits, buttonQuit };
    onClickCreditsHandler->UIObjectsToShow = { textCredits, textCreditsMipelius, credits_buttonBackToMainMenu };
    credits_onClickBackToMainMenuHandler->UIObjectsToHide = { textCredits, textCreditsMipelius, credits_buttonBackToMainMenu};
    credits_onClickBackToMainMenuHandler->UIObjectsToShow = { buttonStart, buttonSettings, buttonCredits, buttonQuit };

}

void SceneTitleScreen::destroy() {
    delete camera_;
    Tile2D::canvas().setCamera(nullptr);
}

void SceneTitleScreen::deserialize(const json::Object &jsonObject) { }
