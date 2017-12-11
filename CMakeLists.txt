# This file is part of SpaceGame.
# Copyright (C) 2014  Miika Pelkonen
#
# SpaceGame is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# SpaceGame is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with SpaceGame.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.8)
project(SpaceGame)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;MinSizeRel;RelWithDebInfo")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/CMakeModules")

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(
        ${OPENGL_INCLUDE_DIR}
        ${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_MIXER_INCLUDE_DIR}
        ./
        3rdParty
        Game
        Game/Object
        Tile2D/Animation
        Tile2D/GUI/Canvas/Drawable
        Tile2D/GUI/Canvas/Light
        Tile2D/GUI/Canvas/Texture
        Tile2D/GUI/Canvas
        Tile2D/GUI
        Tile2D/Physics
        Tile2D/Sound
        Tile2D/Util/DataStructures
        Tile2D/Util/Event
        Tile2D/Util/Primitives
        Tile2D/Util/Property
        Tile2D/Util
        Tile2D/Core
)

set(SOURCE_FILES
        3rdParty/json.cpp
        3rdParty/json.h
        Tile2D/Animation/AnimatedTexture.cpp
        Tile2D/Animation/AnimatedTexture.h
        Tile2D/Animation/AnimationBase.cpp
        Tile2D/Animation/AnimationBase.h
        Tile2D/Animation/AnimationManager.cpp
        Tile2D/Animation/AnimationManager.h
        Tile2D/Animation/IAnimation.h
        Tile2D/GUI/Canvas/Drawable/Background.cpp
        Tile2D/GUI/Canvas/Drawable/Background.h
        Tile2D/GUI/Canvas/Drawable/Color.cpp
        Tile2D/GUI/Canvas/Drawable/Color.h
        Tile2D/GUI/Canvas/Drawable/BasicDrawableBase.cpp
        Tile2D/GUI/Canvas/Drawable/BasicDrawableBase.h
        Tile2D/GUI/Canvas/Drawable/DrawableGroup.cpp
        Tile2D/GUI/Canvas/Drawable/DrawableGroup.h
        Tile2D/GUI/Canvas/Drawable/DrawableMap.cpp
        Tile2D/GUI/Canvas/Drawable/DrawableMap.h
        Tile2D/GUI/Canvas/Drawable/Font.cpp
        Tile2D/GUI/Canvas/Drawable/Font.h
        Tile2D/GUI/Canvas/Drawable/DrawableBase.h
        Tile2D/GUI/Canvas/Drawable/Plot.cpp
        Tile2D/GUI/Canvas/Drawable/Plot.h
        Tile2D/GUI/Canvas/Drawable/Sprite.cpp
        Tile2D/GUI/Canvas/Drawable/Sprite.h
        Tile2D/GUI/Canvas/Drawable/Text.cpp
        Tile2D/GUI/Canvas/Drawable/Text.h
        Tile2D/GUI/Canvas/Light/IShadowMask.cpp
        Tile2D/GUI/Canvas/Light/IShadowMask.h
        Tile2D/GUI/Canvas/Light/LightMap.cpp
        Tile2D/GUI/Canvas/Light/LightMap.h
        Tile2D/GUI/Canvas/Light/PartialLightMap.cpp
        Tile2D/GUI/Canvas/Light/PartialLightMap.h
        Tile2D/GUI/Canvas/Light/PartialLightMapUpdate.h
        Tile2D/GUI/Canvas/Light/PointLight.cpp
        Tile2D/GUI/Canvas/Light/PointLight.h
        Tile2D/GUI/Canvas/Light/PointLightMovedEventArgs.h
        Tile2D/GUI/Canvas/Light/ShadowMask.cpp
        Tile2D/GUI/Canvas/Light/ShadowMask.h
        Tile2D/GUI/Canvas/Texture/ITexture.h
        Tile2D/GUI/Canvas/Texture/MapTexture.cpp
        Tile2D/GUI/Canvas/Texture/MapTexture.h
        Tile2D/GUI/Canvas/Texture/Texture.cpp
        Tile2D/GUI/Canvas/Texture/Texture.h
        Tile2D/GUI/Canvas/Camera.cpp
        Tile2D/GUI/Canvas/Camera.h
        Tile2D/GUI/Canvas/Canvas.cpp
        Tile2D/GUI/Canvas/Canvas.h
        Tile2D/GUI/GuiComponentBase.cpp
        Tile2D/GUI/GuiComponentBase.h
        Tile2D/GUI/Window.cpp
        Tile2D/GUI/Window.h
        Tile2D/Physics/Tile.cpp
        Tile2D/Physics/Tile.h
        Tile2D/Physics/TileSet.cpp
        Tile2D/Physics/TileSet.h
        Tile2D/Physics/Body.cpp
        Tile2D/Physics/Body.h
        Tile2D/Physics/BodyCollisionEventArgs.cpp
        Tile2D/Physics/BodyCollisionEventArgs.h
        Tile2D/Physics/CollisionShape.cpp
        Tile2D/Physics/CollisionShape.h
        Tile2D/Physics/PhysicsWorld.cpp
        Tile2D/Physics/PhysicsWorld.h
        Tile2D/Physics/TileMap.cpp
        Tile2D/Physics/TileMap.h
        Tile2D/Physics/WorldMapModifiedEventArgs.h
        Tile2D/Sound/Ears.cpp
        Tile2D/Sound/Ears.h
        Tile2D/Sound/Music.cpp
        Tile2D/Sound/Music.h
        Tile2D/Sound/MusicPlayer.cpp
        Tile2D/Sound/MusicPlayer.h
        Tile2D/Sound/Player.cpp
        Tile2D/Sound/Player.h
        Tile2D/Sound/AudioClip.cpp
        Tile2D/Sound/AudioClip.h
        Tile2D/Sound/SamplePlayer.cpp
        Tile2D/Sound/SamplePlayer.h
        Tile2D/Util/DataStructures/Array2d.h
        Tile2D/Util/Event/Event.h
        Tile2D/Util/Event/EventArgs.cpp
        Tile2D/Util/Event/EventArgs.h
        Tile2D/Util/Event/EventHandler.h
        Tile2D/Util/Event/IEventHandler.h
        Tile2D/Util/Primitives/Rect.cpp
        Tile2D/Util/Primitives/Rect.h
        Tile2D/Util/Primitives/Vec.cpp
        Tile2D/Util/Primitives/Vec.h
        Tile2D/Util/Property/BooleanProperty.cpp
        Tile2D/Util/Property/BooleanProperty.h
        Tile2D/Util/Property/Property.h
        Tile2D/Util/Property/ReadableProperty.h
        Tile2D/Util/JsonFileManager.cpp
        Tile2D/Util/JsonFileManager.h
        Tile2D/Core/Tile2D.cpp
        Tile2D/Core/Tile2D.h
        Tile2D/Core/SceneManager.cpp
        Tile2D/Core/SceneManager.h
        Tile2D/Core/IScene.h
        Tile2D/Core/Resources.cpp
        Tile2D/Core/Resources.h
        Tile2D/Core/ResourceContainer.h
        Tile2D/Core/Tile2DObject.cpp
        Tile2D/Core/Tile2DObject.h
        main.cpp
        precompile.h
        Game/SceneInGame.cpp
        Game/SceneInGame.h
        Game/SceneTitleScreen.cpp
        Game/SceneTitleScreen.h
        Game/SceneGameOver.cpp
        Game/SceneGameOver.h
        Game/Scenes.h Tile2D/Core/GameObject.cpp Tile2D/Core/GameObject.h Tile2D/Core/Tile2DComponent.h Tile2D/GUI/Canvas/Drawable/DrawableBase.cpp Tile2D/Core/Tile2DBehaviour.cpp Tile2D/Core/Tile2DBehaviour.h Game/PlayerController.cpp Game/PlayerController.h)

add_executable(SpaceGame ${SOURCE_FILES})

target_link_libraries(SpaceGame
        ${OPENGL_LIBRARY}
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_MIXER_LIBRARY})

