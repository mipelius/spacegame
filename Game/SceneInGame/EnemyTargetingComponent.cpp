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

#include "Tile2DMath.h"
#include "EnemyTargetingComponent.h"
#include "EnemyAIBase.h"
#include "GameObject.h"
#include "Tile2D.h"
#include "t2Time.h"

Vecf EnemyTargetingComponent::getTargetPosition() {
    auto AI = gameObject()->getComponent<EnemyAIBase>();

    auto target = AI->getTarget();
    auto targetVelocity = target->getComponent<Body>()->getVelocity();
    auto targetPosition = target->transform().getPosition();

    auto targetApproximatedFuturePosition = targetPosition + (targetVelocity * Tile2D::time().getDeltaTime() * 12.0f);
    auto normal = Vecf(targetApproximatedFuturePosition.y, -targetApproximatedFuturePosition.x).normalized();
    auto error = normal * Mathf::random(-40.0f, 40.0f);

    return targetApproximatedFuturePosition + error;
}

Tile2DComponent *EnemyTargetingComponent::clone() {
    return new EnemyTargetingComponent(*this);
}

void EnemyTargetingComponent::deserialize(const json::Object &jsonObject) { }
