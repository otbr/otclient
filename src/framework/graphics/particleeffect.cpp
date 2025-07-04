/*
 * Copyright (c) 2010-2025 OTClient <https://github.com/edubart/otclient>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "particleeffect.h"

#include "particlesystem.h"

void ParticleEffectType::load(const OTMLNodePtr& node)
{
    m_node = node->clone();
    for (const auto& childNode : node->children()) {
        if (childNode->tag() == "name")
            m_name = childNode->value();
        else if (childNode->tag() == "description")
            m_description = childNode->value();
    }
}

void ParticleEffect::load(const ParticleEffectTypePtr& effectType)
{
    if (!effectType)
        throw Exception("effect type not found");

    for (const auto& childNode : effectType->getNode()->children()) {
        if (childNode->tag() == "System") {
            const auto& system = std::make_shared<ParticleSystem>();
            system->load(childNode);
            m_systems.emplace_back(system);
        }
    }

    m_effectType = effectType;
}

void ParticleEffect::render() const
{
    for (const auto& system : m_systems)
        system->render();
}

void ParticleEffect::update()
{
    for (auto it = m_systems.begin(); it != m_systems.end();) {
        const auto& system = *it;

        if (system->hasFinished()) {
            it = m_systems.erase(it);
        } else {
            system->update();
            ++it;
        }
    }
}