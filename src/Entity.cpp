#include "Entity.h"

namespace rose {


Entity::Entity(const Sprite& sprite, const glm::vec2& pos) {
    m_Sprite = sprite;
    SetBoundingBox(0.0f, 0.0f, static_cast<float>(sprite.TexDimensions.x), static_cast<float>(sprite.TexDimensions.y));
    SetPosition(pos);
}

void Entity::MoveTo(const glm::vec2& _pos) {
    m_FromPos = {x, y};
    m_ToPos = {_pos.x, _pos.y};
}
void Entity::FadeTo(float _alpha) {
    m_FromAlpha = alpha;
    m_ToAlpha = _alpha;
}
void Entity::ScaleTo(const glm::vec2& _scale) {
    m_FromScale = {xScale, yScale};
    m_ToScale = _scale;
}
void Entity::RotateTo(float _angle) {
    m_FromAngle = angle;
    m_ToAngle = _angle;
}
void Entity::OnAnimationUpdate(float _t) {
    x = m_FromPos.x + (m_ToPos.x - m_FromPos.x) * _t;
    y = m_FromPos.y + (m_ToPos.y - m_FromPos.y) * _t;
    xScale = m_FromScale.x + (m_ToScale.x - m_FromScale.x) * _t;
    yScale = m_FromScale.y + (m_ToScale.y - m_FromScale.y) * _t;
    angle = m_FromAngle + (m_ToAngle - m_FromAngle) * _t;
    alpha = m_FromAlpha + (m_ToAlpha - m_FromAlpha) * _t;
}
void Entity::OnAnimationEnd() {
    x = m_ToPos.x;
    y = m_ToPos.y;
    m_FromPos.x = x;
    m_FromPos.y = y;
    xScale = m_ToScale.x;
    yScale = m_ToScale.y;
    m_FromScale.x = xScale;
    m_FromScale.y = yScale;
    angle = m_ToAngle;
    m_FromAngle = angle;
    alpha = m_ToAlpha;
    m_FromAlpha = alpha;
}


bool Entity::PointCollision(float pointX, float pointY) const {
    float left = x + m_BoundingBox.x - m_BoundingBox.z * .5f;
    float right = x + m_BoundingBox.x + m_BoundingBox.z * .5f;
    float upper = y + m_BoundingBox.y - m_BoundingBox.w * .5f;
    float lower = y + m_BoundingBox.y + m_BoundingBox.w * .5f;
    if(pointX < left) return false;
    if(pointY < upper) return false;
    if(pointX > right) return false;
    if(pointY > lower) return false;
    return true;
}

void Entity::SetBoundingBox(float _x, float _y, float _w, float _h) {
    m_BoundingBox.x = _x;
    m_BoundingBox.y = _y;
    m_BoundingBox.z = _w;
    m_BoundingBox.w = _h;
}

void Entity::SetPosition(const glm::vec2& _coords) {
    x = _coords.x;
    y = _coords.y;
    m_FromPos = {_coords.x, _coords.y};
    m_ToPos = {_coords.x, _coords.y};
}

glm::mat4 Entity::GetModelMatrix() const {
    glm::mat4 rotated = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 scaled = glm::scale(glm::mat4(1.0f), glm::vec3(xScale, yScale, 1.0f));
    glm::mat4 translated = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
    return translated * scaled * rotated;
}


}
