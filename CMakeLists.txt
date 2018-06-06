# MIT License
#
# This file is part of SpaceGame.
# Copyright (c) 2014-2018 Miika Pelkonen
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

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
        Game/SceneInGame
        Game/SceneTitleScreen
        Game/SceneEndScreen
        Game/SceneQuickTesting
        Game/SceneInGame/Items
        Tile2D/Animation
        Tile2D/LightSystem
        Tile2D/Canvas/ParticleSystem
        Tile2D/Canvas/Drawable
        Tile2D/Canvas/Texture
        Tile2D/Canvas
        Tile2D/UI
        Tile2D/Physics
        Tile2D/Sound
        Tile2D/Util/DataStructures
        Tile2D/Util/Event
        Tile2D/Util/Primitives
        Tile2D/Util/Property
        Tile2D/Util
        Tile2D/Core
        Tile2D/PathFinding
)

set(SOURCE_FILES
        3rdParty/json.cpp
        3rdParty/json.h
        Tile2D/Canvas/Drawable/Background.cpp
        Tile2D/Canvas/Drawable/Background.h
        Tile2D/Canvas/Drawable/Color.cpp
        Tile2D/Canvas/Drawable/Color.h
        Tile2D/Canvas/Drawable/DrawableMap.cpp
        Tile2D/Canvas/Drawable/DrawableMap.h
        Tile2D/Canvas/Drawable/Font.cpp
        Tile2D/Canvas/Drawable/Font.h
        Tile2D/Canvas/Drawable/DrawableBase.h
        Tile2D/Canvas/Drawable/DrawableBase.cpp
        Tile2D/Canvas/Drawable/Sprite.cpp
        Tile2D/Canvas/Drawable/Sprite.h
        Tile2D/Canvas/Drawable/Text.cpp
        Tile2D/Canvas/Drawable/Text.h
        Tile2D/UI/Button.cpp
        Tile2D/UI/Button.h
        Tile2D/LightSystem/PointLight.cpp
        Tile2D/LightSystem/PointLight.h
        Tile2D/LightSystem/LightSystem.cpp
        Tile2D/LightSystem/LightSystem.h
        Tile2D/Canvas/ParticleSystem/Particle.cpp
        Tile2D/Canvas/ParticleSystem/Particle.h
        Tile2D/Canvas/ParticleSystem/ParticleSystem.cpp
        Tile2D/Canvas/ParticleSystem/ParticleSystem.h
        Tile2D/Canvas/Texture/MapTexture.cpp
        Tile2D/Canvas/Texture/MapTexture.h
        Tile2D/Canvas/Texture/Texture.cpp
        Tile2D/Canvas/Texture/Texture.h
        Tile2D/Canvas/Camera.cpp
        Tile2D/Canvas/Camera.h
        Tile2D/Canvas/Canvas.cpp
        Tile2D/Canvas/Canvas.h
        Tile2D/Physics/Tile.cpp
        Tile2D/Physics/Tile.h
        Tile2D/Physics/TileSet.cpp
        Tile2D/Physics/TileSet.h
        Tile2D/Physics/Body.cpp
        Tile2D/Physics/Body.h
        Tile2D/Physics/PhysicsWorld.cpp
        Tile2D/Physics/PhysicsWorld.h
        Tile2D/Physics/TileMap.cpp
        Tile2D/Physics/TileMap.h
        Tile2D/Physics/WorldMapModifiedEventArgs.h
        Tile2D/Physics/PolygonCollider.cpp
        Tile2D/Physics/PolygonCollider.h
        Tile2D/Physics/ColliderLayerMatrix.cpp
        Tile2D/Physics/ColliderLayerMatrix.h
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
        Tile2D/Util/Primitives/Rect.cpp
        Tile2D/Util/Primitives/Rect.h
        Tile2D/Util/Primitives/Vec.h
        Tile2D/Util/Primitives/LineSegment.cpp
        Tile2D/Util/Primitives/LineSegment.h
        Tile2D/Util/JsonFileManager.cpp
        Tile2D/Util/JsonFileManager.h
        Tile2D/Util/gl_utils.h
        Tile2D/Util/Pool.h
        Tile2D/Util/Tile2DMath.h
        Tile2D/Util/Timer.cpp
        Tile2D/Util/Timer.h
        Tile2D/Util/CountDownTimer.cpp
        Tile2D/Util/CountDownTimer.h
        Tile2D/Core/ObjectCreator.h
        Tile2D/Core/Tile2DComponent.cpp
        Tile2D/Core/Tile2DComponent.h
        Tile2D/Core/Tile2DBehaviour.cpp
        Tile2D/Core/Tile2DBehaviour.h
        Tile2D/Core/Tile2D.cpp
        Tile2D/Core/Tile2D.h
        Tile2D/Core/SceneManager.cpp
        Tile2D/Core/SceneManager.h
        Tile2D/Core/IScene.h
        Tile2D/Core/Resources.cpp
        Tile2D/Core/Resources.h
        Tile2D/Core/ResourceContainer.h
        Tile2D/Core/GameObject.cpp
        Tile2D/Core/GameObject.h
        Tile2D/Core/Window.cpp
        Tile2D/Core/Window.h
        Tile2D/Core/Input.cpp
        Tile2D/Core/Input.h
        Tile2D/Core/Transform.cpp
        Tile2D/Core/Transform.h
        Tile2D/PathFinding/PathFinder.cpp
        Tile2D/PathFinding/PathFinder.h
        Tile2D/PathFinding/IPathValidator.cpp
        Tile2D/PathFinding/IPathValidator.h
        Tile2D/PathFinding/PathValidator.cpp
        Tile2D/PathFinding/PathValidator.h

        precompile.h

        Game/main.cpp
        Game/Scenes.h
        Game/SortingLayers.h
        Game/Tags.h
        Game/ColliderLayers.h

        Game/SceneEndScreen/SceneEndScreen.cpp
        Game/SceneEndScreen/SceneEndScreen.h

        Game/SceneTitleScreen/SceneTitleScreen.cpp
        Game/SceneTitleScreen/SceneTitleScreen.h
        Game/SceneTitleScreen/MovingCameraBehaviour.cpp
        Game/SceneTitleScreen/MovingCameraBehaviour.h

        Game/SceneInGame/SceneInGame.cpp
        Game/SceneInGame/SceneInGame.h
        Game/SceneInGame/BackgroundBehaviour.cpp
        Game/SceneInGame/BackgroundBehaviour.h
        Game/SceneInGame/BombBehaviour.cpp
        Game/SceneInGame/BombBehaviour.h
        Game/SceneInGame/DebugBehaviour.cpp
        Game/SceneInGame/DebugBehaviour.h
        Game/SceneInGame/EnemyAIBase.cpp
        Game/SceneInGame/EnemyAIBase.h
        Game/SceneInGame/FlyingEnemyAI.cpp
        Game/SceneInGame/FlyingEnemyAI.h
        Game/SceneInGame/Health.cpp
        Game/SceneInGame/Health.h
        Game/SceneInGame/LimitedLifetimeBehaviour.cpp
        Game/SceneInGame/LimitedLifetimeBehaviour.h
        Game/SceneInGame/PlayerController.cpp
        Game/SceneInGame/PlayerController.h
        Game/SceneInGame/Prefabs.cpp
        Game/SceneInGame/Prefabs.h
        Game/SceneInGame/PulseLightBehaviour.cpp
        Game/SceneInGame/PulseLightBehaviour.h
        Game/SceneInGame/SparkleBehaviour.cpp
        Game/SceneInGame/SparkleBehaviour.h
        Game/SceneInGame/EnemySpawner.cpp
        Game/SceneInGame/EnemySpawner.h
        Game/SceneInGame/WalkingEnemyAI.cpp
        Game/SceneInGame/WalkingEnemyAI.h

        Game/UIPrefabs.cpp
        Game/UIPrefabs.h
        Game/SceneInGame/HUD.cpp
        Game/SceneInGame/HUD.h

        Tile2D/Core/t2Time.cpp
        Tile2D/Core/t2Time.h

        Game/SceneQuickTesting/SceneQuickTesting.cpp
        Game/SceneQuickTesting/SceneQuickTesting.h
        Game/SceneQuickTesting/QuickTestingBehaviour.cpp
        Game/SceneQuickTesting/QuickTestingBehaviour.h
        Tile2D/Animation/Animation.cpp
        Tile2D/Animation/Animation.h

        Tile2D/Canvas/Drawable/SpriteBase.cpp
        Tile2D/Canvas/Drawable/SpriteBase.h
        Tile2D/Animation/AnimatedSprite.cpp
        Tile2D/Animation/AnimatedSprite.h

        Game/SceneInGame/Power.cpp
        Game/SceneInGame/Power.h
        Game/SceneInGame/Items/Inventory.cpp
        Game/SceneInGame/Items/Inventory.h
        Game/SceneInGame/Items/WeaponBase.cpp
        Game/SceneInGame/Items/WeaponBase.h
        Game/SceneInGame/Items/ItemBase.cpp
        Game/SceneInGame/Items/ItemBase.h
        Game/SceneInGame/Items/Cannon.cpp
        Game/SceneInGame/Items/Cannon.h
        Game/SceneInGame/Items/BombDropper.cpp
        Game/SceneInGame/Items/BombDropper.h
        Game/SceneInGame/Items/Healer.cpp
        Game/SceneInGame/Items/Healer.h
        Game/SceneInGame/Items/ItemTags.h
        Game/SceneInGame/SwirlingBehaviour.cpp
        Game/SceneInGame/SwirlingBehaviour.h
        Game/SceneInGame/TargetingComponentBase.cpp
        Game/SceneInGame/TargetingComponentBase.h
        Game/SceneInGame/PlayerTargetingComponent.cpp
        Game/SceneInGame/PlayerTargetingComponent.h
        Game/SceneInGame/EnemyTargetingComponent.cpp
        Game/SceneInGame/EnemyTargetingComponent.h
        Game/SceneInGame/AmmoComponent.cpp
        Game/SceneInGame/AmmoComponent.h
        Game/SceneInGame/SpawnerBase.cpp
        Game/SceneInGame/SpawnerBase.h
        Game/SceneInGame/Spawnable.cpp
        Game/SceneInGame/Spawnable.h
        Game/SceneInGame/PickupSpawner.cpp
        Game/SceneInGame/PickupSpawner.h
        Game/SceneInGame/Pickup.cpp
        Game/SceneInGame/Pickup.h
        Tile2D/Core/ISerializable.h
        Tile2D/Core/Reflector.cpp Tile2D/Core/Reflector.h Tile2D/Core/Prefab.cpp Tile2D/Core/Prefab.h)

add_executable(SpaceGame ${SOURCE_FILES})

target_link_libraries(SpaceGame
        ${OPENGL_LIBRARY}
        ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARY}
        ${SDL2_MIXER_LIBRARY})

