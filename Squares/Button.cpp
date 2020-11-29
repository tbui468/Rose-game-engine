#include "Button.h"

namespace sqs {


Button::Button(const rose::Sprite& sprite, const glm::vec2& size, const glm::vec4& boundingBox, const glm::vec2& pos): 
    Entity(sprite, size, boundingBox, pos) {}

void Button::GoAway() { MoveTo(m_OffScreenCoords); }

void Button::ComeBack() { MoveTo(m_OnScreenCoords); }

void Button::SetAnimationCoords(const glm::vec2& on, const glm::vec2& off) {
    m_OffScreenCoords = off;
    m_OnScreenCoords = on;
}


}
